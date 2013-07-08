// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/frustum_cull.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void frustum_cull(
    Task* task,
    uintptr_t data )
  {
    FrustumCullData* fcd = (FrustumCullData*)data;

    for (size_t i = 0; i < fcd->num_of_objects; ++i) {
      const VisualRepresentation& object = fcd->objects[i];
      VisualRepresentation::Culled culled;
      culled.id = object.id;
      culled.visible = 0xFFFFFFFFu;
      (*fcd->culled) += culled;
    }

    Allocators::scratch().free((void*)data);
  }
} // Tasks
} // butane
