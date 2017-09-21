//===-- yeti/core/platform/lock.cc ----------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/lock.h"

// TODO(mtwilliams): Allocate from a pool.
#include "yeti/core/allocators/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace core {

struct Lock::Storage {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  CRITICAL_SECTION cs;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
};

Lock::Lock() {
  storage_ =
    (Lock::Storage *)global_heap_allocator().allocate(sizeof(Lock::Storage),
                                                      alignof(Lock::Storage));

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::InitializeCriticalSection(&storage_->cs);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

Lock::~Lock() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::DeleteCriticalSection(&storage_->cs);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

  core::global_heap_allocator().deallocate((void *)storage_);
}

void Lock::acquire() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::EnterCriticalSection(&storage_->cs);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Lock::release() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::LeaveCriticalSection(&storage_->cs);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // core
} // yeti
