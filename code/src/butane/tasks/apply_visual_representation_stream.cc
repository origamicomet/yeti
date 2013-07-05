// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/apply_visual_representation_stream.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void apply_visual_representation_stream(
    Task* task,
    uintptr_t data )
  {
    ApplyVisualRepresentationStreamData* avrsd =
      (ApplyVisualRepresentationStreamData*)data;
    if (avrsd->vrs)
      avrsd->world->_visual_representation.apply(*avrsd->vrs);
    make_delete(VisualRepresentationStream, Allocators::scratch(), avrsd->vrs);
    Allocators::scratch().free((void*)data);
  }
} // Tasks
} // butane
