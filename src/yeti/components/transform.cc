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

// TODO(mtwilliams): Use our own sort.
#include <algorithm>

#include "yeti/components/transform.h"

// TODO(mtwilliams): Quantize scale.

// PERF(mtwilliams): Sort or bucket transforms by depth. This will increase
// the complexity of managing transforms but will allow us to parallelize
// updates.

namespace yeti {

// TODO(mtwilliams): Error messages during compilation.
// TODO(mtwilliams): Contextualize by indicating location in source.

#pragma pack(push, 1)
  struct Transform::Compiled {
    f32 position[3];
    f32 rotation[4];
    f32 scale[3];
  };
#pragma pack(pop)


class TransformCompiler {
 public:
  TransformCompiler(const component_compiler::Environment *env,
                    const component_compiler::Input *input,
                    const component_compiler::Output *output);

  ~TransformCompiler();

 public:
  bool compile();

 private:
  bool parse();
  bool output();

 private:
  bool parse_a_vector(const xml_element_t *element,
                      f32 *vector,
                      unsigned order);

 private:
  const component_compiler::Environment *env_;
  const component_compiler::Input *input_;
  const component_compiler::Output *output_;

  // Tracks whether or not various properties have been specified.
  bool position_has_been_specified_,
       rotation_has_been_specified_,
       scale_has_been_specified_;

  // Intermediate storage of the transform we're compiling. Eventually written
  // out after compilation.
  Transform::Compiled transform_;
};

TransformCompiler::TransformCompiler(const component_compiler::Environment *env,
                                     const component_compiler::Input *input,
                                     const component_compiler::Output *output)
  : env_(env)
  , input_(input)
  , output_(output)
  , position_has_been_specified_(false)
  , rotation_has_been_specified_(false)
  , scale_has_been_specified_(false)
{
  core::memory::zero((void *)&transform_, sizeof(Transform::Compiled));
}

TransformCompiler::~TransformCompiler()
{
}

bool TransformCompiler::compile()
{
  return parse() && output();
}

bool TransformCompiler::parse()
{
  const xml_element_t *root = input_->root;

  // We parse the component in entirety so we can report as many issues as
  // possible to reduce frustration.
  unsigned errors = 0;

  for (const xml_element_t *e = root->children; e; e = e->sibling) {
    if (core::string::compare("position", e->name.s, e->name.l)) {
      if (position_has_been_specified_) {
        env_->error(env_, "Position has already been specified!");
        errors++;
      } else {
        errors = this->parse_a_vector(e, transform_.position, 3) ? 0 : 1;
        position_has_been_specified_ = true;
      }
    } else if (core::string::compare("rotation", e->name.s, e->name.l)) {
      if (rotation_has_been_specified_) {
        env_->error(env_, "Rotation has already been specified!");
        errors++;
      } else {
        errors += this->parse_a_vector(e, transform_.rotation, 4) ? 0 : 1;
        rotation_has_been_specified_ = true;
      }
    } else if (core::string::compare("scale", e->name.s, e->name.l)) {
      if (scale_has_been_specified_) {
        env_->error(env_, "Scale has already been specified!");
        errors++;
      } else {
        errors += this->parse_a_vector(e, transform_.scale, 3) ? 0 : 1;
        rotation_has_been_specified_ = true;
      }
    } else {
      env_->error(env_, "Transforms do not have a '%.*s' property.", e->name.l, e->name.s);
      errors++;
    }
  }

  return (errors == 0);
}

bool TransformCompiler::parse_a_vector(const xml_element_t *root,
                                       f32 *vector,
                                       unsigned order)
{
  u32 mask = 0;

  for (const xml_element_t *e = root->children; e; e = e->sibling) {
    i32 axis = -1;

    if (e->name.l == 1 && *e->name.s == 'x')               axis = 0;
    if (e->name.l == 1 && *e->name.s == 'y')               axis = 1;
    if (e->name.l == 1 && *e->name.s == 'z' && order >= 3) axis = 2;
    if (e->name.l == 1 && *e->name.s == 'w' && order >= 4) axis = 3;

    if (axis < 0) {
      static const char *expected[] = { "x or y", "x, y, or z", "x, y, z, or w" };
      env_->error(env_, "Expected %s not \"%.*s\" for %.*s.", expected[order-2], e->name.l, e->name.s, root->name.l, root->name.s);
      return false;
    }

    if (mask & (1 << axis)) {
      env_->error(env_, "You have already specified the %c-axis for %.*s.", "xyzw"[axis], e->name.l, e->name.s);
      return false;
    }

    if (sscanf(e->body.s, "%f", &vector[axis]) != 1) {
      env_->error(env_, "Expected a floating-point value but got \"%.*s\" for %c-axis of %.*s.", e->body.l, e->body.l, "xyzw"[axis], root->name.l, root->name.s);
      return false;
    }

    mask |= (1 << axis);
  }

  for (unsigned axis = 0; axis < order; ++axis)
    if (~mask & (1 << axis))
      env_->error(env_, "You must specify the %c-axis for %.*s.", "xyzw"[axis], root->name.l, root->name.s);

  if (order >= 2 && mask < 3)  return false;
  if (order >= 3 && mask < 7)  return false;
  if (order >= 4 && mask < 15) return false;

  return true;
}

bool TransformCompiler::output()
{
  output_->write(env_, (const void *)&transform_, sizeof(Transform::Compiled));
  return true;
}

namespace transform {
  static bool compile(const component_compiler::Environment *env,
                      const component_compiler::Input *input,
                      const component_compiler::Output *output)
  {
    return TransformCompiler(env, input, output).compile();
  }

  static void spawn(void *system,
                    const Entity *entities,
                    const u32 *owners,
                    size_t instances,
                    const void *data)
  {
  }

  static bool compatible(u32 version)
  {
    return false;
  }
}

TransformSystem::TransformSystem(EntityManager *entities)
  : System(TransformSystem::component())
  , entities_(entities)
  , n_(0)
  , limit_(entities->limit())
  , entity_to_instance_(core::global_page_allocator(), limit_)
  , instance_to_entity_(core::global_page_allocator(), limit_)
  , parent_(core::global_page_allocator(), limit_)
  , local_poses_(core::global_page_allocator(), limit_)
  , world_poses_(core::global_page_allocator(), limit_)
  , dirty_(core::global_page_allocator(), limit_)
  , changed_(core::global_page_allocator(), limit_)
  , dead_(core::global_heap_allocator())
{
  for (unsigned index = 0; index < limit_; ++index)
    // We use -1 to indicate that there is no instance associated with an entity.
    entity_to_instance_[index].index = -1;
}

TransformSystem::~TransformSystem() {
}

Transform::Handle TransformSystem::create(Entity entity,
                                          const Vec3 &position,
                                          const Quaternion &rotation,
                                          const Vec3 &scale) {
  // Enforce one-to-one mapping.
  yeti_assert_with_reason_debug(!this->has(entity), "Transform component already associated with entity.");

  const u32 instance = n_++;

  entity_to_instance_[entity.index()].index = instance;
  instance_to_entity_[instance] = entity;

  parent_[instance] = -1;

  local_poses_[instance] = Mat4::IDENTITY;
  world_poses_[instance] = Mat4::IDENTITY;

  dirty_[instance] = true;
  changed_[instance] = true;

  return { entity.index() };
}

void TransformSystem::destroy(Transform::Handle handle) {
  const Transform::Instance instance = resolve(handle);

  // Defer destruction until next update as this prevents instances from being
  // invalidated (pointing to wrong instances) for the duration of a frame.
  dead_.push(instance);

  // Unlink all children otherwise they'll be linked to a random transform at
  // some point in the future.
  this->unlink_all_children(instance);

  // Unmap.
  entity_to_instance_[handle.opaque] = { u32(-1) };
  instance_to_entity_[instance.index] = { 0xFFFFFFFF };
}

void TransformSystem::destroy(Entity entity) {
  const Transform::Handle handle = lookup(entity);
  const Transform::Instance instance = resolve(handle);

  if (instance.index != -1)
    // Has a transform component, so destroy it.
    this->destroy(handle);
}

void TransformSystem::link(Transform::Instance child,
                           Transform::Instance parent,
                           const Vec3 &position,
                           const Quaternion &rotation,
                           const Vec3 &scale) {
  YETI_TRAP();
}

void TransformSystem::unlink(Transform::Instance handle) {
  YETI_TRAP();
}

Mat4 TransformSystem::get_local_pose(Transform::Instance instance) {
  return local_poses_[instance.index];
}

Vec3 TransformSystem::get_local_position(Transform::Instance instance) {
  const Mat4 &pose = local_poses_[instance.index];
  return translation_from_matrix(pose);
}

Quaternion TransformSystem::get_local_rotation(Transform::Instance instance) {
  const Mat4 &pose = local_poses_[instance.index];
  return rotation_from_matrix(pose);
}

Vec3 TransformSystem::get_local_scale(Transform::Instance instance) {
  const Mat4 &pose = local_poses_[instance.index];
  return scale_from_matrix(pose);
}

void TransformSystem::set_local_pose(Transform::Instance instance,
                                     const Vec3 &position,
                                     const Quaternion &rotation,
                                     const Vec3 &scale) {
  const Mat4 new_local_pose = Mat4::compose(position, rotation, scale);

  local_poses_[instance.index] = new_local_pose;

  // Mark instance and descendants as dirty and changed.
  this->modified(instance);
}

void TransformSystem::set_local_position(Transform::Instance instance,
                                         const Vec3 &new_local_position) {
  Mat4 &pose = local_poses_[instance.index];

  pose(0,3) = new_local_position.x;
  pose(1,3) = new_local_position.y;
  pose(2,3) = new_local_position.z;

  // Mark instance and descendants as dirty and changed.
  this->modified(instance);
}

// PERF(mtwilliams): Only modify rotation (scaling accordingly).

void TransformSystem::set_local_rotation(Transform::Instance instance,
                                         const Quaternion &new_local_rotation) {
  Mat4 &pose = local_poses_[instance.index];

  // Derive existing position.
  const Vec3 old_local_position = translation_from_matrix(pose);

  // Derive existing scale.
  const Vec3 old_local_scale = scale_from_matrix(pose);

  // Build new local pose.
  pose = Mat4::compose(old_local_position, new_local_rotation, old_local_scale);

  // Mark instance and descendants as dirty and changed.
  this->modified(instance);
}

void TransformSystem::set_local_scale(Transform::Instance instance,
                                      const Vec3 &new_local_scale) {
  Mat4 &pose = local_poses_[instance.index];

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
  this->modified(instance);
}

Mat4 TransformSystem::get_world_pose(Transform::Instance instance) {
  return world_poses_[instance.index];
}

void TransformSystem::modified(Transform::Instance instance) {
  // TODO(mtwilliams): Mark descendants.
  dirty_[instance.index] = true;
  changed_[instance.index] = true;
}

void TransformSystem::unlink_all_children(Transform::Instance instance) {
  // TODO(mtwilliams): Unlink all children.
}

// TODO(mtwilliams): Ensure transforms are updated from lowest link depth to
// greatest.

void TransformSystem::update() {
  // Quick 'n' dirty linear walk through transforms.
  for (u32 index = 0; index < n_; ++index)
    if (dirty_[index])
      this->recompute({ index });

  // Blow away dead transforms.
  TransformSystem::gc();
}

void TransformSystem::recompute(Transform::Instance instance) {
  const u32 index = instance.index;

  if (parent_[index] != -1)
    // Linked instances are transformed by their parent.
    world_poses_[index] = world_poses_[parent_[index]]
                        * local_poses_[index];
  else
    // Otherwise local-pose is equivalent to world-pose.
    world_poses_[index] = local_poses_[index];

  // No longer dirty, of course.
  dirty_[instance.index] = false;
}

void TransformSystem::gc() {
  // Sort from highest to lowest so we can work backwards.
  std::sort(dead_.begin(), dead_.end(), [](const auto &a, const auto &b) { return a.index > b.index; });

  u32 swap;

  // Skip any dead instances occupying the tail end of our arrays.
  for (swap = 0; swap < dead_.size(); swap++)
    if (dead_[swap].index != n_ - swap - 1)
      break;

  // Swap the last alive instance with remaining dead instances.
  for (Transform::Instance *dead = &dead_[swap]; dead < dead_.end(); ++dead) {
    // Identify the last alive instance.
    const Entity replacement = instance_to_entity_[swap];

    // Remap.
    entity_to_instance_[replacement.index()] = { dead->index };
    instance_to_entity_[dead->index] = replacement;

    // Replace parent and poses but not flags as we collect after recomputing
    // poses.
    parent_[dead->index] = parent_[swap];
    local_poses_[dead->index] = local_poses_[swap];
    world_poses_[dead->index] = world_poses_[swap];

    swap--;
  }

  // Now "pop" the dead instances that are all at the end of our arrays.
  n_ -= dead_.size();

  // Clear our queue for the next frame.
  dead_.clear();
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
