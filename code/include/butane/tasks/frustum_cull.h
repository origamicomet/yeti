// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASKS_FRUSTUM_CULL_H_
#define _BUTANE_TASKS_FRUSTUM_CULL_H_

#include <butane/butane.h>
#include <butane/task.h>
#include <butane/math.h>
#include <butane/visual_representation.h>

namespace butane {
namespace Tasks {
  struct FrustumCullData {
    const VisualRepresentation* objects;
    size_t num_of_objects;
    Mat4 frustums[1 /* 32 */];
    Array<VisualRepresentation::Culled>* culled;
  };

  extern BUTANE_EXPORT void frustum_cull(
    Task* task,
    uintptr_t data );
} // Tasks
} // butane

#endif // _BUTANE_TASKS_FRUSTUM_CULL_H_
