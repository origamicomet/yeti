//===-- yeti/core/platform/lock.h -----------------------*- mode: C++11 -*-===//
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
/// \brief Lightweight mutual exclusion primitive.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_LOCK_H_
#define _YETI_CORE_PLATFORM_LOCK_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// A lightweight mutual exclusion primitive.
class YETI_PUBLIC Lock {
 YETI_DISALLOW_COPYING(Lock)

 public:
  Lock();
  ~Lock();

 public:
  /// \brief Acquire the lock, waiting on other threads indefinitely until it
  /// can be acquired.
  void acquire();

  /// \brief Releases the lock, allowing other threads to acquire it.
  void release();

 private:
  struct Storage;
  Storage *storage_;
};

/// \def YETI_SCOPED_LOCK
/// \brief Holds a lock for the duration of the current scope.
#define YETI_SCOPED_LOCK(Lock) \
  const ::yeti::core::ScopedLock YETI_PASTE(__scoped_lock__, __LINE__)(Lock);

/// Holds a lock for the duration of its scope.
class ScopedLock {
 YETI_DISALLOW_COPYING(ScopedLock)

 public:
  ScopedLock(Lock *lock) : lock_(lock) { lock_->acquire(); }
  ScopedLock(Lock &lock) : lock_(&lock) { lock_->acquire(); }
  ~ScopedLock() { lock_->release(); }

 private:
  Lock *lock_;
};

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_LOCK_H_
