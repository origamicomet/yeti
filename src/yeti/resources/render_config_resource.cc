//===-- yeti/resources/render_config_resource.cc --------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/resources/render_config_resource.h"

// Compilation logic is factored into a seperate class due to its complexity.
#include "render_config_compiler.h"

namespace yeti {

RenderConfigResource::RenderConfigResource(Resource::Id id)
  : Resource(id)
  , memory_resident_data_(NULL) {
}

RenderConfigResource::~RenderConfigResource() {
}

const Resource::Type *RenderConfigResource::type() {
  static const char *extensions[] = {"render_config", NULL};

  static const Resource::Type type = {
    /* .name = */ "render_config",
    /* .extensions = */ extensions,
    /* .prepare = */ &RenderConfigResource::prepare,
    /* .load = */ &RenderConfigResource::load,
    /* .unload = */ &RenderConfigResource::unload,
    /* .online = */ NULL,
    /* .offline = */ NULL,
    /* .compile = */ &RenderConfigResource::compile,
    /* .lifecycle_preference */ resource::LifecyclePreferences::INDIFFERENT
  };

  return &type;
}

Resource *RenderConfigResource::prepare(Resource::Id id) {
  // TODO(mtwilliams): Use an arena allocator.
  return (Resource *)YETI_NEW(RenderConfigResource, core::global_heap_allocator())(id);
}

void RenderConfigResource::load(Resource *resource, const Resource::Data &data) {
  RenderConfigResource *render_config_resource = (RenderConfigResource *)resource;

  core::File::Info memory_resident_data_info;
  core::fs::info(data.memory_resident_data, &memory_resident_data_info);

  render_config_resource->memory_resident_data_ =
    core::global_heap_allocator().allocate(memory_resident_data_info.size);

  core::fs::read(
    data.memory_resident_data,
    render_config_resource->memory_resident_data_,
    memory_resident_data_info.size);

  core::fs::close(data.memory_resident_data);
  core::fs::close(data.streaming_data);

  const render_config_format::Header *header =
    (const render_config_format::Header *)
      render_config_resource->memory_resident_data_;

  const uintptr_t base_ptr             = (uintptr_t)render_config_resource->memory_resident_data_;

  const uintptr_t variables_ptr        = base_ptr + header->variables_offset;
  const uintptr_t global_resources_ptr = base_ptr + header->global_resources_offset;
  const uintptr_t local_resources_ptr  = base_ptr + header->local_resources_offset;
  const uintptr_t generators_ptr       = base_ptr + header->generators_offset;
  const uintptr_t modifiers_ptr        = base_ptr + header->modifiers_offset;
  const uintptr_t layers_ptr           = base_ptr + header->layers_offset;

  // ...
}

void RenderConfigResource::unload(Resource *resource) {
  RenderConfigResource *render_config_resource = (RenderConfigResource *)resource;

  core::global_heap_allocator().deallocate(render_config_resource->memory_resident_data_);

  YETI_DELETE(RenderConfigResource, core::global_heap_allocator(), render_config_resource);
}

bool RenderConfigResource::compile(const resource_compiler::Environment *env,
                                   const resource_compiler::Input *input,
                                   const resource_compiler::Output *output) {
  return RenderConfigCompiler(env, input, output).run();
}

} // yeti
