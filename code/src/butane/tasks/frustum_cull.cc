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

    const VisualRepresentation* object = fcd->objects;
    const VisualRepresentation* last_object = (const VisualRepresentation*)(
      ((uintptr_t)fcd->objects) + fcd->num_of_objects * fcd->stride_between_objects);

    while (object < last_object) {
      VisualRepresentation::Culled culled;
      culled.id = object->id;
      culled.visible = 0xFFFFFFFFu;
      (*fcd->culled) += culled;
      object = (const VisualRepresentation*)(
        ((uintptr_t)object) + fcd->stride_between_objects);
    }

    Allocators::scratch().free((void*)data);
  }
} // Tasks
} // butane
