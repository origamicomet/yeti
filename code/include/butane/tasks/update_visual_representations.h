// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASKS_UPDATE_VISUAL_REPRESENTATIONS_H_
#define _BUTANE_TASKS_UPDATE_VISUAL_REPRESENTATIONS_H_

#include <butane/butane.h>
#include <butane/task.h>

namespace butane {
  class World;
  class VisualRepresentationStream;
namespace Tasks {
  struct UpdateVisualRepresentationsData {
    World* world;
  };

  extern BUTANE_EXPORT void update_visual_representations(
    Task* task,
    uintptr_t data );
} // Tasks
} // butane

#endif // _BUTANE_TASKS_UPDATE_VISUAL_REPRESENTATIONS_H_
