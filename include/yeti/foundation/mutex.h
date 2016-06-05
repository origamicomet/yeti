//===-- yeti/foundation/mutex.h ---------------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_MUTEX_H_
#define _YETI_FOUNDATION_MUTEX_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

/// A lightweight user-space mutex.
class YETI_PUBLIC Mutex {
 YETI_DISALLOW_CONSTRUCTION(Mutex);
 YETI_DISALLOW_COPYING(Mutex);

 public:
  /// \brief Creates a lightweight user-space mutex.
  static Mutex *create(u32 spin = 0);

  /// \brief Destroys this mutex.
  void destroy();

 public:
  /// \brief Locks this mutex, waiting on other threads indefinitely.
  void lock();

  /// \brief Attmpts to lock this mutex.
  bool try_lock();

  /// \brief Unlocks this mutex.
  void unlock();

 private:
  struct Implementation;
};

/// \def YETI_SCOPED_LOCK
/// \brief Acquires a lock for the duration of the current scope.
#define YETI_SCOPED_LOCK(_Mutex) \
  const ::yeti::foundation::ScopedLock YETI_PASTE(__scoped_lock__, __LINE__)(_Mutex);

/// Acquires a lock for the duration of its scope.
class ScopedLock {
 YETI_DISALLOW_COPYING(ScopedLock);

 public:
  ScopedLock(Mutex *mutex) : m(mutex) { m->lock(); }
  ~ScopedLock() { m->unlock(); }

 private:
  Mutex *m;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_MUTEX_H_
