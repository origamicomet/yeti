//=== bitbyte/butane/task.cc =================================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

#include <bitbyte/butane/task.h>

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//
// Constructors:
//

Task::Task() {}

//===----------------------------------------------------------------------===//
// Copy constructors:
//

#if 0
Task::Task(const Task &task) {}
#endif

//===----------------------------------------------------------------------===//
// Assignment operators:
//

#if 0
Task &Task::operator=(const Task &task) { return *this; }
#endif

//===----------------------------------------------------------------------===//
// Destructor:
//

Task::~Task() {}

//===----------------------------------------------------------------------===//
// Task::describe
//

Task &Task::describe() {
  // OPTIMIZATION(mwilliams): Reuse tasks via an object pool.
  Task &task = *(new (foundation::heap().alloc(sizeof(Task), alignof(Task))) Task);
  foundation::atomic::relaxed::store(&task.permissions_, -1);
  task.permits_ = NULL;
  task.kernel_ = []() -> void {};
  return task;
}

//===----------------------------------------------------------------------===//
// Task::parent
//

Task &Task::parent(Task &parent) {
  // OPTIMIZATION(mwilliams): Reuse permits via an object pool.
  Task::Permit *permit =
    (Task::Permit *)foundation::heap().alloc(
      sizeof(Task::Permit),
      alignof(Task::Permit));
  permit->next = permits_;
  permit->task = &parent;
  permits_ = permit;
  foundation::atomic::relaxed::decrement(&parent.permissions_);
  return *this;
}

//===----------------------------------------------------------------------===//
// Task::dependency
//

Task &Task::dependency(Task &dependency) {
  // OPTIMIZATION(mwilliams): Reuse permits via an object pool.
  Task::Permit *permit =
    (Task::Permit *)foundation::heap().alloc(
      sizeof(Task::Permit),
      alignof(Task::Permit));
  permit->next = dependency.permits_;
  permit->task = this;
  dependency.permits_ = permit;
  foundation::atomic::relaxed::decrement(&permissions_);
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
