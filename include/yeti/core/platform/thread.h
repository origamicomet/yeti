//===-- yeti/core/platform/thread.h ---------------------*- mode: C++11 -*-===//
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
/// \brief System threads.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_THREAD_H_
#define _YETI_CORE_PLATFORM_THREAD_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// A system-level thread.
class YETI_PUBLIC Thread {
 YETI_DISALLOW_COPYING(Thread)

 private:
  Thread();
  ~Thread();

 public:
  typedef void (*EntryPoint)(void *);

  struct Options {
    /// A name to associate with the new thread.
    ///
    /// \note The semantics of this differs per platform. Usually, threads
    /// are nameless objects. Thus this only makes sense in the context of a
    /// debugger or tooling.
    ///
    /// \warning Thread names will be silently truncated to system limit. On
    /// Linux this is only 16 characters, including null terminator!
    ///
    const char *name;

    /// A bitmask of all the logical cores the new thread can be scheduled on.
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    u32 affinity;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    u64 affinity;
  #endif

    /// The maximum size (in bytes) of the stack to provide the new thread.
    ///
    /// \note If zero, a reasonable default is chosen.
    ///
    size_t stack;
  };

 public:
  /// \brief Creates and starts a system-level thread with reasonable defaults.
  static Thread *spawn(EntryPoint entry_point,
                       void *entry_point_argument);

  /// \brief Creates and starts a system-level thread with @options.
  static Thread *spawn(EntryPoint entry_point,
                       void *entry_point_argument,
                       const Options &options);

  /// \brief Waits until the thread exits.
  void join();

  /// \brief Lets the thread execute freely. Any allocated resources will be
  /// deallocated after it exits.
  void detach();

  /// \brief Non-gracefully terminates the thread.
  ///
  /// \warning This thread may still hold exclusive resources, e.g. locks. This
  /// can cause a deadlock if not accounted for.
  ///
  void terminate();

  /// \brief Returns the current thread's identifier.
  static u64 id();

  /// \brief Switch execution to another thread.
  static void yield();

 private:
  uintptr_t handle_;
};

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_THREAD_H_
