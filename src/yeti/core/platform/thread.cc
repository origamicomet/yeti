//===-- yeti/core/platform/thread.cc --------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/thread.h"

#include "yeti/core/allocators/global_heap_allocator.h"
#include "yeti/core/allocators/thread_safe/scratch_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
  #include <intrin.h>
#endif

#if YETI_PLATFORM == YETI_PLATFORM_MAC || \
    YETI_PLATFORM == YETI_PLATFORM_LINUX
  #include <limits.h>
  #include <unistd.h>
  #include <pthread.h>
  #include <sched.h>
#endif

#if YETI_PLATFORM == YETI_PLATFORM_LINUX
  // We use Linux specific process control to set thread name from within our
  // entry point, rather than setting thread name in `Thread::spawn`.
  #include <sys/prctl.h>

  // To get TID.
  #include <sys/types.h>
#endif

namespace yeti {
namespace core {

namespace {
  struct ThreadStartInfo {
    char name[256];

    // TODO(mtwilliams): Allow more than one argument?
    Thread::EntryPoint entry_point;
    void *entry_point_argument;
  };

  // NOTE(mtwilliams): Decreasing this can cause significant contention (and
  // thus performance degredation) if the number of outstanding threads (i.e.
  // not hit `thread_entry_point` and deallocated their respective
  // ThreadStartInfo) exceeds the amount of memory made available.
  static const size_t thread_start_info_memory_size_ = 16384;
  static u8 thread_start_info_memory_[thread_start_info_memory_size_] = { 0, };
  static thread_safe::ScratchAllocator thread_start_info_allocator_((void *)thread_start_info_memory_,
                                                                    thread_start_info_memory_size_);

  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    #if YETI_COMPILER == YETI_COMPILER_MSVC
      #pragma pack(push, 8)
      struct THREAD_NAME_INFO {
        DWORD dwType;
        LPCSTR szName;
        DWORD dwThreadID;
        DWORD dwFlags;
      };
      #pragma pack(pop)
    #endif

    static DWORD WINAPI thread_entry_point(LPVOID thread_start_info_ptr) {
      const ThreadStartInfo *thread_start_info =
        (const ThreadStartInfo *)thread_start_info_ptr;

      Thread::EntryPoint entry_point = thread_start_info->entry_point;
      void *entry_point_argument = thread_start_info->entry_point_argument;

    #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
      // TODO(mtwilliams): Raise exception regardless of compiler?
      #if YETI_COMPILER == YETI_COMPILER_MSVC
        THREAD_NAME_INFO thread_name_info;
        thread_name_info.dwType = 0x1000;
        thread_name_info.szName = &thread_start_info->name[0];
        thread_name_info.dwThreadID = GetCurrentThreadId();
        thread_name_info.dwFlags = 0;

        #pragma warning(push)
        #pragma warning(disable: 6320 6322)
          __try {
            RaiseException(0x406D1388, 0, sizeof(THREAD_NAME_INFO)/sizeof(ULONG_PTR), (ULONG_PTR *)&thread_name_info);
          } __except (EXCEPTION_EXECUTE_HANDLER) {
          }
        #pragma warning(pop)
      #endif
    #endif

      // Return memory to `thread_start_info_allocator_` as soon as possible.
      thread_start_info_allocator_.deallocate(thread_start_info_ptr);

      entry_point(entry_point_argument);

      return 0;
    }
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
        YETI_PLATFORM == YETI_PLATFORM_LINUX
    static void *loom_thread_entry_point(void *thread_start_info_ptr) {
      const ThreadStartInfo *thread_start_info =
        (const ThreadStartInfo *)thread_start_info_ptr;

      Thread::EntryPoint entry_point = thread_start_info->entry_point;
      void *entry_point_argument = thread_start_info->entry_point_argument;

    #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
      #if YETI_PLATFORM == YETI_PLATFORM_MAC
        pthread_setname_np(&thread_start_info->name[0]);
      #elif YETI_PLATFORM == YETI_PLATFORM_LINUX
        prctl(PR_SET_NAME, &thread_start_info->name[0], 0, 0, 0);
      #endif
    #endif

      // Return memory to `thread_start_info_allocator_` as soon as possible.
      thread_start_info_allocator_.deallocate(thread_start_info_ptr);

      entry_point(entry_point_argument);

      return NULL;
    }
  #endif

  static size_t round_and_bound_stack(size_t stack) {
  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    if (stack < 0x100000)
      return 0x100000;

    if (stack % 0x100000)
      // Round up to nearest MiB.
      stack += 0x100000 - (stack % 0x100000);

    return stack;
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
        YETI_PLATFORM == YETI_PLATFORM_LINUX
    if (stack < PTHREAD_STACK_MIN)
      return PTHREAD_STACK_MIN;

    const size_t page = getpagesize();

    if (stack % page)
      // Round up to nearest page.
      stack += page - (stack % page);

    return stack;
  #endif
  }
}

Thread::Thread() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  handle_ = (uintptr_t)INVALID_HANDLE_VALUE;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  handle_ = NULL;
#endif
}

Thread::~Thread() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if ((HANDLE)handle_ != INVALID_HANDLE_VALUE)
    ::CloseHandle((HANDLE)handle_);
#endif
}

Thread *Thread::spawn(EntryPoint entry_point,
                      void *entry_point_argument) {
  static const Thread::Options DEFAULTS = {
    // Unnamed.
    NULL,

    // Any available core.
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    ~0ul,
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    ~0ull,
  #endif

    // 4 MiB stack.
    0x400000
  };

  return Thread::spawn(entry_point, entry_point_argument, DEFAULTS);
}

Thread *Thread::spawn(EntryPoint entry_point,
                      void *entry_point_argument,
                      const Options &options) {
  Thread *thread = YETI_NEW(Thread, core::global_heap_allocator());

  ThreadStartInfo *thread_start_info = NULL;

  while (!thread_start_info) {
    thread_start_info =
      (ThreadStartInfo *)thread_start_info_allocator_.allocate(sizeof(ThreadStartInfo),
                                                               alignof(ThreadStartInfo));

    if (!thread_start_info == NULL)
      // Contended; back off.
      Thread::yield();
  }

  if (options.name) {
    // Silently truncates thread name.
    static const size_t length = sizeof(thread_start_info->name);
    strncpy(&thread_start_info->name[0], options.name, length - 1);
    thread_start_info->name[length - 1] = '\0';
  } else {
    thread_start_info->name[0] = '\0';
  }

  thread_start_info->entry_point = entry_point;
  thread_start_info->entry_point_argument = entry_point_argument;

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  thread->handle_ =
    (uintptr_t)::CreateThread(NULL,
                              round_and_bound_stack(options.stack),
                              (LPTHREAD_START_ROUTINE)&thread_entry_point,
                              (LPVOID)thread_start_info,
                              CREATE_SUSPENDED | STACK_SIZE_PARAM_IS_A_RESERVATION,
                              NULL);

  if (~options.affinity != 0) {
    // NOTE(mtwilliams): This will truncate to 32 bits, and therefore 32 cores,
    // if on 32 bit. There's not much we can do.
    ::SetThreadAffinityMask((HANDLE)thread->handle_, (DWORD_PTR)options.affinity);

    if (options.affinity) {
      if (options.affinity & (options.affinity - 1)) {
        DWORD processor;

        // TODO(mtwilliams): Replace intrinsics with common utility functions.
      #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
        _BitScanForward(&processor, options.affinity);
      #else YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
        _BitScanForward64(&processor, options.affinity);
      #endif

        ::SetThreadIdealProcessor((HANDLE)thread->handle_, processor);
      }
    }
  }

  // Be free!
  ::ResumeThread((HANDLE)thread->handle_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  pthread_attr_t attributes;

  ::pthread_attr_init(&attributes);

  ::pthread_attr_setstacksize(&attributes, round_and_bound_stack(options.stack));

#if YETI_PLATFORM == YETI_PLATFORM_LINUX
  cpu_set_t cpus;

  if (~options.affinity) {
    CPU_ZERO(&cpus);

  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    static const unsigned n = 32;
  #else YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    static const unsigned n = 64;
  #endif

    // Naive, but simpler than iterating set bits.
    for (unsigned cpu = 0; cpu < n; ++)
      if ((options.affinity >> cpu) & 1)
        CPU_SET(cpu, &cpus);
  } else {
    // HACK(mtwilliams): Technically opaque, so we shouldn't do this.
    memset((void *)&cpus, ~0, sizeof(cpus));
  }

  ::pthread_attr_setaffinity_np(&attributes, CPU_SETSIZE, &cpus);
#endif

  ::pthread_create((pthread_t *)&thread->handle_,
                   &attributes,
                   &thread_entry_point,
                   (void *)thread_start_info);

  ::pthread_attr_destroy(&attributes);

  #if YETI_PLATFORM == YETI_PLATFORM_MAC
    // NOTE(mtwilliams): Mach doesn't allow explicit thread placement, so we
    // have to ignore affinity. However, we can use "affinity sets" (as of
    // Mac OS X 10.5) as a hint to Mach to schedule a thread on a particular
    // logical processor.
    if (options.affinity != ~0ull) {
      if (options.affinity & (options.affinity - 1)) {
        // TODO(mtwilliams): Hint for a specific processor.
      }
    }
  #endif
#endif

  return thread;
}

void Thread::join() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // INVESTIGATE(mtwilliams): Should we check that this returns `WAIT_OBJECT_0`?
  ::WaitForSingleObject((HANDLE)handle_, INFINITE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  ::pthread_join((pthread_t)handle_, NULL);
#endif

  YETI_DELETE(Thread, core::global_heap_allocator(), this);
}

void Thread::detach() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // Handle is closed by destructor.
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  ::pthread_detach((pthread_t)handle_);
#endif

  YETI_DELETE(Thread, core::global_heap_allocator(), this);
}

void Thread::terminate() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // I drink your milkshake...
  ::TerminateThread((HANDLE)handle_, 0x0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  ::pthread_cancel((pthread_t)handle_);
#endif

  YETI_DELETE(Thread, core::global_heap_allocator(), this);
}

u64 Thread::id() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (u64)::GetCurrentThreadId();
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  u64 tid;
  pthread_threadid_np(NULL, &tid);
  return tid;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return (u64)::gettid();
#endif
}

void Thread::yield() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (::SwitchToThread() == 0)
    // Windows didn't schedule another thread, so try to force it.
    ::Sleep(0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  ::sched_yield();
#endif
}

} // core
} // yeti
