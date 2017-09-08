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
  , source_(foundation::heap())
  , bytecode_(foundation::heap())
{
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
  foundation::fs::read_into_buffer(input_->source, source_);

  // Compile but don't run.
  const int result = luaL_loadbuffer(lua_, (const char *)&source_[0], source_.size(), input_->path);

  // Report any errors.
  if (result == LUA_ERRSYNTAX)
    env_->error(env_, lua_tostring(lua_, -1));
  else if (result == LUA_ERRMEM)
    env_->error(env_, "Ran out of memory while compiling.");

  return (result == 0);
}

namespace {
  static int copy_to_memory(lua_State *L,
                            const void *chunk,
                            size_t size,
                            foundation::Array<u8> *array)
  {
    YETI_UNUSED(L);

    const size_t offset = array->size();

    array->resize(array->size() + size);

    foundation::memory::copy((uintptr_t)chunk,
                             (uintptr_t)&(*array)[offset],
                             size);

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

  foundation::fs::write(output_->memory_resident_data, (uintptr_t)&path[0], sizeof(path));
  foundation::fs::write(output_->memory_resident_data, (uintptr_t)&bytecode_[0], bytecode_.size());

  return true;
}

} // yeti
