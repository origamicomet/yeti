//===-- yeti/components/light.cc ------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/components/light.h"

namespace yeti {

namespace light {
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

LightSystem::LightSystem(EntityManager *entities)
  : entities_(entities)
  , entity_to_instance_(core::global_heap_allocator(), 256)
  , instance_to_entity_(core::global_heap_allocator())
  , lights_(core::global_heap_allocator())
  , siblings_(core::global_heap_allocator())
{
}

LightSystem::~LightSystem() {
}

// Automatically register with component registry.
YETI_AUTO_REGISTER_COMPONENT(LightSystem::component());

const Component *LightSystem::component() {
  static const Component::Property properties[] = {
    { "type",          Component::Property::STRING,  1 },
    { "radius",        Component::Property::FLOAT,   1 },
    { "angle",         Component::Property::FLOAT,   1 },
    { "color",         Component::Property::FLOAT,   4 },
    { "intensity",     Component::Property::FLOAT,   1 },
    { "enabled",       Component::Property::BOOLEAN, 1 },
    { "disabled",      Component::Property::BOOLEAN, 1 },
    { "baked",         Component::Property::BOOLEAN, 1 },
    { "virtual",       Component::Property::BOOLEAN, 1 },
    { "shadows",       Component::Property::BOOLEAN, 1 },
    { "volumetric",    Component::Property::BOOLEAN, 1 }
  };

  static const Component component = {
    /* .name              = */ "light",
    /* .version           = */ 1,
    /* .order             = */ 0,
    /* .properties        = */ properties,
    /* .num_of_properties = */ YETI_ELEMENTS_IN_ARRAY(properties),
    /* .create_a_system   = */ &LightSystem::create,
    /* .destroy_a_system  = */ &LightSystem::destroy,
    /* .compile           = */ &light::compile,
    /* .spawn             = */ &light::spawn,
    /* .compatible        = */ &light::compatible
  };

  return &component;
}

void *LightSystem::create(EntityManager *entities) {
  return YETI_NEW(LightSystem, core::global_heap_allocator())(entities);
}

void LightSystem::destroy(void *system) {
  YETI_DELETE(LightSystem, core::global_heap_allocator(), (LightSystem *)system);
}

} // yeti
