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
}

TransformSystem::~TransformSystem() {
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
