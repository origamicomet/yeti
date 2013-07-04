// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/update_world.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void update_world(
    Task* task,
    uintptr_t data )
  {
    UpdateWorldData* uwd = (UpdateWorldData*)data;
  }
} // Tasks
} // butane
