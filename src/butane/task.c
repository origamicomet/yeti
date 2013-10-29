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

butane_task_id_t butane_task_invalid() {
  return ~((butane_task_id_t)0);
}

butane_task_affinity_t butane_task_any() {
  return ~((butane_task_affinity_t)0);
}

/* ========================================================================== */

typedef void (*butane_task_kernel_0)(void);
typedef void (*butane_task_kernel_1)(uintptr_t);
typedef void (*butane_task_kernel_2)(uintptr_t,uintptr_t);
typedef void (*butane_task_kernel_3)(uintptr_t,uintptr_t,uintptr_t);
typedef void (*butane_task_kernel_4)(uintptr_t,uintptr_t,uintptr_t,uintptr_t);

void butane_task_run(const butane_task_t *task) {
  bt_assert(debug, task != NULL);
  bt_assert(debug, task->refs_ == 1);
  bt_assert(paranoid, task->kernel_ != NULL);
  bt_assert(paranoid, task->num_of_args_ > 0);
  bt_assert(paranoid, task->num_of_args_ <= 4);
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
  }

  Task::~Task() {
  }

  void Task::run() const {
    butane_task_run((const butane_task_t *)this);
  }
} /* butane */
#endif
