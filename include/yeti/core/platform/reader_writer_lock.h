//===-- yeti/core/platform/reader_writer_lock.h ---------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Lightweight reader-writer lock primitive.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_READER_WRITER_LOCK_H_
#define _YETI_CORE_PLATFORM_READER_WRITER_LOCK_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// \brief A lightweight lock that permits non-exclusive acquisitions and
/// exclusive acquisition operations.
///
/// \details A reader-writer lock is intended for use when you have many
/// threads reading state, but only one thread mutating state non-atomically.
///
class YETI_PUBLIC ReaderWriterLock {
 YETI_DISALLOW_COPYING(ReaderWriterLock)

 public:
  ReaderWriterLock();
  ~ReaderWriterLock();

 public:
  /// \brief Acquire the lock, waiting on other threads indefinitely until it
  /// can be acquired.
  ///
  /// \param @exlusive Acquire this lock exclusively, i.e. as a writer.
  ///
  void acquire(bool exlusive);

  /// \brief Releases the lock, allowing other threads to acquire it.
  ///
  /// \param @exlusive Lock was acquired exclusively.
  ///
  void release(bool exlusive);

 private:
  struct Storage;
  Storage *storage_;
};

/// \def YETI_SCOPED_LOCK_NON_EXCLUSIVE
/// \brief Holds a reader-writer lock non-exclusively for the duration of the
/// current scope.
#define YETI_SCOPED_LOCK_NON_EXCLUSIVE(Lock) \
  const ::yeti::core::ScopedReaderWriterLock YETI_PASTE(__scoped_lock__, __LINE__)(Lock, false);

/// \def YETI_SCOPED_LOCK_EXCLUSIVE
/// \brief Holds a reader-writer lock exclusively for the duration of the
/// current scope.
#define YETI_SCOPED_LOCK_EXCLUSIVE(Lock) \
  const ::yeti::core::ScopedReaderWriterLock YETI_PASTE(__scoped_lock__, __LINE__)(Lock, true);

/// Holds a reader-writer lock for the duration of its scope.
class ScopedReaderWriterLock {
 YETI_DISALLOW_COPYING(ScopedReaderWriterLock)

 public:
  ScopedReaderWriterLock(ReaderWriterLock *lock, bool exclusive)
    : lock_(lock), exclusive_(exclusive) { lock_->acquire(exclusive_); }

  ScopedReaderWriterLock(ReaderWriterLock &lock, bool exclusive)
    : lock_(&lock), exclusive_(exclusive) { lock_->acquire(exclusive_); }

  ~ScopedReaderWriterLock() { lock_->release(exclusive_); }

 private:
  ReaderWriterLock *lock_;
  bool exclusive_;
};

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_READER_WRITER_LOCK_H_
