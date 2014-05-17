//===-- bitbyte/butane/task.cc ----------------------------------*- C++ -*-===//
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

#include <bitbyte/butane/task.h>
#include <bitbyte/butane/task_scheduler.h>

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//

Task *Task::describe()
{
  // TODO(mtwilliams): Reuse tasks via an object pool.
  Task *task = (Task *)foundation::heap().alloc(sizeof(Task), alignof(Task));

  memset((void *)task, 0, sizeof(Task));
  task->work.kind = Task::Work::None;
  task->permits = NULL;

  // To prevent execution until explictly submitted task->permissions is
  // initially set to -1.
  foundation::atomic::relaxed::store(&task->permissions, -1);

  return task;
}

Task *Task::parent(Task *parent)
{
  bitbyte_butane_assert(debug, parent != NULL);

  // TODO(mtwilliams): Reuse task permits via an object pool.
  Task::Permit *permit =
    (Task::Permit *)foundation::heap().alloc(
      sizeof(Task::Permit),
      alignof(Task::Permit));

  memset((void *)permit, 0, sizeof(Task::Permit));
  permit->next = this->permits;
  permit->task = parent;
  this->permits = permit;

  return this;
}

Task *Task::dependency(Task *dependency)
{
  bitbyte_butane_assert(debug, dependency != NULL);

  // TODO(mtwilliams): Reuse task permits via an object pool.
  Task::Permit *permit =
    (Task::Permit *)foundation::heap().alloc(
      sizeof(Task::Permit),
      alignof(Task::Permit));

  memset((void *)permit, 0, sizeof(Task::Permit));
  permit->next = dependency->permits;
  permit->task = this;
  dependency->permits = permit;

  return this;
}

//===----------------------------------------------------------------------===//

void Task::submit()
{
  TaskScheduler::submit(this);
}

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//
