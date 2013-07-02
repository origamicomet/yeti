// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/tasks/update_scene_graphs.h>

#include <butane/scene_graph.h>

namespace butane {
namespace Tasks {
  void update_scene_graphs(
    Task* task,
    uintptr_t data )
  {
    UpdateSceneGraphsData* usgd = (UpdateSceneGraphsData*)data;
    SceneGraph* scene_graph = usgd->scene_graphs;
    SceneGraph* last_scene_graph =
    usgd->scene_graphs + (sizeof(SceneGraph) + usgd->stride) * usgd->num_of_scene_graphs;
    while (scene_graph < last_scene_graph) {
      scene_graph->update();
      scene_graph += (sizeof(SceneGraph) + usgd->stride);
    }
  }
} // Tasks
} // butane
