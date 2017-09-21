//===-- yeti/core/platform/reader_writer_lock.cc --------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/reader_writer_lock.h"

// TODO(mtwilliams): Allocate from a pool.
#include "yeti/core/allocators/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace core {

struct ReaderWriterLock::Storage {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  SRWLOCK srwl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
};

ReaderWriterLock::ReaderWriterLock() {
  storage_ =
    (ReaderWriterLock::Storage *)global_heap_allocator().allocate(sizeof(ReaderWriterLock::Storage),
                                                                  alignof(ReaderWriterLock::Storage));

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::InitializeSRWLock(&storage_->srwl);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

ReaderWriterLock::~ReaderWriterLock() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

  core::global_heap_allocator().deallocate((void *)storage_);
}

void ReaderWriterLock::acquire(bool exclusive) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (YETI_LIKELY(!exclusive))
    ::AcquireSRWLockShared(&storage_->srwl);
  else
    ::AcquireSRWLockExclusive(&storage_->srwl);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void ReaderWriterLock::release(bool exclusive) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (YETI_LIKELY(!exclusive))
    ::ReleaseSRWLockShared(&storage_->srwl);
  else
    ::ReleaseSRWLockExclusive(&storage_->srwl);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // core
} // yeti
