//===-- yeti/foundation/readers_writer_lock.h --------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_READERS_WRITER_LOCK_H_
#define _YETI_FOUNDATION_READERS_WRITER_LOCK_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

/// \brief A lightweight user-space lock that permits non-exclusive
/// acquisitions and exclusive acquisition operations.
///
/// \details A readers-writer lock is intended for use when you have many
/// threads reading state, but only one thread writing state, that produces
/// erroneous state until completed, i.e. an atomic write is not applicable.
///
class YETI_PUBLIC ReadersWriterLock {
 YETI_DISALLOW_CONSTRUCTION(ReadersWriterLock)
 YETI_DISALLOW_COPYING(ReadersWriterLock)

 public:
  /// \brief Creates a lightweight user-space readers-writer lock.
  static ReadersWriterLock *create();

  /// \brief Destroys this readers-writer lock.
  void destroy();

 public:
  /// \brief Locks this readers-writer lock.
  /// \param @exlusive Acquire this lock exclusively, i.e. as a writer.
  void lock(bool exclusive = false);

  /// \brief Unlocks this readers-writer lock.
  /// \param @exlusive This lock was acquired exclusively.
  void unlock(bool exclusive = false);

 private:
  struct Implementation;
};

/// \def YETI_SCOPED_LOCK_NON_EXCLUSIVE
/// \brief Acquires a readers-writer lock non-exclusively for the duration of the current scope.
#define YETI_SCOPED_LOCK_NON_EXCLUSIVE(_ReadersWriterLock) \
  const ::yeti::foundation::ReadersWritersScopedLock YETI_PASTE(__scoped_lock__, __LINE__)(_ReadersWriterLock, false);

/// \def YETI_SCOPED_LOCK_EXCLUSIVE
/// \brief Acquires a readers-writer lock exclusively for the duration of the current scope.
#define YETI_SCOPED_LOCK_EXCLUSIVE(_ReadersWriterLock) \
  const ::yeti::foundation::ReadersWritersScopedLock YETI_PASTE(__scoped_lock__, __LINE__)(_ReadersWriterLock, true);

/// Acquires a readers-writer lock for the duration of its scope.
class ReadersWritersScopedLock {
 YETI_DISALLOW_COPYING(ReadersWritersScopedLock)

 public:
  ReadersWritersScopedLock(ReadersWriterLock *ReadersWriterLock, bool exclusively)
    : m(ReadersWriterLock), exclusively_(exclusively) {
    m->lock(exclusively);
  }

  ~ReadersWritersScopedLock() {
    m->unlock(exclusively_);
  }

 private:
  ReadersWriterLock *m;
  bool exclusively_;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_READERS_WRITER_LOCK_H_
