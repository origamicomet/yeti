//===-- yeti/task_scheduler.h -----------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_TASK_SCHEDULER_H_
#define _YETI_TASK_SCHEDULER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

namespace yeti {

// See `include/yeti/task.h` for definition.
struct Task;

namespace task_scheduler {

///
struct Config {
  ///
  size_t workers;
};

/// \brief Initializes the task-scheduler.
///
/// \note Assumes execution on main thread.
///
extern YETI_PUBLIC void initialize(const Config &config);

/// \brief Shuts down the task-scheduler.
///
/// \warning Worker threads are terminated, thus there is no guarantee that any
/// scheduled tasks won't be interuppted or that any remaining tasks will be
/// scheduled. See yeti::foundation::Thread::terminate for more information.
///
extern YETI_PUBLIC void shutdown();

/// \brief Submits a task to the task-scheduler.
///
/// \warning The task is immediately ignored if not schedulable.
///
extern YETI_PUBLIC void submit(Task *task);

/// \brief Schedules tasks while waiting for an event to be signaled.
///
extern YETI_PUBLIC void do_work_while_waiting_for(foundation::Event *event);

} // task_scheduler

} // yeti

#endif // _YETI_TASK_SCHEDULER_H_
