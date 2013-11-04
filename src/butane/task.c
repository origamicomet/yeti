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
 #  include <butane/task.h>
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_task_t):                                                        */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

butane_task_id_t butane_task_invalid(void) {
  return ~((butane_task_id_t)0);
}

butane_task_affinity_t butane_task_any(void) {
  return ~((butane_task_affinity_t)0);
}

/* ========================================================================== */

butane_task_id_t butane_task_id(const butane_task_t *task) {
  bt_assert(debug, task != NULL);
  return task->id_;
}

void butane_task_set_id(butane_task_t *task, const butane_task_id_t id) {
  bt_assert(debug, task != NULL);
  task->id_ = id;
}

butane_task_affinity_t butane_task_affinity(const butane_task_t *task) {
  bt_assert(debug, task != NULL);
  return task->affinity_;
}

void butane_task_set_affinity(butane_task_t *task, const butane_task_affinity_t affinity) {
  bt_assert(debug, task != NULL);
  task->affinity_ = affinity;
}

butane_task_id_t butane_task_parent(const butane_task_t *task) {
  bt_assert(debug, task != NULL);
  return task->parent_;
}

void butane_task_set_parent(butane_task_t *task, const butane_task_id_t parent) {
  bt_assert(debug, task != NULL);
  task->parent_ = parent;
}

butane_task_id_t butane_task_dependency(const butane_task_t *task) {
  bt_assert(debug, task != NULL);
  return task->dependency_;
}

void butane_task_set_dependency(butane_task_t *task, const butane_task_id_t dependency) {
  bt_assert(debug, task != NULL);
  task->dependency_ = dependency;
}

butane_task_kernel_fn butane_task_kernel(const butane_task_t *task) {
  bt_assert(debug, task != NULL);
  return task->kernel_;
}

void butane_task_set_kernel(
  butane_task_t *task,
  butane_task_kernel_fn kernel)
{
  bt_assert(debug, task != NULL);
  task->kernel_ = kernel;
  task->num_of_args_ = 0;
}

void butane_task_set_kernel_1(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0)
{
  bt_assert(debug, task != NULL);
  task->kernel_ = kernel;
  task->num_of_args_ = 1;
  task->args_[0] = _0;
}

void butane_task_set_kernel_2(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1)
{
  bt_assert(debug, task != NULL);
  task->kernel_ = kernel;
  task->num_of_args_ = 2;
  task->args_[0] = _0;
  task->args_[1] = _1;
}

void butane_task_set_kernel_3(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2)
{
  bt_assert(debug, task != NULL);
  task->kernel_ = kernel;
  task->num_of_args_ = 3;
  task->args_[0] = _0;
  task->args_[1] = _1;
  task->args_[2] = _2;
}

void butane_task_set_kernel_4(
  butane_task_t *task,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2, uintptr_t _3)
{
  bt_assert(debug, task != NULL);
  task->kernel_ = kernel;
  task->num_of_args_ = 4;
  task->args_[0] = _0;
  task->args_[1] = _1;
  task->args_[2] = _2;
  task->args_[3] = _3;
}

/* ========================================================================== */

typedef void (*butane_task_kernel_0)(void);
typedef void (*butane_task_kernel_1)(uintptr_t);
typedef void (*butane_task_kernel_2)(uintptr_t,uintptr_t);
typedef void (*butane_task_kernel_3)(uintptr_t,uintptr_t,uintptr_t);
typedef void (*butane_task_kernel_4)(uintptr_t,uintptr_t,uintptr_t,uintptr_t);

void butane_task_run(const butane_task_t *task) {
  bt_assert(debug, task != NULL);
  switch (task->num_of_args_) {
    case 0:
      ((butane_task_kernel_0)task->kernel_)();
      break;
    case 1:
      ((butane_task_kernel_1)task->kernel_)(
        task->args_[0]);
      break;
    case 2:
      ((butane_task_kernel_2)task->kernel_)(
        task->args_[0],
        task->args_[1]);
      break;
    case 3:
      ((butane_task_kernel_3)task->kernel_)(
        task->args_[0],
        task->args_[1],
        task->args_[2]);
      break;
    case 4:
      ((butane_task_kernel_4)task->kernel_)(
        task->args_[0],
        task->args_[1],
        task->args_[2],
        task->args_[3]);
      break;
  }
}

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::Task):                                                       */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  const Task::Id Task::invalid = ~((Id)butane_task_invalid());
  const Task::Affinity Task::any = ~((Affinity)butane_task_any());

  Task::Task() {
    id_ = Task::invalid;
    affinity_ = butane_task_any();
    parent_ = butane_task_invalid();
    dependency_ = butane_task_invalid();
    kernel_ = NULL;
    num_of_args_ = 0;
    args_[0] = args_[1] = args_[2] = args_[3] = 0;
    refs_by_children_and_self_ = 1;
    refs_by_dependencies_ = 0;
    increment_on_completion_ = NULL;
  }

  Task::~Task() {
  }

  void Task::run() const {
    butane_task_run((const butane_task_t *)this);
  }

  Task::Id Task::id() const {
    return ((Id)butane_task_id((const butane_task_t *)this));
  }

  void Task::set_id(const Id id) {
    butane_task_set_id((butane_task_t *)this, (const butane_task_id_t)id);
  }

  Task::Affinity Task::affinity() const {
    return ((Affinity)butane_task_affinity((const butane_task_t *)this));
  }

  void Task::set_affinity(const Affinity affinity) {
    butane_task_set_affinity(
      (butane_task_t *)this,
      (const butane_task_affinity_t)affinity);
  }

  Task::Id Task::parent() const {
    return ((Id)butane_task_parent((const butane_task_t *)this));
  }

  void Task::set_parent(const Id parent) {
    butane_task_set_parent(
      (butane_task_t *)this,
      (const butane_task_id_t)parent);
  }

  Task::Id Task::dependency() const {
    return ((Id)butane_task_dependency((const butane_task_t *)this));
  }

  void Task::set_dependency(const Id dependency) {
    butane_task_set_dependency(
      (butane_task_t *)this,
      (const butane_task_id_t)dependency);
  }

  Task::Kernel Task::kernel() const {
    return ((Kernel)butane_task_kernel((const butane_task_t *)this));
  }

  void Task::set_kernel(Kernel kernel, uintptr_t _0) {
    butane_task_set_kernel_1(
      (butane_task_t *)this,
      (butane_task_kernel_fn)kernel,
      _0);
  }

  void Task::set_kernel(Kernel kernel, uintptr_t _0, uintptr_t _1) {
    butane_task_set_kernel_2(
      (butane_task_t *)this,
      (butane_task_kernel_fn)kernel,
      _0, _1);
  }

  void Task::set_kernel(Kernel kernel, uintptr_t _0, uintptr_t _1, uintptr_t _2) {
    butane_task_set_kernel_3(
      (butane_task_t *)this,
      (butane_task_kernel_fn)kernel,
      _0, _1, _2);
  }

  void Task::set_kernel(Kernel kernel, uintptr_t _0, uintptr_t _1, uintptr_t _2, uintptr_t _3) {
    butane_task_set_kernel_4(
      (butane_task_t *)this,
      (butane_task_kernel_fn)kernel,
      _0, _1, _2, _3);
  }
} /* butane */
#endif
