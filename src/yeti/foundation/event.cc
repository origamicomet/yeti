//===-- yeti/foundation/event.cc ------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/event.h"

#include "yeti/foundation/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace foundation {

Event *Event::create(bool manual) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (Event *)::CreateEvent(NULL, manual, FALSE, NULL);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Event::destroy() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::CloseHandle((HANDLE)this);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Event::signal() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::SetEvent((HANDLE)this);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Event::unsignal() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ResetEvent((HANDLE)this);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool Event::signalled() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (::WaitForSingleObject((HANDLE)this, 0) == WAIT_OBJECT_0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Event::wait() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::WaitForSingleObject((HANDLE)this, INFINITE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // foundation
} // yeti
