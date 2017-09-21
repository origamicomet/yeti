//===-- yeti/task_scheduler.cc --------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/task_scheduler.h"

namespace yeti {

namespace task_scheduler {
  namespace {
    // TODO(mtwilliams): Statistics.
    static void prologue(Task *task, void *) {}
    static void epilogue(Task *task, void *) {}
  }
}

void task_scheduler::initialize(const task_scheduler::Config &config) {
  ::loom_options_t options;

  options.workers = config.workers;

  // Only true of our runtime.
  options.main_thread_does_work = false;

  options.prologue.fn = (::loom_prologue_fn)&prologue;
  options.prologue.context = NULL;

  options.epilogue.fn = (::loom_epilogue_fn)&epilogue;
  options.epilogue.context = NULL;

  // TODO(mtwilliams): Determine reasonable defaults.
  options.tasks   = 4096;
  options.permits = 4096;
  options.queue   = 4096;

  ::loom_initialize(&options);
}

void task_scheduler::shutdown() {
  ::loom_shutdown();
}

void task_scheduler::kick(Task::Handle task) {
  ::loom_kick(task);
}

void task_scheduler::kick_n(unsigned n, const Task::Handle *tasks) {
  ::loom_kick_n(n, tasks);
}

void task_scheduler::kick_and_wait(Task::Handle task) {
  ::loom_kick_and_wait(task);
}

void task_scheduler::kick_and_wait_n(unsigned n, const Task::Handle *tasks) {
  ::loom_kick_and_wait_n(n, tasks);
}

void task_scheduler::kick_and_do_work_while_waiting(Task::Handle task) {
  ::loom_kick_and_do_work_while_waiting(task);
}

void task_scheduler::kick_and_do_work_while_waiting_n(unsigned n, const Task::Handle *tasks) {
  ::loom_kick_and_do_work_while_waiting_n(n, tasks);
}

static bool is_desired_yet(volatile u32 *v, const u32 desired) {
  return (atomic::load(v) == desired)
      && (atomic::cmp_and_xchg(v, desired, desired) == desired);
}

bool task_scheduler::do_some_work() {
  return ::loom_do_some_work();
}

void task_scheduler::do_some_work_until_zero(volatile u32 *v) {
  while (!is_desired_yet(v, 0))
    if (!do_some_work())
      core::Thread::yield();
}

void task_scheduler::do_some_work_until_equal(volatile u32 *v, u32 desired) {
  while (!is_desired_yet(v, desired))
    if (!do_some_work())
      core::Thread::yield();
}

} // yeti
