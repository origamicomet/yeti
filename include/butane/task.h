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
/*! @file include/butane/task.h
     Provides `butane_task_t` and `butane::Task` which represent a single
     unit of work that can be scheduled on the CPU.                           */
/* ========================================================================== */

#ifndef _BUTANE_TASK_H_
#define _BUTANE_TASK_H_

#include <butane/config.h>
#include <butane/foundation.h>

/* ========================================================================== */
/*  Task:                                                                     */
/*   * C (butane_task_t);                                                     */
/*   * "C++" (butane::Task).                                                  */
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_task_t):                                                        */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/*! */
typedef uint32_t butane_task_id_t;

/*! */
typedef uint32_t butane_task_affinity_t;

/*! */
typedef void (*butane_task_kernel_fn)(void);

/*! */
typedef struct butane_task {
  butane_task_id_t id_;
  butane_task_affinity_t affinity_;
  butane_task_id_t parent_;
  butane_task_id_t dependency_;
  butane_task_kernel_fn kernel_;
  size_t num_of_args_;
  uintptr_t args_[4];
  uint32_t refs_by_children_and_self_;
  uint32_t refs_by_dependencies_;
} butane_task_t;

/* ========================================================================== */

/*! */
extern butane_task_id_t butane_task_invalid();

/*! */
extern butane_task_affinity_t butane_task_any();

/* ========================================================================== */

/*! */
extern butane_task_id_t butane_task_id(
  const butane_task_t *task);

/*! */
extern void butane_task_set_id(
  butane_task_t *task,
  const butane_task_id_t id);

/*! */
extern butane_task_affinity_t butane_task_affinity(
  const butane_task_t *task);

/*! */
extern void butane_task_set_affinity(
  butane_task_t *task,
  const butane_task_affinity_t affinity);

/*! */
extern butane_task_id_t butane_task_parent(
  const butane_task_t *task);

/*! */
extern void butane_task_set_parent(
  butane_task_t *task,
  const butane_task_id_t parent);

/*! */
extern butane_task_id_t butane_task_dependency(
  const butane_task_t *task);

/*! */
extern void butane_task_set_dependency(
  butane_task_t *task,
  const butane_task_id_t dependency);

/*! */
extern butane_task_kernel_fn butane_task_kernel(
  const butane_task_t *task);

/*! */
extern void butane_task_set_kernel(
  butane_task_t *task,
  butane_task_kernel_fn kernel);

extern void butane_task_set_kernel_1(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0);

extern void butane_task_set_kernel_2(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1);

extern void butane_task_set_kernel_3(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2);

extern void butane_task_set_kernel_4(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2, uintptr_t _3);

/* ========================================================================== */

/*! */
extern void butane_task_run(const butane_task_t *task);


#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::Task):                                                       */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  /*! @copydoc butane_task_t */
  class Task : private butane_task_t {
    public:
      /*! @copydoc butane_task_id_t */
      typedef butane_task_id_t Id;
      /*! @copydoc butane_task_affinity_t */
      typedef butane_task_affinity_t Affinity;
      /*! @copydoc butane_task_kernel_fn */
      typedef butane_task_kernel_fn Kernel;
    public:
      /*! @copydoc butane_task_invalid */
      static const Id invalid;
      /*! @copydoc butane_task_any */
      static const Affinity any;
    public:
      Task();
      ~Task();
    public:
      /*! @copydoc butane_task_run */
      void run() const;
    public:
      /*! @copydoc butane_task_id */
      Id id() const;
      /*! @copydoc butane_task_set_id */
      void set_id(const Id id);
      /*! @copydoc butane_task_affinity */
      Affinity affinity() const;
      /*! @copydoc butane_task_set_affinity */
      void set_affinity(const Affinity affinity);
      /*! @copydoc butane_task_parent */
      Id parent() const;
      /*! @copydoc butane_task_set_parent */
      void set_parent(const Id parent);
      /*! @copydoc butane_task_dependency */
      Id dependency() const;
      /*! @copydoc butane_task_set_dependency */
      void set_dependency(const Id dependency);
      /*! @copydoc butane_task_kernel */
      Kernel kernel() const;
      /*! @copydoc butane_task_set_kernel */
      void set_kernel(Kernel kernel);
      void set_kernel(Kernel kernel, uintptr_t);
      void set_kernel(Kernel kernel, uintptr_t, uintptr_t);
      void set_kernel(Kernel kernel, uintptr_t, uintptr_t, uintptr_t);
      void set_kernel(Kernel kernel, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
  };
} /* butane */
#endif

#endif /* _BUTANE_TASK_H_ */
