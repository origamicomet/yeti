//===-- yeti/foundation/thread.cc -----------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/thread.h"

#include "yeti/foundation/global_heap_allocator.h"
#include "yeti/foundation/thread_safe/scratch_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
  #include <intrin.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  #include <unistd.h>
  #include <pthread.h>
  #include <limits.h>
  #include <sched.h>
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace foundation {

namespace {
  static const Thread::Options default_thread_opts_ = {
    // By default, threads aren't named.
    {'\0',},

    // By default, threads can be scheduled on any core.
    ~UINT64_C(0),

    // By default, we use 1MiB stacks.
    UINT64_C(0x100000)
  };
}

Thread::Thread()
  : native_hndl_(NULL)
{
}

Thread::~Thread() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (native_hndl_)
    ::CloseHandle((HANDLE)native_hndl_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

namespace {
  struct ThreadStartInfo {
    // See `include/yeti/foundation/thread.h`.
    Thread::Name name;

    // TODO(mtwilliams): Allow more than one argument.
    Thread::EntryPoint entry_point;
    uintptr_t entry_point_arg;
  };

  // NOTE(mtwilliams): Decreasing this can cause significant contention (and
  // thus performance degredation) if the number of outstanding threads (i.e.
  // not hit `thread_entry_point` and deallocated their respective
  // ThreadStartInfo) exceeds the amount of memory made available.
  static const size_t thread_start_info_mem_sz_ = 12288;
  static u8 thread_start_info_mem_[thread_start_info_mem_sz_] = { 0, };
  static thread_safe::ScratchAllocator thread_start_info_allocator_((uintptr_t)thread_start_info_mem_, thread_start_info_mem_sz_);
}

namespace {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #if YETI_COMPILER == YETI_COMPILER_MSVC
    // Refer to this article:
     // https://blogs.msdn.microsoft.com/stevejs/2005/12/20/naming-threads-in-win32-and-net/
    #pragma pack(push, 8)
    struct THREAD_NAME_INFO {
      DWORD dwType;
      LPCSTR szName;
      DWORD dwThreadID;
      DWORD dwFlags;
    };
    #pragma pack(pop)
  #endif

  static DWORD thread_entry_point(uintptr_t thread_start_info_ptr) {
    const ThreadStartInfo *thread_start_info =
      (const ThreadStartInfo *)thread_start_info_ptr;

    Thread::EntryPoint entry_point = thread_start_info->entry_point;
    uintptr_t entry_point_arg = thread_start_info->entry_point_arg;

  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
      YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
    // TODO(mtwilliams): Raise exception regardless of compiler?
    #if YETI_COMPILER == YETI_COMPILER_MSVC
      THREAD_NAME_INFO thread_name_info;
      thread_name_info.dwType = 0x1000;
      thread_name_info.szName = &thread_start_info->name[0];
      thread_name_info.dwThreadID = ::GetCurrentThreadId();
      thread_name_info.dwFlags = 0;

      #pragma warning(push)
      #pragma warning(disable: 6320 6322)
        __try {
          ::RaiseException(0x406D1388, 0, sizeof(THREAD_NAME_INFO)/sizeof(ULONG_PTR), (ULONG_PTR *)&thread_name_info);
        } __except (EXCEPTION_EXECUTE_HANDLER) {
        }
      #pragma warning(pop)
    #endif
  #endif

    // Return memory to |thread_start_info_allocator_| as soon as possible.
    thread_start_info_allocator_.deallocate(thread_start_info_ptr);

    entry_point(entry_point_arg);

    return 0x00000000ul;
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  static void *thread_entry_point(void *thread_start_info_ptr) {
    const ThreadStartInfo *thread_start_info =
      (const ThreadStartInfo *)thread_start_info_ptr;

    Thread::EntryPoint entry_point = thread_start_info->entry_point;
    uintptr_t entry_point_arg = thread_start_info->entry_point_arg;

    // QUESTION(mtwilliams): Should we only name threads in debug and
    // development builds?
    ::pthread_setname_np(&thread_start_info->name[0]);

    // Return memory to |thread_start_info_allocator_| as soon as possible.
    thread_start_info_allocator_.deallocate((uintptr_t)thread_start_info_ptr);

    entry_point(entry_point_arg);

    return NULL;
  }
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  // http://stackoverflow.com/questions/2369738/can-i-set-the-name-of-a-thread-in-pthreads-linux
  // http://stackoverflow.com/questions/778085/how-to-name-a-thread-in-linux?lq=1
#endif
}

Thread *Thread::spawn(Thread::EntryPoint entry_point,
                      uintptr_t entry_point_arg) {
  return Thread::spawn(entry_point, entry_point_arg, &default_thread_opts_);
}

Thread *Thread::spawn(Thread::EntryPoint entry_point,
                      uintptr_t entry_point_arg,
                      const Thread::Options *options) {
  yeti_assert_debug(entry_point != NULL);
  yeti_assert_debug(options != NULL);
  yeti_assert_debug(options->affinity != 0);
  yeti_assert_debug(options->stack_size != 0);

  // TODO(mtwilliams): Move retry logic back into |thread_start_info_allocator_|?
  ThreadStartInfo *thread_start_info = NULL;
  while (thread_start_info == NULL) {
    thread_start_info = (ThreadStartInfo *)thread_start_info_allocator_.allocate(sizeof(ThreadStartInfo));
    if (thread_start_info == NULL)
      // Heavily contended; back off.
      Thread::yield();
  }

  strncpy((char *)thread_start_info->name, (const char *)options->name, sizeof(Thread::Name));
  thread_start_info->entry_point = entry_point;
  thread_start_info->entry_point_arg = entry_point_arg;

  Thread *thread = new (foundation::heap()) Thread();

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  thread->native_hndl_ =
    (uintptr_t)::CreateThread(NULL,
                              options->stack_size,
                              (LPTHREAD_START_ROUTINE)&thread_entry_point,
                              (LPVOID)thread_start_info,
                              CREATE_SUSPENDED,
                              NULL);

  yeti_assert((HANDLE)thread->native_hndl_ != INVALID_HANDLE_VALUE);

  if (options->affinity != ~0ull) {
    // NOTE(mtwilliams): This will truncate to 32 bits, and therefor 32 cores,
    // if on 32 bit. There's not much we can do.
    ::SetThreadAffinityMask((HANDLE)thread->native_hndl_, (DWORD_PTR)options->affinity);

    // PROFILE(mtwilliams): Determine if this improves scheduling at all.
    if (options->affinity) {
      if (options->affinity & (options->affinity - 1)) {
        DWORD ideal_processor;

        // TODO(mtwilliams): Replace intrinsics with common utility functions.
      #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
        if (!_BitScanForward(&ideal_processor, (DWORD)options->affinity)) {
          _BitScanForward(&ideal_processor, (DWORD)(options->affinity >> 32));
          ideal_processor += 32;
        }
      #else YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
        _BitScanForward64(&ideal_processor, options->affinity);
      #endif

        ::SetThreadIdealProcessor((HANDLE)thread->native_hndl_, ideal_processor);
      }
    }
  }

  // Be free!
  ::ResumeThread((HANDLE)thread->native_hndl_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  yeti_assert_debug(options->stack_size >= PTHREAD_STACK_MIN);
  yeti_assert_debug((options->stack_size % getpagesize()) == 0);

  // NOTE(mtwilliams): POSIX allows implementations to define `pthread_t` as
  // a structure. Fortunately for us, Apple has defined it as a pointer (to an
  // opaque structure.)

  pthread_attr_t attributes;

  yeti_assert(pthread_attr_init(&attributes) == 0);
  yeti_assert(pthread_attr_setstacksize(&attributes, options->stack_size) == 0);

  const int result =
    ::pthread_create(
      (pthread_t *)&thread->native_hndl_,
      &attributes,
      &thread_entry_point,
      (void *)thread_start_info);

  yeti_assert(result == 0);

  pthread_attr_destroy(&attributes);

  if (options->affinity != ~0ull) {
    // NOTE(mtwilliams): Mach doesn't allow explicit thread placement, so we
    // have to ignore affinity. However, we can use "affinity sets" (as of
    // Mac OS X 10.5) as a hint to Mach to schedule a thread on a particular
    // "processor."

    if (options->affinity & (options->affinity - 1)) {
      // TODO(mtwilliams): Hint for a specific processor.
       // Refer to http://superuser.com/questions/149312.
    }
  }
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

  return thread;
}

void Thread::join() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // QUESTION(mtwilliams): Should we check that this returns `WAIT_OBJECT_0`?
  ::WaitForSingleObject((HANDLE)native_hndl_, INFINITE);
  // Handle is closed in destructor.
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  ::pthread_join((pthread_t)native_hndl_, NULL);
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Thread::detach() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // Handle is closed in destructor.
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  ::pthread_detach((pthread_t)native_hndl_);
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Thread::terminate() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // I drink your milkshake...
  ::TerminateThread((HANDLE)native_hndl_, 0x0);
  // Handle is closed in destructor.
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  ::pthread_cancel((pthread_t)native_hndl_);
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Thread::yield() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (::SwitchToThread() == 0)
    // Windows didn't schedule another thread, so try to force it.
    ::Sleep(0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  sched_yield();
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // foundation
} // yeti
