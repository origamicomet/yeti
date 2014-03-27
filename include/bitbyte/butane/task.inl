//=== bitbyte/foundation/butane/task.inl =====================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/foundation/butane/task.inl
/// @brief ...
///

#ifndef _BITBYTE_BUTANE_TASK_INL_
#define _BITBYTE_BUTANE_TASK_INL_

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//
// Task::describe
//

Task &Task::describe() {
  Task &task = *((Task *)foundation::heap().alloc(sizeof(Task), alignof(Task)));
  task.parent_ = NULL;
  task.dependency_ = NULL;
  task.kernel_ = [](Task &) -> void {};
  foundation::atomic::relaxed::store(&task.refs_by_children_and_self_, 2);
  foundation::atomic::relaxed::store(&task.refs_by_dependencies_, 0);
  return task;
}

//===----------------------------------------------------------------------===//
// Task::parent
//

Task &Task::parent(Task *parent) {
  bitbyte_butane_assert(debug, parent != NULL);
  this->parent_ = parent;
  foundation::atomic::relaxed::increment(&parent->refs_by_children_and_self_);
  return *this;
}

//===----------------------------------------------------------------------===//
// Task::dependency
//

Task &Task::dependency(Task *dependency) {
  bitbyte_butane_assert(debug, dependency != NULL);
  this->dependency_ = dependency;
  foundation::atomic::relaxed::increment(&dependency->refs_by_dependencies_);
  return *this;
}

//===----------------------------------------------------------------------===//
// Task::kernel
//

Task &Task::kernel(Task::Kernel &kernel) {
  bitbyte_butane_assert(debug, kernel);
  this->kernel_ = kernel;
  return *this;
}

} // butane
} // bitbyte

#endif // _BITBYTE_BUTANE_TASK_INL_
