// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/task.h>

namespace butane {
  Task::Task(
    const char* name,
    Affinity affinity,
    Task* parent,
    Task* depends_on,
    int32_t num_of_open_work_items,
    Kernel kernel,
    uintptr_t data
  ) : _name(name)
    , _affinity(affinity)
    , _parent(parent)
    , _depends_on(depends_on)
    , _num_of_open_work_items(num_of_open_work_items)
    , _num_of_open_dependencies(0)
    , _kernel(kernel)
    , _data(data)
  {
  }

  Task::~Task()
  {
  #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_RELEASE_BUILD)
    Allocators::scratch().free((void*)_name);
  #endif
  }

  static const char* derive_task_name_from_kernel(
    Task::Kernel kernel )
  {
  #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_RELEASE_BUILD)
    const Symbol sym = Symbol::from_address((void*)kernel);
    char* name = (char*)Allocators::scratch().alloc(sym.name().size());
    copy((void*)name, (const void*)sym.name().raw(), sym.name().size());
    return (const char*)name;
  #else
    return nullptr;
  #endif
  }

  Task* Task::prepare(
    const Affinity affinity,
    Kernel kernel,
    uintptr_t data,
    Task* depends_on )
  {
    assert(kernel != nullptr);
    Task* task = make_new(Task, Allocators::scratch())(
      derive_task_name_from_kernel(kernel),
      affinity, nullptr, depends_on, 1, kernel, data);
    if (depends_on)
      depends_on->_num_of_open_dependencies += 1;
    return task;
  }

  Task* Task::child(
    const Affinity affinity,
    Kernel kernel,
    uintptr_t data,
    Task* depends_on )
  {
    assert(kernel != nullptr);
    Task* task = make_new(Task, Allocators::scratch())(
      derive_task_name_from_kernel(kernel),
      affinity, this, depends_on, 1, kernel, data);
    _num_of_open_work_items += 1;
    if (depends_on)
      depends_on->_num_of_open_dependencies += 1;
    return task;
  }

  void Task::kick()
  {
    Scheduler::enqueue(this);
  }
} // butane
