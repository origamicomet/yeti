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

/* ========================================================================== */
/*  Worker Thread:                                                            */
/* ========================================================================== */

static void butane_task_sched_worker_thread(uintptr_t id) {
  const butane_task_affinity_t affinity =
    (butane_task_affinity_t)(((unsigned)1 << (unsigned)id));
  while (fnd_atomic_uint32_load_relaxed(&scheduling_)) {
    /* TODO(mtwilliams): Consume consumable tasks. */
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
  /* TODO(mtwilliams): Enqueue and patch tasks. */
}

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::TaskScheduler::*):                                           */
/* ========================================================================== */

/* ... */
