//===-- yeti/core/platform/event.cc ---------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/event.h"

// TODO(mtwilliams): Allocate from a pool.
#include "yeti/core/allocators/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace core {

struct Event::Storage {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  HANDLE handle;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
};

Event::Event(bool manual) {
  storage_ =
    (Event::Storage *)global_heap_allocator().allocate(sizeof(Event::Storage),
                                                       alignof(Event::Storage));

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  storage_->handle = ::CreateEvent(NULL, manual, FALSE, NULL);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

Event::~Event() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::CloseHandle(storage_->handle);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

  core::global_heap_allocator().deallocate((void *)storage_);
}

void Event::signal() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::SetEvent(storage_->handle);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Event::unsignal() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ResetEvent(storage_->handle);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool Event::signaled() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (::WaitForSingleObject(storage_->handle, 0) == WAIT_OBJECT_0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Event::wait() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::WaitForSingleObject(storage_->handle, INFINITE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // core
} // yeti
