//===-- yeti/task.h ---------------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_TASK_H_
#define _YETI_TASK_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

/// \brief Describes a schedulable unit of work and its dependencies.
struct YETI_PUBLIC Task {
  /// \brief Specifies the work to do when a task is scheduled.
  struct Work {
    enum Kind {
      NONE = 0,
      CPU  = 1
    };

    Kind kind;

    union {
      struct {
        void (*kernel)(uintptr_t ctx);
        uintptr_t ctx;
      } cpu;
    };
  };

  /// \brief Specifies a task that is permitted to run by another task.
  ///
  /// \details Permits are a simplified implementation of reverse dependencies,
  /// as described by Charles Bloom in his blog posts.
  ///
  /// By using permits the following is achieved:
  ///
  ///  (1) Simpler code--scheduling and permitting code is substantially
  ///      smaller and simpler than forward dependencies.
  ///
  ///  (2) Guaranteed scheduling without overhead--by maintaining references to
  ///      any non-queued tasks in a "permit" any submitted tasks are
  ///      guaranteed to be scheduled without an additional lock-free
  ///      data-structure; either by being inserted into a woker's queue
  ///      immediately, or by being inserted later upon the completion of the
  ///      final permitting task.
  ///
  ///  (3) Improved scheduling characteristics--reverse dependencies allow
  ///      simple adherence to Charles Bloom's guiding principles:
  ///
  ///        1. Always yield worker threads when they cannot schedule work.
  ///        2. Never have a worker thread sleep when it can schedule work.
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

  /// \brief Number of outstanding permits.
  typedef u32 Permission;

  Task::Work work;
  Task::Permit *permits;
  Task::Permission permission;
};

namespace task {
  /// \brief Creates a task with no work and no outstanding permissions.
  extern YETI_PUBLIC Task *describe();

  /// \brief Creates a task with work but no outstanding permissions.
  extern YETI_PUBLIC Task *describe(void (*kernel)(uintptr_t), uintptr_t ctx = NULL);

  /// \briefs Prevents @permittee from being scheduled until @task has completed.
  extern YETI_PUBLIC void permits(Task *task, Task *permittee);
} // task

} // yeti

#endif // _YETI_TASK_H_
