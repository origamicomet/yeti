//===-- yeti/resources/script_compiler.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "script_compiler.h"

namespace yeti {

ScriptCompiler::ScriptCompiler(const resource_compiler::Environment *env,
                               const resource_compiler::Input *input,
                               const resource_compiler::Output *output)
  : env_(env)
  , input_(input)
  , output_(output)
  , lua_(luaL_newstate())
  , source_(core::global_heap_allocator())
  , bytecode_(core::global_heap_allocator())
{
  yeti_assert_debug(env_ != NULL);
  yeti_assert_debug(input_ != NULL);
  yeti_assert_debug(output_ != NULL);
}

ScriptCompiler::~ScriptCompiler() {
  lua_close(lua_);
}

bool ScriptCompiler::run() {
  return this->compile()
      && this->dump();
}

bool ScriptCompiler::compile() {
  // Load source code into memory.
  core::fs::read_into_buffer(input_->source, source_);

  // Compile but don't run.
  const int result = !this->empty() ? luaL_loadbuffer(lua_, (const char *)&source_[0], source_.size(), input_->path)
                                    : luaL_loadbuffer(lua_, "", 0, input_->path);

  // Report any errors.
  if (result == LUA_ERRSYNTAX)
    env_->error(env_, lua_tostring(lua_, -1));
  else if (result == LUA_ERRMEM)
    env_->error(env_, "Ran out of memory while compiling.");
  else if (result != 0)
    env_->error(env_, "Something went horribly wrong!");

  return (result == 0);
}

bool ScriptCompiler::empty() const {
  return (source_.size() == 0);
}

namespace {
  static int copy_to_memory(lua_State *L,
                            const void *chunk,
                            size_t size,
                            core::Array<u8> *array)
  {
    YETI_UNUSED(L);

    const size_t offset = array->size();
    array->resize(array->size() + size);
    core::memory::copy(chunk, (void *)&(*array)[offset], size);

    return 0;
  }
}

bool ScriptCompiler::dump() {
  // Dump bytecode to memory.
  lua_dump(lua_, (lua_Writer)&copy_to_memory, (void *)&bytecode_);

  // Prefix with path so we can name the chunk on load.
  char path[256];
  strncpy(&path[0], input_->path, 255);
  path[255] = '\0';

  core::fs::write(output_->memory_resident_data, (void *)&path[0], sizeof(path));
  core::fs::write(output_->memory_resident_data, (void *)&bytecode_[0], bytecode_.size());

  return true;
}

} // yeti
