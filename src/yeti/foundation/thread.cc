//===-- yeti/foundation/thread.cc -------------------------*- mode: C++ -*-===//
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

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace foundation {

namespace {
  static const Thread::Options default_thread_opts_ = {
    // By default, threads aren't named.
    {'\0',},

    // By default, threads can be scheduled on any core.
    0xFFFFFFFFul,

    // By default, we use 1MiB stacks.
    0x100000ul
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
    const ThreadStartInfo *thread_start_info = (const ThreadStartInfo *)thread_start_info_ptr;

    Thread::EntryPoint entry_point = thread_start_info->entry_point;
    uintptr_t entry_point_arg = thread_start_info->entry_point_arg;

  #if YETI_COMPILER == YETI_COMPILER_MSVC
    THREAD_NAME_INFO thread_name_info;
    thread_name_info.dwType = 0x1000;
    thread_name_info.szName = &thread_start_info->name[0];
    thread_name_info.dwThreadID = ::GetCurrentThreadId();
    thread_name_info.dwFlags = 0;
    #pragma warning(push)
    #pragma warning(disable: 6320 6322)
      __try{
        ::RaiseException(0x406D1388, 0, sizeof(THREAD_NAME_INFO)/sizeof(ULONG_PTR), (ULONG_PTR *)&thread_name_info);
      } __except (EXCEPTION_EXECUTE_HANDLER){
      }
    #pragma warning(pop)
  #endif

    heap().deallocate(thread_start_info_ptr);

    entry_point(entry_point_arg);

    return 0x00000000ul;
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  // http://stackoverflow.com/questions/2057960/how-to-set-a-threadname-in-macosx?lq=1
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

  Thread *thread = new (foundation::heap()) Thread();

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ThreadStartInfo *thread_start_info = new (heap()) ThreadStartInfo;
  strncpy((char *)thread_start_info->name, (const char *)options->name, sizeof(Thread::Name));
  thread_start_info->entry_point = entry_point;
  thread_start_info->entry_point_arg = entry_point_arg;

  thread->native_hndl_ =
    (uintptr_t)::CreateThread(NULL,
                              options->stack_size,
                              (LPTHREAD_START_ROUTINE)&thread_entry_point,
                              (LPVOID)thread_start_info,
                              CREATE_SUSPENDED,
                              NULL);

  yeti_assert((HANDLE)thread->native_hndl_ != INVALID_HANDLE_VALUE);

  if (options->affinity != 0xFFFFFFFFul) {
    // TODO(mtwilliams): Expose a 64-bit affinity mask?
    ::SetThreadAffinityMask((HANDLE)thread->native_hndl_, (DWORD_PTR)options->affinity);

    // PROFILE(mtwilliams): Determine if this improves scheduling at all.
    if (options->affinity & (options->affinity - 1)) {
      DWORD ideal_processor;
      _BitScanForward(&ideal_processor, (DWORD)options->affinity);
      ::SetThreadIdealProcessor((HANDLE)thread->native_hndl_, ideal_processor);
    }
  }

  // Be free!
  ::ResumeThread((HANDLE)thread->native_hndl_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

  return thread;
}

void Thread::join() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // Should we check that this returns `WAIT_OBJECT_0`?
  ::WaitForSingleObject((HANDLE)native_hndl_, INFINITE);
  // Handle is closed in destructor.
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Thread::detach() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // Handle is closed in destructor.
  delete this;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Thread::yield() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (::SwitchToThread() == 0)
    // Windows didn't schedule another thread, so try to force it.
    ::Sleep(0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // foundation
} // yeti
