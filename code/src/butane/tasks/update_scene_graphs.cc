// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/update_scene_graphs.h>

#include <butane/world.h>

namespace butane {
namespace Tasks {
  void update_scene_graphs(
    Task* task,
    uintptr_t data )
  {
    UpdateSceneGraphsData* usgd = (UpdateSceneGraphsData*)data;
    for (size_t unit = 0; unit < usgd->world->_units.size(); ++unit)
      usgd->world->_units[unit]->scene_graph().update();
  }
} // Tasks
} // butane
