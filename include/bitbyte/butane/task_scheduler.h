//===-- bitbyte/butane/task_scheduler.h -------------------------*- C++ -*-===//
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
/// \brief Defines the task-scheduler.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_TASK_SCHEDULER_H_
#define _BITBYTE_BUTANE_TASK_SCHEDULER_H_

//===----------------------------------------------------------------------===//

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>

//===----------------------------------------------------------------------===//

#include <bitbyte/butane/task.h>

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

/// \brief The task-scheduler.
///
namespace TaskScheduler {
  //===--------------------------------------------------------------------===//

  /// \brief Initializes the task-scheduler.
  /// \param internal_queue_sz Specifies the size of the internal lock-free
  /// multi-producer multi-consumer queue used by the task-scheduler to store
  /// schedulable tasks (tasks with no outstanding permits).
  ///
  extern BITBYTE_BUTANE_EXPORT void inititalize(size_t internal_queue_sz);

  /// \brief Shutsdown the task-scheduler.
  /// \warning Worker threads are terminated, thus there is no guarantee that
  /// any remaining tasks will be scheduled.
  ///
  extern BITBYTE_BUTANE_EXPORT void shutdown();

  //===--------------------------------------------------------------------===//

  /// \brief Submits a task to the task-scheduler.
  ///
  extern BITBYTE_BUTANE_EXPORT void submit(Task *task);

  /// \brief Schedules tasks while waiting for an event to be signaled.
  ///
  extern BITBYTE_BUTANE_EXPORT void do_work_while_waiting_for(foundation::Event &event);

  //===--------------------------------------------------------------------===//
}

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // _BITBYTE_BUTANE_TASK_SCHEDULER_H_

//===----------------------------------------------------------------------===//
