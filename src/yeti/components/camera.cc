//===-- yeti/components/camera.cc -----------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/components/camera.h"

namespace yeti {

namespace camera {
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

CameraSystem::CameraSystem(EntityManager *entities)
  : entities_(entities)
  , entity_to_instance_(core::global_heap_allocator(), 256)
  , instance_to_entity_(core::global_heap_allocator())
  , cameras_(core::global_heap_allocator())
  , siblings_(core::global_heap_allocator())
{
}

CameraSystem::~CameraSystem() {
}

// Automatically register with component registry.
YETI_AUTO_REGISTER_COMPONENT(CameraSystem::component());

const Component *CameraSystem::component() {
  static const Component::Property properties[] = {
    { "type"      ,    Component::Property::STRING, 1 },
    { "field_of_view", Component::Property::FLOAT,  1 },
    { "box",           Component::Property::FLOAT,  4 },
    { "near",          Component::Property::FLOAT,  1 },
    { "far",           Component::Property::FLOAT,  1 }
  };

  static const Component component = {
    /* .name              = */ "camera",
    /* .version           = */ 1,
    /* .order             = */ 0,
    /* .properties        = */ properties,
    /* .num_of_properties = */ YETI_ELEMENTS_IN_ARRAY(properties),
    /* .create_a_system   = */ &CameraSystem::create,
    /* .destroy_a_system  = */ &CameraSystem::destroy,
    /* .compile           = */ &camera::compile,
    /* .spawn             = */ &camera::spawn,
    /* .compatible        = */ &camera::compatible
  };

  return &component;
}

void *CameraSystem::create(EntityManager *entities) {
  return YETI_NEW(CameraSystem, core::global_heap_allocator())(entities);
}

void CameraSystem::destroy(void *system) {
  YETI_DELETE(CameraSystem, core::global_heap_allocator(), (CameraSystem *)system);
}

} // yeti
