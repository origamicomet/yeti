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

namespace yeti {

namespace {
  // Shim that destroys transforms associated with destroyed entities.
  static void entity_destroyed_callback_shim(Entity entity, void *context) {
    ((TransformSystem *)context)->destroy(entity);
  }
}

TransformSystem::TransformSystem(EntityManager *entities)
  : entities_(entities)
  , entity_to_instance_(core::global_heap_allocator())
  , instance_to_entity_(core::global_heap_allocator())
  , parent_(core::global_heap_allocator())
  , local_poses_(core::global_heap_allocator())
  , world_poses_(core::global_heap_allocator())
  , dirty_(core::global_heap_allocator())
  , changed_(core::global_heap_allocator())
{
  // entities_->register_destroy_callback(&entity_destroyed_callback_shim, (void *)this);
}

TransformSystem::~TransformSystem() {
  // entities_->unregister_destroy_callback((void *)this);
}

const Component *TransformSystem::component() {
  static const Component component = {
    /* .name = */ "transform",
    /* .version = */ 1,
    /* .order = */ 1,
    /* .create_a_system = */ &TransformSystem::create,
    /* .destroy_a_system = */ &TransformSystem::destroy,
    /* .spawn = */ &TransformSystem::spawn,
    /* .compile = */ &TransformSystem::compile,
    /* .compatible = */ &TransformSystem::compatible
  };

  return &component;
}

void *TransformSystem::create(EntityManager *entities) {
  return YETI_NEW(TransformSystem, core::global_heap_allocator())(entities);
}

void TransformSystem::destroy(void *system) {
  YETI_DELETE(TransformSystem, core::global_heap_allocator(), (TransformSystem *)system);
}

void TransformSystem::spawn(void *system,
                            const Entity *entities,
                            const u32 *owners,
                            size_t instances,
                            const void *data) {
}

bool TransformSystem::compile(const component_compiler::Environment *env,
                              const component_compiler::Input *input,
                              const component_compiler::Output *output) {
  return false;
}

bool TransformSystem::compatible(u32 version) {
  return false;
}

} // yeti
