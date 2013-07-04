// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/task.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("task scheduler", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  static thread_safe::Queue<Task*>& __tasks_initializer() {
    static thread_safe::Queue<Task*> tasks(
      allocator(), BUTANE_TASK_SCHEDULER_MAXIMUM_NUM_OF_INFLIGHT_TASKS);
    return tasks;
  }

  static const thread_safe::Static< thread_safe::Queue<Task*> >
    __ts_tasks(&__tasks_initializer);

  static thread_safe::Queue<Task*>& __tasks() {
    return __ts_tasks();
  }

  static System::Info& sys_info()
  {
    static System::Info sys_info;
    static bool has_initialized = false;
    if (has_initialized)
      return sys_info;
    System::info(sys_info);
    has_initialized = true;
    return sys_info;
  }

  void Task::Scheduler::initialize()
  {
    static bool has_initialized = false;
    if (has_initialized)
      return;
    for (size_t worker_thread_id = 0; worker_thread_id < num_of_worker_treads(); ++worker_thread_id) {
      Thread worker_thread(&Scheduler::worker_thread, (void*)worker_thread_id);
      worker_thread.set_affinity((1u << worker_thread_id));
      worker_thread.run(); }
    has_initialized = true;
  }

  void Task::Scheduler::enqueue(
    Task* task )
  {
    assert(task != nullptr);
    __tasks().enqueue(task);
  }

  size_t Task::Scheduler::num_of_worker_treads()
  {
    return min(sys_info().processors.count, (size_t)BUTANE_TASK_SCHEDULER_MAXIMUM_NUM_OF_WORKER_THREADS);
  }

  Thread::Return Task::Scheduler::worker_thread(
    void* closure )
  {
    const uint id = (uint)closure;
    const Task::Affinity affinity = (1u << id);

    const String name = String::format(Allocators::heap(), "Worker Thread %u", id);
    const LogScope _(name.raw());

    while (true) {
      Task* task = nullptr;
      __tasks().dequeue(task);

      if (!(task->_affinity & affinity))
        goto skip;

      if (task->_depends_on)
        if (!__sync_bool_compare_and_swap(&task->_depends_on->_num_of_open_work_items, 0, 0))
          goto skip;

      if (!__sync_bool_compare_and_swap(&task->_num_of_open_work_items, 1, 1))
        goto skip;

      task->_kernel(task, task->_data);

      if (task->_depends_on)
        if (__sync_fetch_and_sub(&task->_depends_on->_num_of_open_dependencies, 1) == 1)
          make_delete(Task, Allocators::scratch(), task->_depends_on);

      if (task->_parent)
        __sync_fetch_and_sub(&task->_parent->_num_of_open_work_items, 1);

      if (__sync_bool_compare_and_swap(&task->_num_of_open_dependencies, 0, 0))
        make_delete(Task, Allocators::scratch(), task);
      else
        __sync_fetch_and_sub(&task->_num_of_open_work_items, 1);

      continue;

    skip:
      __tasks().enqueue(task);
    }

    __builtin_unreachable();
    return 0;
  }
} // butane
