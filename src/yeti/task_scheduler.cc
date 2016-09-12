//===-- yeti/task_scheduler.cc ----------------------------*- mode: C++ -*-===//
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

#include "yeti/task.h"
#include "yeti/task_manager.h"

namespace yeti {

namespace task_scheduler {
  namespace {
    // NOTE(mtwilliams): We support up to 63 worker threads.
    static size_t num_workers_ = 0;
    static foundation::Thread *worker_threads_[63];

    typedef foundation::thread_safe::SpMcDeque<Task *> WorkQueue;
    static WorkQueue *work_queues_[64];

    // OPTIMIZE(mtwilliams): We dedicate 1MiB to worker queues. This may be
    // excessive. We should determine a reasonable upper limit.
    static u8 worker_queue_mem_[1048576];
  }
}

namespace task_scheduler {
  namespace {
    // We use a thread-local pointer to track the appropriate work queue. This
    // makes handling submissions much, much easier.
    static YETI_THREAD_LOCAL WorkQueue *Q = NULL;
  }

  // Seperate functions for resue by |do_work_while_waiting_for|.
  static Task *grab();
  static void schedule(Task * task);

  static void worker_thread(const uintptr_t worker) {
    Q = work_queues_[worker + 1];

    while (true)
      if (Task *task = grab())
        schedule(task);
  }

  static Task *grab() {
    if (Task *task = Q->pop()) {
      // We got some work from our queue.
      return task;
    } else {
      // This worker's queue is empty. Try to steal some work from another
      // worker.
      WorkQueue *const victim = work_queues_[foundation::prng<size_t>() % num_workers_];

      if (victim == Q)
        // Don't steal from ourself.
        return NULL;

      if (Task *task = victim->steal())
        return task;

      // TODO(mtwilliams): Retry a few times then sleep?
    }

    // No work available, or failed to steal some.
    return NULL;
  }

  static void schedule(Task *task) {
    // Relinquish soon as possible. Definitely helps when |task| queues another
    // task, or our task queue is nearly exhausted. Theoretically reduces the
    // possibility of false-sharing.
    Task local_copy_of_task;
    memcpy((void *)&local_copy_of_task, (const void *)task, sizeof(Task));
    task_manager::relinquish_a_task(task);

    switch (local_copy_of_task.work.kind) {
      case Task::Work::NONE:
        // Do absolutely nothing.
        break;

      case Task::Work::CPU:
        yeti_assert_debug(local_copy_of_task.work.cpu.kernel != NULL);
        local_copy_of_task.work.cpu.kernel(local_copy_of_task.work.cpu.ctx);
        break;
    }

    // REFACTOR(mtwilliams): Move into seperate function?
    Task::Permit *permit = local_copy_of_task.permits;
    while (permit) {
      if (foundation::atomic::sub(&permit->task->permission, 1) == 1)
        // Submit to this worker's queue.
        Q->push(task);

      // SMELL(mtwilliams): Use double indirection.
      Task::Permit *const next = permit->next;
      task_manager::relinquish_a_permit(permit);
      permit = next;
    }
  }
}

void task_scheduler::initialize() {
  // TODO(mtwilliams): Yank via `cpuid`.
  num_workers_ = 8 - 1;

  const size_t mem_per_worker_queue =
    sizeof(worker_queue_mem_) /  (num_workers_ + 1);

  // We reserve the first work queue for the main thread, essentially
  // treating the main thread as a worker.
  work_queues_[0] =
    new (foundation::heap()) WorkQueue(
      (uintptr_t)&worker_queue_mem_[0],
      mem_per_worker_queue);

  // NOTE(mtwilliams): Assume the main thread called us.
  Q = work_queues_[0];

  for (size_t worker = 0; worker < num_workers_; ++worker) {
    work_queues_[worker + 1] =
      new (foundation::heap()) WorkQueue(
        (uintptr_t)&worker_queue_mem_[(worker + 1) * mem_per_worker_queue],
        mem_per_worker_queue);

    foundation::Thread::Options worker_thread_opts;
    sprintf(&worker_thread_opts.name[0], "Worker #%02u", worker + 1);
    worker_thread_opts.affinity = (1ull << worker);

    worker_threads_[worker] =
      foundation::Thread::spawn(&worker_thread, (uintptr_t)worker, &worker_thread_opts);
  }
}

void task_scheduler::shutdown() {
  // NOTE(mtwilliams): Since we terminate the worker threads non-gracefully,
  // they may still hold exclusive resources, e.g. locks. This can cause a
  // deadlock if not accounted for.

  // TODO(mtwilliams): Gracefully terminate to prevent the aforementioned?
  for (size_t worker = 0; worker < num_workers_; ++worker)
    worker_threads_[worker]->terminate();

  // NOTE(mtwilliams): We don't really care about the minor memory-leak this
  // causes because the task scheduler is only shutdown prior to exit.

  // for (size_t worker = 0; worker < num_workers_; ++worker)
  //   delete work_queues_[worker + 1];
  // delete work_queues_[0];
}

void task_scheduler::submit(Task *task) {
  yeti_assert_debug(task != NULL);

  if (foundation::atomic::load(&task->permission) != 0)
    // Can't schedule. Should be picked up later.
    return;

  // Tasks should never be submitted outside of the main or worker threads.
  yeti_assert_debug(Q != NULL);

  Q->push(task);
}

#if 0

void task_scheduler::do_work_while_waiting_for(foundation::Event &event) {
  while (!event.signalled())
    if (Task *task = grab())
      schedule(task);
}

#endif

} // yeti
