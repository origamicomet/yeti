//===-- bitbyte/butane/task.h -----------------------------------*- C++ -*-===//
//
//  Butane, a data-driven game engine.
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mwilliams@bitbyte.ca>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines task and task-related types.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_TASK_H_
#define _BITBYTE_BUTANE_TASK_H_

//===----------------------------------------------------------------------===//

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

/// \brief Describes a schedulable unit of work and its hierarchical and
/// dependent relationships (via permits.)
///
struct BITBYTE_BUTANE_EXPORT Task {
  //===--------------------------------------------------------------------===//

  /// \brief Specifies the work to do when a task is executed.
  ///
  struct Work {
    /// \brief Specifies the kind of work to do.
    ///
    enum Kind {
      None = 0,
      Cpu  = 1
    };

    Work::Kind kind;

    union {
      struct {
        void (*kernel)(void *ctx);
        void *ctx;
      } cpu;
    };
  };

  /// \brief Specifies a task that is permitted to run by another task.
  ///
  /// \details Permits are a simplified implementation of reverse dependencies,
  /// as described by Charles Bloom in his blog posts on cbloomrants.blogspot.ca.
  ///
  /// By using permits the following is achieved:
  ///
  ///  (1) Simpler code--scheduling and permitting code is substantially smaller
  ///      and simpler than forward dependencies.
  ///
  ///  (2) Guaranteed scheduling without overhead--by maintaining references to
  ///      any non-queued tasks in butane_task_permit_t any submitted tasks are
  ///      guaranteed to be scheduled without an additional lock-free
  ///      data-structure; either by being inserted into the schedulable queue
  ///      immediately or by being inserted later upon the completion of the
  ///      final permitting task.
  ///
  ///  (3) Improved scheduling characteristics--reverse dependencies allow simple
  ///      adherence to Charle Bloom's guiding principles:
  ///
  ///        1. Always yield worker threads when they cannot schedule work.
  ///        2. Never have a yielded worker thread when it can schedule work.
  ///        3. Never wake a worker thread only to yield it immediately.
  ///
  ///  (4) Reduced memory footprint--forward dependencies requires more memory
  ///      due to additional members in task descriptions and the use of empty
  ///      parent tasks to collect dependencies for tasks with more than one
  ///      dependency.
  ///
  struct Permit {
    Permit *next;
    Task *task;
  };

  /// \brief Count of outstanding permits.
  ///
  typedef foundation::atomic::s32 Permissions;

  //===--------------------------------------------------------------------===//

  Task::Work work;
  Task::Permit *permits;
  Task::Permissions permissions;

  //===--------------------------------------------------------------------===//

  /// \brief Creates a task with no work and no outstanding permissions that
  /// can only be scheduled after being explictly submitted.
  ///
  static Task *describe();

  /// \brief Specifies a parent task that this task is a child of.
  ///
  Task *parent(Task *parent);

  /// \brief Specifies a dependency that this task depends on.
  ///
  Task *dependency(Task *dependency);

  //===--------------------------------------------------------------------===//

  /// \brief Submits this task to the task scheduler.
  /// \see bitbyte::butane::TaskScheduler::submit
  ///
  void submit();

  //===--------------------------------------------------------------------===//
};

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // _BITBYTE_BUTANE_TASK_H_

//===----------------------------------------------------------------------===//
