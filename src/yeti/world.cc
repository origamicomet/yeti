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

#include "yeti/resource_manager.h"

#include "yeti/resources/level_resource.h"
#include "yeti/resources/entity_resource.h"
#include "yeti/resources/prefab_resource.h"

#include "yeti/task.h"
#include "yeti/task_scheduler.h"

namespace yeti {

World::World()
  : entities_()
  , systems_(&entities_)
  , transforms_((TransformSystem *)systems_.lookup("transform"))
  , cameras_((CameraSystem *)systems_.lookup("camera"))
  , lights_((LightSystem *)systems_.lookup("light"))
{
}

World::~World() {
}

World *World::create() {
  World *world = YETI_NEW(World, core::global_heap_allocator());
  return world;
}

void World::update(const f32 delta_time) {
  yeti_assert_debug(delta_time >= 0.f);

  // Build task graph.
  // Kick and wait.
}

void World::destroy() {
  YETI_DELETE(World, core::global_heap_allocator(), this);
}

Entity World::spawn(Resource::Id id,
                    const Vec3 &position,
                    const Quaternion &rotation,
                    const Vec3 &scale) {
  const EntityResource *resource =
    (const EntityResource *)resource_manager::lookup(id);

  if (resource_manager::autoloads()) {
    // Wait until resource is loaded.
    while (resource_manager::state(id) != Resource::LOADED)
      core::Thread::yield();
  } else {
    yeti_assert_development(resource != NULL);
    yeti_assert_development(resource_manager::state(id) == Resource::LOADED);
  }

  const Entity entity = entities_.create();

  // TODO(mtwilliams): Create components.
  // TODO(mtwilliams): Lifecycle events.

  resource->deref();

  return entity;
}

void World::kill(Entity entity) {
  // TODO(mtwilliams): Destroy components.
  // TODO(mtwilliams): Lifecycle events.

  entities_.destroy(entity);
}

} // yeti
