//===-- yeti/world.cc -----------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/world.h"

#include "yeti/task.h"
#include "yeti/task_scheduler.h"

namespace yeti {

World::World() {
}

World::~World() {
}

World *World::create() {
  return NULL;
}

void World::update(const f32 delta_time) {
  yeti_assert_debug(delta_time >= 0.f);

  // Build task graph.
  // Kick and wait.
}

void World::destroy() {
  // BUG(mtwilliams): Potential memory leak?
  delete this;
}

void World::reflect() const {
  // Lifecycle
  // For each system...
   // If `visual`, reflect.
   // If `audio`, trap.
   // If `physics`, trap.
}

void World::apply() {
  // Lifecycle
   // For each system...
    // Notify about lifecycle events.
    // Apply changes.
}

} // yeti
