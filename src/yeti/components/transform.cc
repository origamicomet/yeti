//===-- yeti/components/transform.cc --------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/components/transform.h"

// PERF(mtwilliams): Move to garbage collection.
// TODO(mtwilliams): Quantize scale.

namespace yeti {

namespace transform {
  static bool compile(const component_compiler::Environment *env,
                      const component_compiler::Input *input,
                      const component_compiler::Output *output) {
    return false;
  }

  static void spawn(void *system,
                    const Entity *entities,
                    const u32 *owners,
                    size_t instances,
                    const void *data) {
  }

  static bool compatible(u32 version) {
    return false;
  }
}

static YETI_INLINE Transform::Handle handle_from_instance(const u32 instance) {
  return { instance };
}

static YETI_INLINE u32 instance_from_handle(const Transform::Handle handle) {
  return handle.instance;
}

TransformSystem::TransformSystem(EntityManager *entities)
  : entities_(entities)
  , entity_to_instance_(core::global_page_allocator(), entities->limit())
  , instance_to_entity_(core::global_page_allocator(), entities->limit())
  , parent_(core::global_page_allocator(), entities->limit())
  , local_poses_(core::global_page_allocator(), entities->limit())
  , world_poses_(core::global_page_allocator(), entities->limit())
  , dirty_(core::global_page_allocator(), entities->limit())
  , changed_(core::global_page_allocator(), entities->limit())
  , n_(0)
{
}

TransformSystem::~TransformSystem() {
}

Transform::Handle TransformSystem::create(Entity entity,
                                          const Vec3 &position,
                                          const Quaternion &rotation,
                                          const Vec3 &scale) {
  const u32 index = entity.index();

  yeti_assert_with_reason_debug(entity_to_instance_[index] == 0,
                                "Transform component already associated with entity.");

  const u32 instance = n_++;

  entity_to_instance_[index] = instance + 1;
  instance_to_entity_[instance] = entity;

  parent_[instance] = -1;

  local_poses_[instance] = Mat4::IDENTITY;
  world_poses_[instance] = Mat4::IDENTITY;

  dirty_[instance] = true;
  changed_[instance] = true;

  return handle_from_instance(instance);
}

void TransformSystem::destroy(Transform::Handle handle) {
  // TODO(mtwilliams): Defer destruction until next update as this prevents
  // handles from being invalidated (pointing to wrong instances) for the
  // duration of a frame.

  // Unlink all children otherwise they'll be linked to a random transform at
  // some point in the future.
  this->unlink_all_children(handle);

  const u32 dead = instance_from_handle(handle);
  const u32 replacement = n_ - 1;

  // Swap 'n' pop.
  entity_to_instance_[instance_to_entity_[replacement].index()] = dead;
  instance_to_entity_[dead] = instance_to_entity_[replacement];
  parent_[dead] = parent_[replacement];
  local_poses_[dead] = local_poses_[replacement];
  world_poses_[dead] = world_poses_[replacement];
  dirty_[dead] = dirty_[replacement];
  changed_[dead] = changed_[replacement];

  // Unmap.
  entity_to_instance_[instance_to_entity_[replacement].index()] = 0;
  entity_to_instance_[instance_to_entity_[dead].index()] = 0;

  n_ -= 1;
}

void TransformSystem::destroy(Entity entity) {
  if (const u32 instance = entity_to_instance_[entity.index()])
    // Has a transform component.
    this->destroy(handle_from_instance(instance - 1));
}

Transform::Handle TransformSystem::lookup(Entity entity) {
  const u32 instance = entity_to_instance_[entity.index()];
  return handle_from_instance(instance - 1);
}

bool TransformSystem::has(Entity entity) const {
  return (entity_to_instance_[entity.index()] != 0);
}

void TransformSystem::link(Transform::Handle child,
                           Transform::Handle parent,
                           const Vec3 &position,
                           const Quaternion &rotation,
                           const Vec3 &scale) {
  YETI_TRAP();
}

void TransformSystem::unlink(Transform::Handle handle) {
  YETI_TRAP();
}

Mat4 TransformSystem::get_local_pose(Transform::Handle handle) {
  return local_poses_[instance_from_handle(handle)];
}

Vec3 TransformSystem::get_local_position(Transform::Handle handle) {
  const Mat4 &pose = local_poses_[instance_from_handle(handle)];
  return translation_from_matrix(pose);
}

Quaternion TransformSystem::get_local_rotation(Transform::Handle handle) {
  const Mat4 &pose = local_poses_[instance_from_handle(handle)];
  return rotation_from_matrix(pose);
}

Vec3 TransformSystem::get_local_scale(Transform::Handle handle) {
  const Mat4 &pose = local_poses_[instance_from_handle(handle)];
  return scale_from_matrix(pose);
}

void TransformSystem::set_local_pose(Transform::Handle handle,
                                     const Vec3 &position,
                                     const Quaternion &rotation,
                                     const Vec3 &scale) {
  const Mat4 new_local_pose = Mat4::compose(position, rotation, scale);

  local_poses_[instance_from_handle(handle)] = new_local_pose;

  // Mark instance and descendants as dirty and changed.
  this->modified(handle);
}

void TransformSystem::set_local_position(Transform::Handle handle,
                                         const Vec3 &new_local_position) {
  Mat4 &pose = local_poses_[instance_from_handle(handle)];

  pose(0,3) = new_local_position.x;
  pose(1,3) = new_local_position.y;
  pose(2,3) = new_local_position.z;

  // Mark instance and descendants as dirty and changed.
  this->modified(handle);
}

// PERF(mtwilliams): Only modify rotation (scaling accordingly).

void TransformSystem::set_local_rotation(Transform::Handle handle,
                                         const Quaternion &new_local_rotation) {
  Mat4 &pose = local_poses_[instance_from_handle(handle)];

  // Derive existing position.
  const Vec3 old_local_position = translation_from_matrix(pose);

  // Derive existing scale.
  const Vec3 old_local_scale = scale_from_matrix(pose);

  // Build new local pose.
  pose = Mat4::compose(old_local_position, new_local_rotation, old_local_scale);

  // Mark instance and descendants as dirty and changed.
  this->modified(handle);
}

void TransformSystem::set_local_scale(Transform::Handle handle,
                                      const Vec3 &new_local_scale) {
  Mat4 &pose = local_poses_[instance_from_handle(handle)];

  // Derive existing scale.
  const Vec3 old_local_scale = scale_from_matrix(pose);

  // Calculate ratio between old and new.
  const Vec3 ratio = new_local_scale / old_local_scale;

  // Scale by ratio.
  pose(0,0) *= ratio.x;
  pose(0,1) *= ratio.y;
  pose(0,2) *= ratio.z;
  pose(1,0) *= ratio.x;
  pose(1,1) *= ratio.y;
  pose(1,2) *= ratio.z;
  pose(2,0) *= ratio.x;
  pose(2,1) *= ratio.y;
  pose(2,2) *= ratio.z;

  // Mark instance and descendants as dirty and changed.
  this->modified(handle);
}

Mat4 TransformSystem::get_world_pose(Transform::Handle handle) {
  return world_poses_[instance_from_handle(handle)];
}

void TransformSystem::modified(Transform::Handle handle) {
  const u32 instance = instance_from_handle(handle);

  // TODO(mtwilliams): Mark descendants.
  dirty_[instance] = true;
  changed_[instance] = true;
}

void TransformSystem::unlink_all_children(Transform::Handle handle) {
  // TODO(mtwilliams): Unlink all children.
}

void TransformSystem::update() {
  YETI_TRAP();
}

void TransformSystem::recompute(Transform::Handle handle) {
  YETI_TRAP();
}

void TransformSystem::changed(core::Array<Entity> &changed) const {
  for (unsigned instance = 0; instance < n_; ++instance)
    if (changed_[instance])
      changed.push(instance_to_entity_[instance]);
}

void TransformSystem::destroyed(Entity entity) {
  this->destroy(entity);
}

// Automatically register with component registry.
YETI_AUTO_REGISTER_COMPONENT(TransformSystem::component());

const Component *TransformSystem::component() {
  static const Component::Property properties[] = {
    { "position",      Component::Property::FLOAT,  3 },
    { "rotation",      Component::Property::FLOAT,  4 },
    { "scale",         Component::Property::FLOAT,  3 },
    { "parent",        Component::Property::ENTITY, 1 }
  };

  static const Component component = {
    /* .name              = */ "transform",
    /* .version           = */ 1,
    /* .order             = */ 0,
    /* .properties        = */ properties,
    /* .num_of_properties = */ YETI_ELEMENTS_IN_ARRAY(properties),
    /* .create_a_system   = */ &TransformSystem::create,
    /* .destroy_a_system  = */ &TransformSystem::destroy,
    /* .compile           = */ &transform::compile,
    /* .spawn             = */ &transform::spawn,
    /* .compatible        = */ &transform::compatible,
  };

  return &component;
}

void *TransformSystem::create(EntityManager *entities) {
  return YETI_NEW(TransformSystem, core::global_heap_allocator())(entities);
}

void TransformSystem::destroy(void *system) {
  YETI_DELETE(TransformSystem, core::global_heap_allocator(), (TransformSystem *)system);
}

} // yeti
