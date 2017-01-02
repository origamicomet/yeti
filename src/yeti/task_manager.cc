//===-- yeti/task_manager.cc ------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/task_manager.h"

namespace yeti {

namespace task_manager {
  namespace {
    static u8 task_pool_mem_[256 * (sizeof(Task) + 8 /* overhead */)] = { 0, };
    static foundation::thread_safe::Pool<Task> task_pool_((uintptr_t)&task_pool_mem_[0], sizeof(task_pool_mem_));
    static u8 permit_pool_mem_[256 * (sizeof(Task) + 8 /* overhead */)] = { 0, };
    static foundation::thread_safe::Pool<Task::Permit> permit_pool_((uintptr_t)&permit_pool_mem_[0], sizeof(permit_pool_mem_));
  }
}

Task *task_manager::acquire_a_task() {
  Task *task = task_pool_.acquire();
  yeti_assert_development(task != NULL);
  return task;
}

void task_manager::relinquish_a_task(Task *task) {
  task_pool_.relinquish(task);
}

Task::Permit *task_manager::acquire_a_permit() {
  Task::Permit *permit = permit_pool_.acquire();
  yeti_assert_development(permit != NULL);
  return permit;
}

void task_manager::relinquish_a_permit(Task::Permit *permit) {
  permit_pool_.relinquish(permit);
}

} // yeti
