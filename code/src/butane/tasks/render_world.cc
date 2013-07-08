// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/render_world.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void render_world(
    Task* task,
    uintptr_t data )
  {
    RenderWorldData* rwd = (RenderWorldData*)data;
    Allocators::scratch().free((void*)rwd);
  }
} // Tasks
} // butane
