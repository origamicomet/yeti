//===-- yeti/foundation/readers_writer_lock.cc ----------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/readers_writer_lock.h"

#include "yeti/foundation/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace foundation {

struct ReadersWriterLock::Implementation {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  SRWLOCK srwl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
};

ReadersWriterLock *ReadersWriterLock::create() {
  Implementation *impl = new (foundation::heap()) Implementation();
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::InitializeSRWLock(&impl->srwl);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
  return (ReadersWriterLock *)impl;
}

void ReadersWriterLock::destroy() {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
  delete impl;
}

// NOTE(mtwilliams): We assume readers-writer locks are mostly acquired
// non-exclusively as that is the intended use-case.

void ReadersWriterLock::lock(bool exclusive) {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (YETI_LIKELY(!exclusive))
    ::AcquireSRWLockShared(&impl->srwl);
  else
    ::AcquireSRWLockExclusive(&impl->srwl);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void ReadersWriterLock::unlock(bool exclusive) {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (YETI_LIKELY(!exclusive))
    ::ReleaseSRWLockShared(&impl->srwl);
  else
    ::ReleaseSRWLockExclusive(&impl->srwl);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // foundation
} // yeti
