//===-- yeti/resources/entity_resource.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/resources/entity_resource.h"

#include "yeti/resource.h"
#include "yeti/resource_compiler.h"

// Compilation logic is factored into a seperate class.
#include "entity_compiler.h"

namespace yeti {

EntityResource::EntityResource(Resource::Id id)
  : Resource(id) {
}

EntityResource::~EntityResource() {
}

const Resource::Type *EntityResource::type() {
  static const char *extensions[] = {"entity", NULL};

  static const Resource::Type type = {
    /* .name = */ "entity",
    /* .extensions = */ extensions,
    /* .version = */ 0,
    /* .prepare = */ &EntityResource::prepare,
    /* .load = */ &EntityResource::load,
    /* .unload = */ &EntityResource::unload,
    /* .online = */ NULL,
    /* .offline = */ NULL,
    /* .compile = */ &EntityResource::compile,
    /* .compatible = */ &EntityResource::compatible,
    /* .lifecycle_preference = */ resource::LifecyclePreferences::INDIFFERENT
  };

  return &type;
}

Resource *EntityResource::prepare(Resource::Id id) {
  // TODO(mtwilliams): Use an arena allocator.
  return (Resource *)YETI_NEW(EntityResource, core::global_heap_allocator())(id);
}

void EntityResource::load(Resource *resource, const Resource::Data &data) {
}

void EntityResource::unload(Resource *resource) {
  EntityResource *entity_resource = (EntityResource *)resource;

  YETI_DELETE(EntityResource, core::global_heap_allocator(), entity_resource);
}

bool EntityResource::compile(const resource_compiler::Environment *env,
                             const resource_compiler::Input *input,
                             const resource_compiler::Output *output) {
  return EntityCompiler(env, input, output).run();
}

bool EntityResource::compatible(u32 version) {
  return false;
}

} // yeti
