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
/*! @file include/butane/task_scheduler.h
     Provides a task scheduler that consumes task graphs.                     */
/* ========================================================================== */

#ifndef _BUTANE_TASK_SCHEDULER_H_
#define _BUTANE_TASK_SCHEDULER_H_

#include <butane/config.h>
#include <butane/foundation.h>
#include <butane/task.h>

/* ========================================================================== */
/*  Task:                                                                     */
/*   * C (butane_task_sched_xxx);                                             */
/*   * C++ (butane::task_sched::*).                                           */
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_task_sched_xxx):                                                */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*  Initialize & Shutdown:                                                    */
/* ========================================================================== */

/*! */
extern void butane_task_sched_initialize(void);

/*! */
extern void butane_task_sched_shutdown(void);

/* ========================================================================== */
/*  Accessors:                                                                */
/* ========================================================================== */

/*! */
extern size_t butane_task_sched_num_of_worker_threads(void);

/* ========================================================================== */
/*  Methods:                                                                  */
/* ========================================================================== */

/*! */
extern void butane_task_sched_enqueue(
  const size_t num_of_tasks,
  const butane_task_t *tasks);

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::task_sched::*):                                              */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  namespace task_sched {
    /* ====================================================================== */
    /*  Initialize & Shutdown:                                                */
    /* ====================================================================== */
    /*! @copydoc butane_task_sched_initialize */
    static void initialize() {
      butane_task_sched_initialize(); }
    /*! @copydoc butane_task_sched_shutdown */
    static void shutdown() {
      butane_task_sched_shutdown(); }

    /* ====================================================================== */
    /*  Accessors:                                                            */
    /* ====================================================================== */
    /*! @copydoc butane_task_sched_num_of_worker_threads */
    static size_t num_of_worker_threads() {
      return butane_task_sched_num_of_worker_threads(); }

    /* ====================================================================== */
    /*  Methods:                                                              */
    /* ====================================================================== */
    /*! @copydoc butane_task_sched_enqueue */
    static void enqueue(const size_t num_of_tasks, const Task *tasks) {
      butane_task_sched_enqueue(num_of_tasks, (const butane_task_t *)tasks); }
  } /* task_sched */
} /* butane */
#endif

#endif /* _BUTANE_TASK_SCHEDULER_H_ */
