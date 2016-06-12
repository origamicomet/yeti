//===-- yeti/task.cc --------------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/task.h"

#include "yeti/task_manager.h"

namespace yeti {

Task *task::describe() {
  Task *task = task_manager::acquire_a_task();

  task->work.kind = Task::Work::NONE;
  task->permits = NULL;
  task->permission = 0;

  return task;
}

Task *task::describe(void (*kernel)(uintptr_t), uintptr_t ctx) {
  yeti_assert_debug(kernel != NULL);

  Task *task = describe();

  task->work.kind = Task::Work::CPU;
  task->work.cpu.kernel = kernel;
  task->work.cpu.ctx = ctx;

  return task;
}

void task::permits(Task *task, Task *permittee) {
  yeti_assert_debug(task != NULL);
  yeti_assert_debug(permittee != NULL);

  foundation::atomic::add(&permittee->permission, 1);

  Task::Permit *permit = task_manager::acquire_a_permit();
  permit->next = (Task::Permit *)foundation::atomic::load((const volatile void **)&task->permits);
  permit->task = permittee;

  foundation::atomic::store((volatile void **)&task->permits, (void *)permit);
}

} // yeti
