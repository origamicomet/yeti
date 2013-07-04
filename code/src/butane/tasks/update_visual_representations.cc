// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/update_visual_representations.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void update_visual_representations(
    Task* task,
    uintptr_t data )
  {
    UpdateVisualRepresentationsData* uvrd =
      (UpdateVisualRepresentationsData*)data;

    VisualRepresentationStream& vrs =
      *(uvrd->world->_visual_representation_stream);

    for (size_t unit = 0; unit < uvrd->world->_spawning.size(); ++unit)
      uvrd->world->_units[uvrd->world->_spawning[unit]].create_visual_representation(vrs);

    for (size_t unit = 0; unit < uvrd->world->_units.size(); ++unit)
      uvrd->world->_units[unit].update_visual_representation(vrs);

    for (size_t unit = 0; unit < uvrd->world->_despawning.size(); ++unit)
      uvrd->world->_units[uvrd->world->_despawning[unit]].destroy_visual_representation(vrs);
  }
} // Tasks
} // butane
