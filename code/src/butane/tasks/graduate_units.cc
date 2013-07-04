// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/graduate_units.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void graduate_units(
    Task* task,
    uintptr_t data )
  {
    GraduateUnitsData* gud = (GraduateUnitsData*)data;

    for (size_t unit = 0; unit < gud->world->_despawning.size(); ++unit) {
      // TODO: fix dtor bug with copy and resize()
      const Unit::Id id = gud->world->_despawning[unit];
      void* dest = (void*)&gud->world->_units[gud->world->_unit_ids[id]];
      const void* src = (const void*)&gud->world->_units[gud->world->_units.size() - 1];
      gud->world->_units[gud->world->_unit_ids[id]].~Unit();
      copy_safe(dest, src, sizeof(Unit));
      gud->world->_unit_ids[gud->world->_units[gud->world->_units.size() - 1].id()] = gud->world->_unit_ids[id];
      gud->world->_unit_ids[id] = gud->world->_next_avail_unit_id;
      gud->world->_next_avail_unit_id = id;
      gud->world->_units.resize(gud->world->_units.size() - 1);
    }

    gud->world->_spawning.resize(0);
    gud->world->_despawning.resize(0);
  }
} // Tasks
} // butane
