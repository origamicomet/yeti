//===-- yeti/foundation/thread.h --------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_THREAD_H_
#define _YETI_FOUNDATION_THREAD_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"

namespace yeti {
namespace foundation {

/// A system-level thread.
class YETI_PUBLIC Thread {
 YETI_DISALLOW_COPYING(Thread);

 public:
  typedef char Name[256];

  typedef void (*EntryPoint)(uintptr_t);

  struct Options {
    /// A name to associate with the new thread.
    /// \note The semantics of this differs per platform. Generally, threads
    /// are nameless objects. Thus, this only makes sense in the context of a
    /// debugger or similar tools.
    Name name;

    /// A bitmask of all the cores the new thread can be scheduled on.
    u64 affinity;

    /// The inital size (in bytes) of the stack to provide the new thread.
    /// \note If zero, a reasonable default is chosen. This is platform dependent.
    /// \note This may be rounded up, i.e. this is the minimum initial size.
    u32 stack_size;
  };

 private:
  Thread();
  ~Thread();

 public:
  /// \brief Creates and starts a system-level thread.
  static Thread *spawn(Thread::EntryPoint entry_point,
                       uintptr_t entry_point_arg);
  static Thread *spawn(Thread::EntryPoint entry_point,
                       uintptr_t entry_point_arg,
                       const Thread::Options *options);

  /// \brief Waits until this thread exits.
  void join();

  /// \brief Lets this thread execute freely. Any allocated resources will be
  /// free'd after it exits.
  void detach();

  /// \brief Switch execution to another thread.
  static void yield();

 public:
  // TODO(mtwilliams): Document this interface.
  uintptr_t to_native_hndl() const;

 private:
  uintptr_t native_hndl_;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_THREAD_H_
