// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/update_units.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void update_units(
    Task* task,
    uintptr_t data )
  {
    UpdateUnitsData* uud = (UpdateUnitsData*)data;
    for (size_t unit = 0; unit < uud->world->_units.size(); ++unit)
      uud->world->_units[unit]->update(uud->dt);
  }
} // Tasks
} // butane

