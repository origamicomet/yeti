//===-- yeti/world.h ------------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_WORLD_H_
#define _YETI_WORLD_H_

#include "yeti/core.h"

#include "yeti/resource.h"

#include "yeti/math.h"

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

// Pointers to commonly accessed components are provided. Reduces overhead, and
// improves readability.
#include "yeti/components/transform.h"
#include "yeti/components/camera.h"
#include "yeti/components/light.h"
#include "yeti/components/mesh.h"
#include "yeti/components/terrain.h"
#include "yeti/components/data.h"
#include "yeti/components/tag.h"

namespace yeti {

class YETI_PUBLIC World {
 YETI_DISALLOW_COPYING(World)

 private:
  World();
  ~World();

 public:
  static World *create();

  void update(const f32 delta_time);

  void destroy();

 public:
  /// Spawns an entity from a resource given by @id at @position with a rotation
  /// of @rotation and scaled by @scale.
  ///
  /// \return Handle to spawned entity.
  ///
  Entity spawn(Resource::Id id,
               const Vec3 &position = Vec3(0.f, 0.f, 0.f),
               const Quaternion &rotation = Quaternion(0.f, 0.f, 0.f, 1.f),
               const Vec3 &scale = Vec3(1.f, 1.f, 1.f));

  /// Kills @entity.
  void kill(Entity entity);

 private:
  EntityManager entities_;

  SystemManager systems_;

  TransformSystem *transforms_;
};

} // yeti

#endif // _YETI_WORLD_H_
