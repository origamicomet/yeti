/* ========================================================================== */
/*                                                                            */
/* This file is part of Butane.                                               */
/*                                                                            */
/* Author(s):                                                                 */
/*                                                                            */
/*   Michael Williams <devbug@bitbyte.ca>                                     */
/*                                                                            */
/* See LICENSE.md for licensing information.                                  */
/*                                                                            */
/* ========================================================================== */
 #  include <butane/task_scheduler.h>
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_task_sched_xxx):                                                */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

static fnd_atomic_uint32_t scheduling_ = UINT32_C(0);
static size_t num_of_worker_threads_ = 0;
static fnd_thread_t **worker_threads_ = NULL;
static butane_task_t *queue_ = NULL;
static fnd_mutex_t *queue_lock_ = NULL;
static size_t queue_read_ = 0;
static size_t queue_write_ = 0;

/* ========================================================================== */
/*  Worker Thread:                                                            */
/* ========================================================================== */

static void butane_task_sched_worker_thread(uintptr_t id) {
  const butane_task_affinity_t affinity =
    (butane_task_affinity_t)(((unsigned)1 << (unsigned)id));
  while (true) {
    fnd_mutex_lock(queue_lock_);
    if (!fnd_atomic_uint32_load_relaxed(&scheduling_)) {
      if (queue_read_ == queue_write_) {
        fnd_mutex_unlock(queue_lock_);
        break; }}
    butane_task_t *consumable = NULL;
    for (size_t read = queue_read_; read < queue_write_; ++read) {
      butane_task_t *task = &queue_[read % BT_TASK_SCHED_QUEUE_SZ];
      if (!(affinity & task->affinity_))
        continue;
      if (task->refs_by_children_and_self_ > 1)
        continue;
      if (task->refs_by_children_and_self_ == 0) {
        if (task->refs_by_dependencies_ == 0)
          if (read == queue_read_)
            ++queue_read_;
        continue; }
      if (task->dependency_ != butane_task_invalid())
        if (queue_[task->dependency_].refs_by_children_and_self_ > 1)
          continue;
      consumable = task;
      consumable->refs_by_children_and_self_ = UINT32_C(0xFFFFFFFF);
      break; }
    fnd_mutex_unlock(queue_lock_);
    /*! TODO(mtwilliams): Yield. */
    if (!consumable)
      continue;
    butane_task_run(consumable);
    if (consumable->increment_on_completion_)
      fnd_atomic_uint32_fetch_increment_relaxed(consumable->increment_on_completion_);
    fnd_mutex_lock(queue_lock_);
    consumable->refs_by_children_and_self_ = UINT32_C(0);
    if (consumable->parent_ != butane_task_invalid())
      queue_[consumable->parent_].refs_by_children_and_self_--;
    if (consumable->dependency_ != butane_task_invalid())
      queue_[consumable->dependency_].refs_by_dependencies_--;
    fnd_mutex_unlock(queue_lock_);
  }
}

/* ========================================================================== */
/*  Initialize & Shutdown:                                                    */
/* ========================================================================== */

static size_t butane_task_sched_derive_num_of_worker_threads() {
  const size_t num_of_cores = fnd_sys_num_of_cores();
  if (num_of_cores >= 8)
    return 7;
  else if (num_of_cores > 1)
    return num_of_cores - 1;
  return 1;
}

void butane_task_sched_initialize(void) {
  scheduling_ = UINT32_C(0xFFFFFFFF);
  num_of_worker_threads_ = butane_task_sched_derive_num_of_worker_threads();
  worker_threads_ = (fnd_thread_t **)
    fnd_allocator_alloc(
      fnd_heap(),
      num_of_worker_threads_ * sizeof(fnd_thread_t *),
      fnd_alignof(fnd_thread_t *));
  queue_ = (butane_task_t *)
    fnd_allocator_alloc(
      fnd_heap(),
      BT_TASK_SCHED_QUEUE_SZ * sizeof(butane_task_t),
      fnd_alignof(butane_task_t));
  queue_lock_ = fnd_mutex_create();
  queue_read_ = queue_write_ = 0;
  for (size_t i = 0; i < num_of_worker_threads_; ++i) {
    worker_threads_[i] = fnd_thread_create(&butane_task_sched_worker_thread, (uintptr_t)i);
    const fnd_thread_affinity_t affinity =
      ((fnd_thread_affinity_t)((unsigned)1 << (i + 1)));
    fnd_thread_set_affinity(worker_threads_[i], affinity);
    fnd_thread_start(worker_threads_[i]);
  }
}

void butane_task_sched_shutdown(void) {
  fnd_atomic_uint32_store_relaxed(&scheduling_, UINT32_C(0));
  for (size_t i = 0; i < num_of_worker_threads_; ++i)
    fnd_thread_join(worker_threads_[i]);
  fnd_allocator_free(fnd_heap(), (void *)worker_threads_);
  fnd_mutex_destroy(queue_lock_);
  fnd_allocator_free(fnd_heap(), (void *)queue_);
}

/* ========================================================================== */
/*  Accessors:                                                                */
/* ========================================================================== */

size_t butane_task_sched_num_of_worker_threads(void) {
  return num_of_worker_threads_;
}

/* ========================================================================== */
/*  Methods:                                                                  */
/* ========================================================================== */

void butane_task_sched_enqueue(const size_t num_of_tasks, const butane_task_t *tasks) {
  bt_assert(debug, num_of_tasks > 0);
  bt_assert(debug, tasks != NULL);
  fnd_mutex_lock(queue_lock_);
  bt_assert(release, (BT_TASK_SCHED_QUEUE_SZ - (queue_write_ - queue_read_)) > num_of_tasks);
  for (size_t write = 0; write < num_of_tasks; ++write) {
    butane_task_t *task = &queue_[((write + queue_write_) % BT_TASK_SCHED_QUEUE_SZ)];
    memcpy((void *)task, (const void *)&tasks[write], sizeof(butane_task_t));
    task->id_ = ((task->id_ + queue_write_) % BT_TASK_SCHED_QUEUE_SZ);
    if (task->parent_ != butane_task_invalid())
      task->parent_ = ((task->parent_ + queue_write_) % BT_TASK_SCHED_QUEUE_SZ);
    if (task->dependency_ != butane_task_invalid())
      task->dependency_ = ((task->dependency_ + queue_write_) % BT_TASK_SCHED_QUEUE_SZ); }
  queue_write_ += num_of_tasks;
  fnd_mutex_unlock(queue_lock_);
}

void butane_task_sched_do_work(void) {
  /* TODO(mtwilliams): Remove code dupliation (refactor worker threads)? */
  fnd_mutex_lock(queue_lock_);
  butane_task_t *consumable = NULL;
  for (size_t read = queue_read_; read < queue_write_; ++read) {
    butane_task_t *task = &queue_[read % BT_TASK_SCHED_QUEUE_SZ];
    if (task->affinity_ != butane_task_any())
      continue;
    if (task->refs_by_children_and_self_ > 1)
      continue;
    if (task->refs_by_children_and_self_ == 0) {
      if (task->refs_by_dependencies_ == 0)
        if (read == queue_read_)
          ++queue_read_;
      continue; }
    if (task->dependency_ != butane_task_invalid())
      if (queue_[task->dependency_].refs_by_children_and_self_ > 1)
        continue;
    consumable = task;
    consumable->refs_by_children_and_self_ = UINT32_C(0xFFFFFFFF);
    break; }
  fnd_mutex_unlock(queue_lock_);
  if (!consumable)
    return;
  butane_task_run(consumable);
  if (consumable->increment_on_completion_)
    fnd_atomic_uint32_fetch_increment_relaxed(consumable->increment_on_completion_);
  fnd_mutex_lock(queue_lock_);
  consumable->refs_by_children_and_self_ = UINT32_C(0);
  if (consumable->parent_ != butane_task_invalid())
    queue_[consumable->parent_].refs_by_children_and_self_--;
  if (consumable->dependency_ != butane_task_invalid())
    queue_[consumable->dependency_].refs_by_dependencies_--;
  fnd_mutex_unlock(queue_lock_);
}

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::TaskScheduler::*):                                           */
/* ========================================================================== */

/* ... */
