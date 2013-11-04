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
 #  include <butane/task_graph.h>
/* ========================================================================== */

#include <butane/task_scheduler.h>

/* ========================================================================== */
/*  C (butane_task_graph_t):                                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

butane_task_graph_t *butane_task_graph_create(size_t sz) {
  butane_task_graph_t *task_graph = (butane_task_graph_t *)
    fnd_allocator_alloc(
      fnd_heap(),
      sizeof(butane_task_graph_t),
      fnd_alignof(butane_task_graph_t));
  task_graph->num_of_tasks_ = 0;
  task_graph->tasks_ = (butane_task_t *)
    fnd_allocator_alloc(
      fnd_heap(),
      sz * sizeof(butane_task_t),
      fnd_alignof(butane_task_t));
  task_graph->tasks_sz_ = sz;
  task_graph->completed_ = UINT32_C(0);
  return task_graph;
}

void butane_task_graph_destroy(butane_task_graph_t *task_graph) {
  bt_assert(debug, task_graph != NULL);
  fnd_allocator_free(fnd_heap(), (void *)task_graph->tasks_);
  fnd_allocator_free(fnd_heap(), (void *)task_graph);
}

/* ========================================================================== */

static butane_task_t *butane_task_graph_add_(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel)
{
  bt_assert(debug, task_graph != NULL);
  bt_assert(release, task_graph->num_of_tasks_ < task_graph->tasks_sz_);
  bt_assert(debug, affinity != 0);
  bt_assert(debug, kernel != NULL);
  const butane_task_id_t id = task_graph->num_of_tasks_++;
  butane_task_t *task = &task_graph->tasks_[id];
  task->id_ = id;
  task->affinity_ = affinity;
  if (parent != butane_task_invalid()) {
    bt_assert(debug, parent < (task_graph->num_of_tasks_ - 1));
    task_graph->tasks_[parent].refs_by_children_and_self_++; }
  task->parent_ = parent;
  if (dependency != butane_task_invalid()) {
    bt_assert(debug, dependency < (task_graph->num_of_tasks_ - 1));
    task_graph->tasks_[dependency].refs_by_dependencies_++; }
  task->dependency_ = dependency;
  task->kernel_ = kernel;
  task->refs_by_children_and_self_ = 1;
  task->refs_by_dependencies_ = 0;
  task->increment_on_completion_ = &task_graph->completed_;
  return task;
}

butane_task_id_t butane_task_graph_add(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel)
{
  butane_task_t *task =
    butane_task_graph_add_(task_graph, affinity, parent, dependency, kernel);
  task->num_of_args_ = 0;
  return task->id_;
}

butane_task_id_t butane_task_graph_add_1(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0)
{
  butane_task_t *task =
    butane_task_graph_add_(task_graph, affinity, parent, dependency, kernel);
  task->num_of_args_ = 1;
  task->args_[0] = _0;
  return task->id_;
}

butane_task_id_t butane_task_graph_add_2(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1)
{
  butane_task_t *task =
    butane_task_graph_add_(task_graph, affinity, parent, dependency, kernel);
  task->num_of_args_ = 2;
  task->args_[0] = _0;
  task->args_[1] = _1;
  return task->id_;
}

butane_task_id_t butane_task_graph_add_3(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2)
{
  butane_task_t *task =
    butane_task_graph_add_(task_graph, affinity, parent, dependency, kernel);
  task->num_of_args_ = 3;
  task->args_[0] = _0;
  task->args_[1] = _1;
  task->args_[2] = _2;
  return task->id_;
}

butane_task_id_t butane_task_graph_add_4(
  butane_task_graph_t *task_graph,
  butane_task_affinity_t affinity,
  butane_task_id_t parent,
  butane_task_id_t dependency,
  butane_task_kernel_fn kernel,
  uintptr_t _0, uintptr_t _1, uintptr_t _2, uintptr_t _3)
{
  butane_task_t *task =
    butane_task_graph_add_(task_graph, affinity, parent, dependency, kernel);
  task->num_of_args_ = 4;
  task->args_[0] = _0;
  task->args_[1] = _1;
  task->args_[2] = _2;
  task->args_[3] = _3;
  return task->id_;
}

/* ========================================================================== */

void butane_task_graph_kick(
  const butane_task_graph_t *task_graph)
{
  bt_assert(debug, task_graph != NULL);
  butane_task_sched_enqueue(task_graph->num_of_tasks_, task_graph->tasks_);
}

void butane_task_graph_kick_and_wait(
  const butane_task_graph_t *task_graph)
{
  bt_assert(debug, task_graph != NULL);
  fnd_atomic_uint32_t *completed = ((fnd_atomic_uint32_t *)&task_graph->completed_);
  const fnd_atomic_uint32_t must_be_completed_ =
    task_graph->completed_ + task_graph->num_of_tasks_;
  butane_task_sched_enqueue(task_graph->num_of_tasks_, task_graph->tasks_);
  while (fnd_atomic_uint32_compare_swap_relaxed(completed, must_be_completed_, must_be_completed_) != must_be_completed_);
}

void butane_task_graph_kick_and_do_work_while_waiting(
  const butane_task_graph_t *task_graph)
{
  bt_assert(debug, task_graph != NULL);
  fnd_atomic_uint32_t *completed = ((fnd_atomic_uint32_t *)&task_graph->completed_);
  const fnd_atomic_uint32_t must_be_completed_ =
    task_graph->completed_ + task_graph->num_of_tasks_;
  butane_task_sched_enqueue(task_graph->num_of_tasks_, task_graph->tasks_);
  while (fnd_atomic_uint32_compare_swap_relaxed(completed, must_be_completed_, must_be_completed_) != must_be_completed_)
    butane_task_sched_do_work();
}

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::TaskGraph):                                                  */
/* ========================================================================== */

/* ... */
