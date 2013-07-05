// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASKS_APPLY_VISUAL_REPRESENTATION_STREAM_H_
#define _BUTANE_TASKS_APPLY_VISUAL_REPRESENTATION_STREAM_H_

#include <butane/butane.h>
#include <butane/task.h>

namespace butane {
  class World;
  class VisualRepresentationStream;
namespace Tasks {
  struct ApplyVisualRepresentationStreamData {
    const World* world;
    const VisualRepresentationStream* vrs;
  };

  extern BUTANE_EXPORT void apply_visual_representation_stream(
    Task* task,
    uintptr_t data );
} // Tasks
} // butane

#endif // _BUTANE_TASKS_APPLY_VISUAL_REPRESENTATION_STREAM_H_
