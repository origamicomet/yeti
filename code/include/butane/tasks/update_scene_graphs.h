// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TASKS_UPDATE_SCENE_GRAPHS_H_
#define _BUTANE_TASKS_UPDATE_SCENE_GRAPHS_H_

#include <butane/butane.h>
#include <butane/task.h>

namespace butane {
  class SceneGraph;
namespace Tasks {
  struct UpdateSceneGraphsData {
    SceneGraph* scene_graphs;
    size_t num_of_scene_graphs;
    size_t stride;
  };

  extern BUTANE_EXPORT void update_scene_graphs(
    Task* task,
    uintptr_t data );
} // Tasks
} // butane

#endif // _BUTANE_TASKS_UPDATE_SCENE_GRAPHS_H_
