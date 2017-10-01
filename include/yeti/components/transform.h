//===-- yeti/components/transform.h ---------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_COMPONENTS_TRANSFORM_H_
#define _YETI_COMPONENTS_TRANSFORM_H_

#include "yeti/core.h"

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

#include "yeti/math.h"

namespace yeti {

struct Pose {
#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  Vec3 position;
  Quaternion rotation;
  Vec3 scale;
#else
  Mat4 matrix;
#endif
};

struct Transform {
  /// Opaque handle to a transform component.
  struct Handle {
    u32 instance;
  };
};

class YETI_PUBLIC TransformSystem {
 YETI_DISALLOW_COPYING(TransformSystem)

 private:
  TransformSystem(EntityManager *entities);
  ~TransformSystem();

 public:
  /// \brief Creates a new transform associated with @entity.
  ///
  /// \param @position The initial local position.
  /// \param @rotation The initial local rotation.
  /// \param @scale The initial local scale.
  ///
  Transform::Handle create(Entity entity,
                           const Vec3 &position = Vec3(0.f, 0.f, 0.f),
                           const Quaternion &rotation = Quaternion::IDENTITY,
                           const Vec3 &scale = Vec3(1.f, 1.f, 1.f));

  /// \brief Destroys a particular transform.
  void destroy(Transform::Handle handle);

  /// \brief Destroys transform associated with @entity.
  void destroy(Entity entity);

  /// \brief Returns a handle to the transform with associated with @entity.
  Transform::Handle lookup(Entity entity);

  /// \brief Determines whether or not @entity has an associated transform.
  bool has(Entity entity);

  /// \brief Links @child to @parent.
  ///
  /// \detail After linking, @child will follow the movement of @parent.
  ///
  /// \param @child The transform that will become the child.
  /// \param @parent The transform that will become the parent.
  /// \param @position Local position @child should have after linking.
  /// \param @rotation Local rotation @child should have after linking.
  /// \param @scale Local scale @child should have after linking.
  ///
  void link(Transform::Handle child,
            Transform::Handle parent,
            const Vec3 &position = Vec3(0.f, 0.f, 0.f),
            const Quaternion &rotation = Quaternion::IDENTITY,
            const Vec3 &scale = Vec3(1.f, 1.f, 1.f));

  /// \brief Unlinks an transform from its parent.
  ///
  /// \detail After unlinking, @handle will no longer follow the movement of
  /// its parent.
  ///
  /// \param @handle The handle that will become parentless.
  ///
  void unlink(Transform::Handle handle);

  /// \brief Returns the pose of an transform with respect to its parent.
  ///
  /// \note If the transform is parentless, then the local pose is the same as
  /// its world pose.
  ///
  /// \warning Since release builds store local poses as matrices as an
  /// optimization, we quantize scale when decomposing the matrix to prevent
  /// drift.
  ///
  /// \return Pose of the transform with respect to its parent.
  ///
  Pose get_local_pose(Transform::Handle handle);

  /// \brief Returns the position of an transform with respect to its parent.
  ///
  /// \note If the transform is parentless, then the local position is the same
  /// as its world position.
  ///
  /// \return Position of the transform with respect to its parent.
  ///
  Vec3 get_local_position(Transform::Handle handle);

  /// \brief Returns the rotation of an transform with respect to its parent.
  ///
  /// \note If the transform is parentless, then the local rotation is the same
  /// as its world rotation.
  ///
  /// \return Rotation of the transform with respect to its parent.
  ///
  Quaternion get_local_rotation(Transform::Handle handle);

  /// \brief Returns the scale of an transform with respect to its parent.
  ///
  /// \note If the transform is parentless, then the local scale is the same as
  /// its world scale.
  ///
  /// \warning Since release builds store local poses as matrices as an
  /// optimization, we quantize scale when decomposing the matrix to prevent
  /// drift.
  ///
  /// \return Scale of the transform with respect to its parent.
  ///
  Vec3 get_local_scale(Transform::Handle handle);

  /// \brief Sets the pose of an transform with respect to its parent.
  ///
  /// \note Updates are deferred. Thus the new local pose is not reflected in
  /// the world poses of the transform or its children until the system is
  /// updated.
  ///
  /// \see yeti::TransformSystem::recompute
  ///
  void set_local_pose(Transform::Handle handle,
                      const Pose &new_local_pose);

  /// \brief Sets the position of an transform with respect to its parent.
  ///
  /// \note Updates are deferred. Thus the new local pose is not reflected in
  /// the world poses of the transform or its children until the system is
  /// updated.
  ///
  /// \see yeti::TransformSystem::recompute
  ///
  void set_local_position(Transform::Handle handle,
                          const Vec3 &new_local_position);

  /// \brief Sets the rotation of an transform with respect to its parent.
  ///
  /// \note Updates are deferred. Thus the new local pose is not reflected in
  /// the world poses of the transform or its children until the system is
  /// updated.
  ///
  /// \see yeti::TransformSystem::recompute
  ///
  void set_local_rotation(Transform::Handle handle,
                          const Quaternion &new_local_rotation);

  /// \brief Sets the scale of an transform with respect to its parent.
  ///
  /// \note Updates are deferred. Thus the new local pose is not reflected in
  /// the world poses of the transform or its children until the system is
  /// updated.
  ///
  /// \see yeti::TransformSystem::recompute
  ///
  void set_local_scale(Transform::Handle handle,
                       const Vec3 &new_local_scale);

  /// \brief Gets the world-space pose of an transform.
  ///
  /// \note May be out of date as updates are deferred.
  ///
  Mat4 get_world_pose(Transform::Handle handle);

 public:
  /// \brief Recomputes world poses of modified transforms.
  void update();

  /// \brief Forces recomputation of the world poses of an instance and its
  /// children.
  ///
  /// \warning Forcing recomputation is very expensive compared to the batch
  /// update performed every frame. You should only rely on this when
  /// absolutely necessary.
  ///
  void recompute(Transform::Handle handle);

 public:
  /// \brief Fills @changed with entities with transforms that have changed
  /// since the last update.
  ///
  /// \warning Since changes are only tracked since the last update, once you
  /// call `update` you won't know which transforms have changed. Make sure to
  /// grab this prior to updating.
  ///
  void changed(core::Array<Entity> &changed) const;

 public:
  /// \internal Description of this component.
  static const Component *component();

 private:
  /// \internal Callbacks and shims exposed through `Component`.
  /// @{

  static void *create(EntityManager *entities);

  static void destroy(void *system);

  static void spawn(void *system,
                    const Entity *entities,
                    const u32 *owners,
                    size_t instances,
                    const void *data);

  static bool compile(const component_compiler::Environment *env,
                      const component_compiler::Input *input,
                      const component_compiler::Output *output);

  static bool compatible(u32 version);

  /// @}

 private:
  EntityManager *entities_;

  core::Array<u32> entity_to_instance_;
  core::Array<Entity> instance_to_entity_;

  core::Array<u32> parent_;

  core::Array<Pose> local_poses_;
  core::Array<Mat4> world_poses_;

  // PERF(mtwilliams): MEM(mtwilliams): Use bitsets.
  core::Array<bool> dirty_;
  core::Array<bool> changed_;
};

} // yeti

#endif // _YETI_COMPONENTS_TRANSFORM_H_
