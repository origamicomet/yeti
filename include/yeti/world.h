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

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/ecs.h"

#include "yeti/transform.h"
#include "yeti/camera.h"
#include "yeti/light.h"

namespace yeti {

/// ...
class YETI_PUBLIC World {
 YETI_DISALLOW_COPYING(World)

 protected:
  World();
  ~World();

 public:
  static World *create();

  void update(const f32 delta_time);

  void destroy();

 public:
  /// \internal Reflects any changes to logical representation.
  void reflect() const;

  /// \internal Applys changes from logical representation.
  void apply();

 private:
#if 0
  EntityManager entities_;

  SystemManager systems_;

  // Commonly used systems.
  TransformSystem *transform_system_;
  CameraSystem *camera_system_;
  LightSystem *light_system_;
  VisibilitySystem *visibility_system_;
  TagSystem *tag_system_;
  UserDataSystem *user_data_system_;
#endif
};

} // yeti

#endif // _YETI_WORLD_H_
