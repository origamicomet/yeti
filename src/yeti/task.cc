//===-- yeti/task.cc ------------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/task.h"

namespace yeti {

Task::Handle task::describe(Task::Kernel kernel, void *data) {
  return ::loom_describe(kernel, data, 0);
}

void task::permits(Task::Handle task, Task::Handle permittee) {
  ::loom_permits(task, permittee);
}

} // yeti
