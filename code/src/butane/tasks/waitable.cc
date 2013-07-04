// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/waitable.h>

namespace butane {
namespace Tasks {
  void waitable(
    Task* task,
    uintptr_t data )
  {
    __sync_val_compare_and_swap(((int32_t*)data), 0, 1);
  }
} // Tasks
} // butane
