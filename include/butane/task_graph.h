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
/*! @file include/butane/task_graph.h
     Provides `butane_task_graph_t` and `butane::TaskGraph` which wrap the
     construction of task graphs (graphs of tasks based on parental and
     dependence relationships).                                               */
/* ========================================================================== */

#ifndef _BUTANE_TASK_GRAPH_H_
#define _BUTANE_TASK_GRAPH_H_

#include <butane/config.h>
#include <butane/foundation.h>
#include <butane/task.h>

/* ========================================================================== */
/*  Task Graph:                                                               */
/*   * C (butane_task_graph_t);                                               */
/*   * "C++" (butane::TaskGraph).                                             */
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_task_graph_t):                                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/*! */
typedef struct butane_task_graph {
  size_t num_of_tasks_;
  butane_task_t *tasks_;
  size_t tasks_sz_;
  fnd_atomic_uint32_t completed_;
} butane_task_graph_t;

/* ========================================================================== */

/*! */
extern butane_task_graph_t *butane_task_graph_create(size_t sz);

/*! */
extern void butane_task_graph_destroy(butane_task_graph_t *task_graph);

/* ========================================================================== */

/*! */
extern butane_task_id_t butane_task_graph_add(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel);

/*! @copydoct butane_task_graph_add */
extern butane_task_id_t butane_task_graph_add_1(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0);

/*! @copydoct butane_task_graph_add */
extern butane_task_id_t butane_task_graph_add_2(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1);

/*! @copydoct butane_task_graph_add */
extern butane_task_id_t butane_task_graph_add_3(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2);

/*! @copydoct butane_task_graph_add */
extern butane_task_id_t butane_task_graph_add_4(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2, uintptr_t _3);

/* ========================================================================== */

/*! */
extern void butane_task_graph_kick(
  const butane_task_graph_t *task_graph);

/*! */
extern void butane_task_graph_kick_and_wait(
  const butane_task_graph_t *task_graph);

/*! */
extern void butane_task_graph_kick_and_do_work_while_waiting(
  const butane_task_graph_t *task_graph);

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::TaskGraph):                                                  */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  /*! @copydoc butane_task_graph_t */
  class TaskGraph : private butane_task_graph_t {
    private:
      TaskGraph(const TaskGraph &);
      TaskGraph &operator= (const TaskGraph &);
    private:
      TaskGraph();
      ~TaskGraph();
    public:
      /*! @copydoc butane_task_graph_create */
      static TaskGraph *create(const size_t sz) {
        return ((TaskGraph *)butane_task_graph_create(sz)); }
      /*! @copydoc butane_task_graph_destroy */
      void destroy();
    public:
      /*! @copydoc butane_task_graph_add */
      Task::Id add(
        const Task::Affinity affinity,
        const Task::Id parent,
        const Task::Id dependency,
        const Task::Kernel kernel)
      {
        return ((Task::Id)
          butane_task_graph_add(
            (butane_task_graph_t *)this,
            (butane_task_affinity_t)affinity,
            (butane_task_id_t)parent,
            (butane_task_id_t)dependency,
            (butane_task_kernel_fn)kernel)); }
      /*! @copydoc butane_task_graph_add */
      Task::Id add(
        const Task::Affinity affinity,
        const Task::Id parent,
        const Task::Id dependency,
        const Task::Kernel kernel,
        uintptr_t _0)
      {
        return ((Task::Id)
          butane_task_graph_add_1(
            (butane_task_graph_t *)this,
            (butane_task_affinity_t)affinity,
            (butane_task_id_t)parent,
            (butane_task_id_t)dependency,
            (butane_task_kernel_fn)kernel,
            _0)); }
      /*! @copydoc butane_task_graph_add */
      Task::Id add(
        const Task::Affinity affinity,
        const Task::Id parent,
        const Task::Id dependency,
        const Task::Kernel kernel,
        uintptr_t _0, uintptr_t _1)
      {
        return ((Task::Id)
          butane_task_graph_add_2(
            (butane_task_graph_t *)this,
            (butane_task_affinity_t)affinity,
            (butane_task_id_t)parent,
            (butane_task_id_t)dependency,
            (butane_task_kernel_fn)kernel,
            _0, _1)); }
      /*! @copydoc butane_task_graph_add */
      Task::Id add(
        const Task::Affinity affinity,
        const Task::Id parent,
        const Task::Id dependency,
        const Task::Kernel kernel,
        uintptr_t _0, uintptr_t _1, uintptr_t _2)
      {
        return ((Task::Id)
          butane_task_graph_add_3(
            (butane_task_graph_t *)this,
            (butane_task_affinity_t)affinity,
            (butane_task_id_t)parent,
            (butane_task_id_t)dependency,
            (butane_task_kernel_fn)kernel,
            _0, _1, _2)); }
      /*! @copydoc butane_task_graph_add */
      Task::Id add(
        const Task::Affinity affinity,
        const Task::Id parent,
        const Task::Id dependency,
        const Task::Kernel kernel,
        uintptr_t _0, uintptr_t _1, uintptr_t _2, uintptr_t _3)
      {
        return ((Task::Id)
          butane_task_graph_add_4(
            (butane_task_graph_t *)this,
            (butane_task_affinity_t)affinity,
            (butane_task_id_t)parent,
            (butane_task_id_t)dependency,
            (butane_task_kernel_fn)kernel,
            _0, _1, _2, _3)); }
    public:
      /*! @copydoc butane_task_graph_kick */
      void kick() const {
        butane_task_graph_kick((const butane_task_graph_t *)this); }
      /*! @copydoc butane_task_graph_kick_and_wait */
      void kick_and_wait() const {
        butane_task_graph_kick_and_wait((const butane_task_graph_t *)this); }
      /*! @copydoc butane_task_graph_kick_and_do_work_while_waiting */
      void kick_and_do_work_while_waiting() const {
        butane_task_graph_kick_and_do_work_while_waiting((const butane_task_graph_t *)this); }
  };
} /* butane */
#endif

#endif /* _BUTANE_TASK_GRAPH_H_ */
