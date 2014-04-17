//===-- bitbyte/butane/task_scheduler.cc ------------------------*- C++ -*-===//
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

namespace TaskScheduler {
  //===--------------------------------------------------------------------===//

  static bool initialized_ = false;
  typedef foundation::lockfree::MpMcQueue<Task *> Schedulable;
  static Schedulable *schedulable_ = NULL;
  static foundation::Thread **worker_threads_ = NULL;
  static size_t num_worker_threads_ = 0;

  //===--------------------------------------------------------------------===//

  static void mark_for_schedule(Task *task)
  {
    bitbyte_butane_assert(debug, task != NULL);
    if (foundation::atomic::relaxed::compare_and_swap(&task->permissions, 0, 0x80000000) == 0)
      schedulable_->enqueue(task);
  }

  static void schedule(Task *task)
  {
    bitbyte_butane_assert(debug, task != NULL);

    switch (task->work.kind) {
      default:
        bitbyte_butane_assert(always, "Trying to schedule a task with an unknown kind of work!");
        break;
      case Task::Work::None:
        break;
      case Task::Work::Cpu:
        task->work.cpu.kernel(task->work.cpu.ctx);
        break;
    }

    Task::Permit *iter = task->permits;
    while (iter) {
      Task::Permit *permit = iter;
      iter = permit->next;
      bitbyte_butane_assert(debug, permit->task != NULL);
      if (foundation::atomic::relaxed::increment(&permit->task->permissions) == 0)
        mark_for_schedule(permit->task);
      foundation::heap().free((void *)permit);
    }

    foundation::heap().free((void *)task);
  }

  //===--------------------------------------------------------------------===//

  void inititalize(size_t internal_queue_sz)
  {
    bitbyte_butane_assert(debug, internal_queue_sz > 0);

    bitbyte_butane_assert(debug, !initialized_);
    bitbyte_butane_assert(debug, schedulable_ == NULL);
    bitbyte_butane_assert(debug, worker_threads_ == NULL);

    schedulable_ =
      bitbyte_butane_new(foundation::heap(), Schedulable)(internal_queue_sz);

    // TODO(mtwilliams): Don't hardcode the number of main threads.
    size_t num_worker_threads_ = foundation::system::num_logical_processors();
    if (num_worker_threads_ >= 2)
      num_worker_threads_ -= 2;
    else
      num_worker_threads_ = 0;

    worker_threads_ =
      (foundation::Thread **)
        foundation::heap().alloc(
          num_worker_threads_ * sizeof(foundation::Thread *),
          alignof(foundation::Thread *));

    for (size_t worker_thread = 0;
         worker_thread < num_worker_threads_;
         ++worker_thread)
    {
      worker_threads_[worker_thread] =
        &foundation::Thread::create([](void *) {
          while(true) {
            Task *task;
            schedulable_->dequeue(&task);
            schedule(task);
          }
        });

      worker_threads_[worker_thread]->set_affinity(1 << (2 + worker_thread));
      worker_threads_[worker_thread]->start();
    }

    initialized_ = true;
  }

  void shutdown()
  {
    bitbyte_butane_assert(debug, initialized_);
    bitbyte_butane_assert(debug, schedulable_ != NULL);
    bitbyte_butane_assert(debug, worker_threads_ != NULL);

    for (size_t worker_thread = 0;
         worker_thread < num_worker_threads_;
         ++worker_thread)
    {
      worker_threads_[worker_thread]->terminate();
    }

    foundation::heap().free((void *)worker_threads_);
    worker_threads_ = NULL;
    num_worker_threads_ = 0;

    bitbyte_butane_delete(foundation::heap(), Schedulable, schedulable_);
    schedulable_ = NULL;

    initialized_ = false;
  }

  //===--------------------------------------------------------------------===//

  void submit(Task *task)
  {
    bitbyte_butane_assert(debug, task != NULL);
    if (foundation::atomic::relaxed::compare_and_swap(&task->permissions, -1, 0x80000000) == -1)
      mark_for_schedule(task);
  }

  void do_work_while_waiting_for(foundation::Event &event)
  {
    while (event.is_not_signaled()) {
      Task *task;
      if (schedulable_->try_dequeue(&task))
        schedule(task);
    }
  }

  //===--------------------------------------------------------------------===//
}

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//
