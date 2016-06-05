//===-- yeti/resources/script_resource.cc -----------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/resources/script_resource.h"

#include "yeti/resource.h"
#include "yeti/resource_compiler.h"

extern "C" {
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
  #include <luajit.h>
}

namespace yeti {

ScriptResource::ScriptResource(Resource::Id id)
  : Resource(id) {
}

ScriptResource::~ScriptResource() {
}

const Resource::Type *ScriptResource::type() {
  static const char *extensions[] = {"lua", (const char *)NULL};
  static const Resource::Type type = {
    /* .name = */ "script",
    /* .extensions = */ extensions,
    /* .prepare = */ &ScriptResource::prepare,
    /* .load = */ &ScriptResource::load,
    /* .unload = */ &ScriptResource::unload,
    /* .online = */ NULL,
    /* .offline = */ NULL,
    /* .compile = */ &ScriptResource::compile
  };

  return &type;
}

Resource *ScriptResource::prepare(Resource::Id id) {
  // TODO(mtwilliams): Use an arena allocator.
  return (Resource *)(new (foundation::heap()) ScriptResource(id));
}

void ScriptResource::load(Resource *resource, const Resource::Data &data) {
  ScriptResource *script_resource = (ScriptResource *)resource;

  foundation::fs::Info memory_resident_data_info;
  foundation::fs::info(data.memory_resident_data, &memory_resident_data_info);

  foundation::fs::read(data.memory_resident_data, (uintptr_t)&script_resource->path_[0], 256);

  script_resource->bytecode_len_ = memory_resident_data_info.size - 256;
  script_resource->bytecode_ = (const char *)foundation::heap().allocate(script_resource->bytecode_len_);
  foundation::fs::read(data.memory_resident_data, (uintptr_t)script_resource->bytecode_, script_resource->bytecode_len_);

  foundation::fs::close(data.memory_resident_data);
  foundation::fs::close(data.streaming_data);
}

void ScriptResource::unload(Resource *resource) {
  ScriptResource *script_resource = (ScriptResource *)resource;

  foundation::heap().deallocate((uintptr_t)script_resource->bytecode_);

  // BUG(mtwilliams): Potential memory leak?
  delete script_resource;
}

namespace {
  static int dump_to_file(lua_State *L, const void *chunk, size_t chunk_sz, foundation::fs::File *file) {
    // BUG(mtwilliams): Doesn't handle larger writes?
    foundation::fs::write(file, (uintptr_t)chunk, chunk_sz);
    return 0;
  }
}

void ScriptResource::compile(const resource_compiler::Input *input,
                              const resource_compiler::Output *output) {
  foundation::fs::Info src_file_info;
  foundation::fs::info(input->source, &src_file_info);

  // BUG(mtwilliams): Doesn't handle larger files?
  foundation::Array<u8> src(foundation::heap(), src_file_info.size + 1);
  foundation::fs::read(input->source, (uintptr_t)src.first(), src_file_info.size);
  src[src_file_info.size] = '\0';

  lua_State *L = luaL_newstate();
  if (luaL_loadbuffer(L, (const char *)src.first(), src_file_info.size, input->path) != 0)
    // TODO(mtwilliams): Properly track resource compilation.
    yeti_assertf_debug(0, "Compilation failed!\n%s", lua_tostring(L, -1));

  char path[256] = { 0, };
  strncpy(&path[0], input->path, 255);
  foundation::fs::write(output->memory_resident_data, (uintptr_t)&path[0], 256);
  if (lua_dump(L, (lua_Writer)&dump_to_file, (void *)output->memory_resident_data) != 0)
    yeti_assertf_debug(0, "Dumping to file failed!\n");

  lua_close(L);
}

} // yeti
