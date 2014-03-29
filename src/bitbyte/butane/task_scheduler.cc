//=== bitbyte/butane/task_scheduler.cc =======================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

#include <bitbyte/butane/task_scheduler.h>

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//
// Singleton
//

static TaskScheduler *task_scheduler_ = NULL;
TaskScheduler &task_scheduler() {
  return *task_scheduler_;
}

//===----------------------------------------------------------------------===//
// Constructors
//

TaskScheduler::TaskScheduler(size_t qsz)
  : schedulable_(qsz)
  , worker_threads_(NULL)
  , num_worker_threads_(0)
{
}

//===----------------------------------------------------------------------===//
// Destructor
//

TaskScheduler::~TaskScheduler()
{
}

//===----------------------------------------------------------------------===//
// TaskScheduler::initialize
//

void TaskScheduler::initialize(size_t internal_queue_sz)
{
  bitbyte_butane_assert(development, task_scheduler_ == NULL);

  task_scheduler_ =
    bitbyte_butane_new(foundation::heap(), TaskScheduler)(internal_queue_sz);

  const size_t num_logical_processors = foundation::system::num_logical_processors();

  if (num_logical_processors <= 2)
    task_scheduler_->num_worker_threads_ = 0;
  else
    task_scheduler_->num_worker_threads_ = num_logical_processors - 2;

  if (task_scheduler_->num_worker_threads_) {
    task_scheduler_->worker_threads_ =
      (foundation::Thread **)foundation::heap().alloc(
        task_scheduler_->num_worker_threads_ * sizeof(foundation::Thread *),
        alignof(foundation::Thread *));

    auto worker_thread = [](void *task_scheduler_) -> void {
      bitbyte_butane_assert(debug, task_scheduler_ != NULL);
      TaskScheduler &task_scheduler = *((TaskScheduler *)task_scheduler_);

      while (true) {
        Task *task;
        task_scheduler.schedulable_.dequeue(&task);

        task->kernel_();

        Task::Permit *iter = task->permits_;
        while (iter) {
          Task::Permit *permit = iter;
          iter = permit->next;
          bitbyte_butane_assert(debug, permit->task != NULL);
          if (foundation::atomic::relaxed::increment(&permit->task->permissions_) == 0)
            task_scheduler.schedule(permit->task);
          foundation::heap().free((void *)permit);
        }

        task->~Task();
        foundation::heap().free((void *)task);
      }
    };

    for (size_t wt = 0; wt < task_scheduler_->num_worker_threads_; ++wt) {
      task_scheduler_->worker_threads_[wt] =
        &foundation::Thread::create(worker_thread, (void *)task_scheduler_)
          .set_affinity(1 << (wt + 2));
    }
  }
}

//===----------------------------------------------------------------------===//
// TaskScheduler::shutdown
//

void TaskScheduler::shutdown()
{
  bitbyte_butane_assert(development, task_scheduler_ != NULL);

  for (size_t wt = 0; wt < task_scheduler_->num_worker_threads_; ++wt)
    task_scheduler_->worker_threads_[wt]->terminate();

  if (task_scheduler_->worker_threads_)
    foundation::heap().free((void *)task_scheduler_->worker_threads_);

  bitbyte_butane_delete(foundation::heap(), TaskScheduler, task_scheduler_);
  task_scheduler_ = NULL;
}

//===----------------------------------------------------------------------===//
// TaskScheduler::submit
//

void TaskScheduler::submit(Task &task)
{
  this->schedule(&task);
}

void TaskScheduler::do_work_while_waiting_for(foundation::Event &event)
{
  while (event.is_not_signaled())
    this->consume();
}

//===----------------------------------------------------------------------===//
// TaskScheduler::schedule
//

void TaskScheduler::schedule(Task *task)
{
  bitbyte_butane_assert(debug, task != NULL);
  if (foundation::atomic::relaxed::compare_and_swap(&task->permissions_, 0, 0xFFFFFFFF) == 0)
    schedulable_.enqueue(task);
}

//===----------------------------------------------------------------------===//
// TaskScheduler::consume
//

void TaskScheduler::consume()
{
  Task *task;
  if (!schedulable_.try_dequeue(&task))
    return;

  task->kernel_();

  Task::Permit *iter = task->permits_;
  while (iter) {
    Task::Permit *permit = iter;
    iter = permit->next;
    bitbyte_butane_assert(debug, permit->task != NULL);
    if (foundation::atomic::relaxed::increment(&permit->task->permissions_) == 0)
      this->schedule(permit->task);
    foundation::heap().free((void *)permit);
  }

  task->~Task();
  foundation::heap().free((void *)task);
}

} // butane
} // bitbyte
