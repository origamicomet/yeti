//===-- yeti/task_manager.h -------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_TASK_MANAGER_H_
#define _YETI_TASK_MANAGER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

// TODO(mtwilliams): Forward declare `Task`?
#include "yeti/task.h"

namespace yeti {

namespace task_manager {
  extern YETI_PUBLIC Task *acquire_a_task();
  extern YETI_PUBLIC void relinquish_a_task(Task *task);
  extern YETI_PUBLIC Task::Permit *acquire_a_permit();
  extern YETI_PUBLIC void relinquish_a_permit(Task::Permit *permit);
} // task_manager

} // yeti

#endif // _YETI_TASK_MANAGER_H_
