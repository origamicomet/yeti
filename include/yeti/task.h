//===-- yeti/task.h -------------------------------------*- mode: C++11 -*-===//
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

#include "yeti/core.h"

#include "loom.h"

namespace yeti {

/// \brief Describes a schedulable unit of work and its dependencies.
struct Task : public ::loom_task_t {
  /// \brief Code comprising a task.
  typedef ::loom_kernel_fn Kernel;

  /// \brief Opaque reference to a task.
  typedef ::loom_handle_t Handle;
};

namespace task {

/// \brief Creates a task with given work.
extern YETI_PUBLIC Task::Handle describe(Task::Kernel kernel,
                                         void *data = NULL);

/// \brief Prevents @permittee from being scheduled until @task has completed.
extern YETI_PUBLIC void permits(Task::Handle task,
                                Task::Handle permittee);

} // task

} // yeti

#endif // _YETI_TASK_H_
