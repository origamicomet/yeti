//===-- yeti/foundation/mutex.cc --------------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/mutex.h"

#include "yeti/foundation/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace foundation {

struct Mutex::Implementation {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  CRITICAL_SECTION cs;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
};

Mutex *Mutex::create(u32 spin) {
  Implementation *impl = new (foundation::heap()) Implementation();
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::InitializeCriticalSectionAndSpinCount(&impl->cs, spin);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
  return (Mutex *)impl;
}

void Mutex::destroy() {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::DeleteCriticalSection(&impl->cs);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
  delete impl;
}

void Mutex::lock() {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::EnterCriticalSection(&impl->cs);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool Mutex::try_lock() {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (::TryEnterCriticalSection(&impl->cs) != 0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void Mutex::unlock() {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::LeaveCriticalSection(&impl->cs);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // foundation
} // yeti
