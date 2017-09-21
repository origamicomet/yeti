//===-- yeti/task_scheduler.h ---------------------------*- mode: C++11 -*-===//
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

#include "yeti/core.h"

#include "yeti/task.h"

namespace yeti {

namespace task_scheduler {

struct Config {
  /// Number of worker threads to spawn.
  ///
  /// \note Setting this to `-n` will result in a worker thread being spawned
  ///       for each core minus `n`, with a maximum of 31 on 32-bit and 63 on
  ///       64-bit platforms.
  ///
  i32 workers;
};

/// \brief Initializes the task scheduler.
///
/// \note Assumes execution on main thread.
///
extern YETI_PUBLIC void initialize(const Config &config);

/// \brief Shuts down the task scheduler.
///
/// \warning All existing tasks are scheduled prior to returning. As long as a
///          task kicks another task, this will not return.
///
/// \note Assumes execution on main thread.
///
extern YETI_PUBLIC void shutdown();

/// \brief Kicks a task and does not wait for it to be completed.
extern YETI_PUBLIC void kick(Task::Handle task);

/// \brief Kicks all tasks and does not wait for any to be completed.
extern YETI_PUBLIC void kick_n(unsigned n, const Task::Handle *tasks);

/// \brief Kicks a task and wait for it to be completed.
extern YETI_PUBLIC void kick_and_wait(Task::Handle task);

/// \brief Kicks all tasks and waits for all to be completed.
extern YETI_PUBLIC void kick_and_wait_n(unsigned n, const Task::Handle *tasks);

/// \brief Kicks a task and does work while waiting for it to be completed.
extern YETI_PUBLIC void kick_and_do_work_while_waiting(Task::Handle task);

/// \brief Kicks all tasks and does work while waiting for all to be completed.
extern YETI_PUBLIC void kick_and_do_work_while_waiting_n(unsigned n, const Task::Handle *tasks);

/// \brief Schedules an available task, if there are any.
/// \warning You should only call this from the main thread!
/// \returns If some work was performed.
extern YETI_PUBLIC bool do_some_work();

/// \brief Schedules available tasks, if there are any, until @v is zero.
/// \warning You should only call this from the main thread!
extern YETI_PUBLIC void do_some_work_until_zero(volatile u32 *v);

/// \brief Schedules available tasks, if there are any, until @v is @desired.
/// \warning You should only call this from the main thread!
extern YETI_PUBLIC void do_some_work_until_equal(volatile u32 *v, u32 desired);

} // task_scheduler

} // yeti

#endif // _YETI_TASK_SCHEDULER_H_
