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
  butane_task_id_t depends_on_;
  butane_task_kernel_fn kernel_;
  size_t num_of_args_;
  uintptr_t args_[4];
  size_t refs_;
} butane_task_t;

/* ========================================================================== */

/*! */
extern butane_task_id_t butane_task_invalid();

/*! */
extern butane_task_affinity_t butane_task_any();

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
      typedef butane_task_id_t Id;
      typedef butane_task_affinity_t Affinity;
      typedef butane_task_kernel_fn Kernel;
    public:
      static const Id invalid;
      static const Affinity any;
    public:
      Task();
      ~Task();
    public:
      void run() const;
    public:
      Id id() const {
        return ((Id)id_); }
      void set_id(const Id id) {
        id_ = ((butane_task_id_t)id); }
      Affinity affinity() const {
        return ((Affinity)affinity_); }
      void set_affinity(const Affinity affinity) {
        affinity_ = ((butane_task_id_t)affinity); }
      Id parent() const {
        return ((Id)parent_); }
      void set_parent(const Id parent) {
        parent_ = ((butane_task_id_t)parent); }
      Id depends_on() const {
        return ((Id)depends_on_); }
      void set_depends_on(const Id depends_on) {
        depends_on_ = ((butane_task_id_t)depends_on); }
      Kernel kernel() const { return ((Kernel)kernel_); }
      void set_kernel(Kernel kernel) {
        kernel_ = ((Kernel)kernel); num_of_args_ = 0; }
      void set_kernel(Kernel kernel, uintptr_t _0) {
        kernel_ = ((Kernel)kernel);  num_of_args_ = 1;
        args_[0] = _0; }
      void set_kernel(Kernel kernel, uintptr_t _0, uintptr_t _1) {
        kernel_ = ((Kernel)kernel); num_of_args_ = 2;
        args_[0] = _0; args_[1] = _1; }
      void set_kernel(Kernel kernel, uintptr_t _0, uintptr_t _1, uintptr_t _2) {
        kernel_ = ((Kernel)kernel); num_of_args_ = 3;
        args_[0] = _0; args_[1] = _1; args_[2] = _2; }
      void set_kernel(Kernel kernel, uintptr_t _0, uintptr_t _1, uintptr_t _2, uintptr_t _3) {
        kernel_ = ((Kernel)kernel); num_of_args_ = 4;
        args_[0] = _0; args_[1] = _1; args_[2] = _2; args_[3] = _3; }
  };
} /* butane */
#endif

#endif /* _BUTANE_TASK_H_ */
