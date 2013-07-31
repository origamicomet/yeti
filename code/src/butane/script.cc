// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/script.h>

#include <butane/script_if.h>
#include <butane/application_if.h>
#include <butane/window_if.h>
#include <butane/viewport_if.h>
#include <butane/swap_chain_if.h>
#include <butane/world_if.h>

namespace butane {
  Script::Script()
    : L(lua_newstate((lua_Alloc)&Script::__alloc, (void*)this))
    , _memory_usage(0)
  {
    _on_error.handler = nullptr;
    _on_error.closure = nullptr;

    static const luaL_Reg libs[] = {
      { "", luaopen_base },
      { LUA_LOADLIBNAME, &luaopen_package },
      { LUA_TABLIBNAME, &luaopen_table },
      { LUA_STRLIBNAME, &luaopen_string },
      { LUA_MATHLIBNAME, &luaopen_math },
    #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_DEVELOPMENT_BUILD)
      { LUA_DBLIBNAME, &luaopen_debug },
    #endif
      { "Script", &luaopen_script },
      { "Application", &luaopen_application },
      { "Window", &luaopen_window },
      { "Viewport", &luaopen_viewport },
      { "SwapChain", &luaopen_swap_chain },
      { "World", &luaopen_world },
      { NULL, NULL }
    };

    for (const luaL_Reg* lib = &libs[0]; lib->func; ++lib) {
      luaL_requiref(L, lib->name, lib->func, 1);
      lua_pop(L, 1);
    }
  }

  Script::~Script()
  {
    if (L)
      lua_close(L);
  }

  void* Script::__alloc(
    Script* script,
    void* ptr,
    size_t old_size,
    size_t new_size )
  {
    script->_memory_usage -= old_size;
    script->_memory_usage += new_size;
    return Allocators::heap().realloc(ptr, new_size);
  }

  void* Script::__compiler_alloc(
    void* /* ud */,
    void* ptr,
    size_t old_size,
    size_t new_size )
  {
    return Allocators::heap().realloc(ptr, new_size);
  }

  int Script::__error_handler(
    lua_State* L )
  {
    const Script& script = *((Script*)lua_touserdata(L, lua_upvalueindex(1)));
    if (script._on_error.handler)
      script._on_error.handler(script, script._on_error.closure, lua_tostring(L, 1));
    return 0;
  }

  namespace {
    struct Writer {
      bool (*write)(
        void* closure,
        const void* bytes,
        size_t num_of_bytes );
      void* closure;
    };

    static int __compiler_writer(
      lua_State* state,
      const void* p,
      size_t sz,
      void* ud )
    {
      const Writer* writer = (const Writer*)ud;
      return (writer->write(writer->closure, p, sz) ? 0 : 1);
    }
  }

  bool Script::compile(
    const char* name,
    const char* code,
    const size_t code_len,
    bool (*write)(
      void* closure,
      const void* bytes,
      size_t num_of_bytes ),
    void (*error)(
      void* closure,
      const char* error ),
    void* closure )
  {
    assert(name != nullptr);
    assert(code != nullptr);
    assert(code_len != 0);
    assert(write != nullptr);
    assert(error != nullptr);

    lua_State* state = lua_newstate((lua_Alloc)&Script::__compiler_alloc, nullptr);

    if (luaL_loadbuffer(state, (const char*)code, code_len, name) != 0) {
      error(closure, lua_tostring(state, -1));
      lua_pop(state, 1);
      lua_close(state);
      return false; }

    Writer writer;
    writer.write = write;
    writer.closure = closure;

    if (lua_dump(state, &__compiler_writer, (void*)&writer)) {
      lua_close(state);
      return false; }

    lua_close(state);
    return true;
  }

  bool Script::load(
    const char* name,
    const void* code,
    const size_t code_len )
  {
    assert(name != nullptr);
    assert(code != nullptr);
    assert(code_len != 0);

    if (luaL_loadbuffer(L, (const char*)code, code_len, name) != 0) {
      // warn("luaL_loadbuffer failed:\n\n%s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
      return false; }

    lua_pushlightuserdata(L, (void*)this);
    lua_pushcclosure(L, &Script::__error_handler, 1);
    lua_insert(L, -2);

    if (lua_pcall(L, 0, LUA_MULTRET, -2) != 0)
      return false;

    return true;
  }

  int Script::call(
    const char* name,
    const int num_of_arguments )
  {
    assert(name != nullptr);
    assert(num_of_arguments >= 0);

    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
    lua_getfield(L, -1, name);
    lua_insert(L, -num_of_arguments - 1);
    lua_pop(L, 1);
    lua_pushlightuserdata(L, (void*)this);
    lua_pushcclosure(L, &Script::__error_handler, 1);
    lua_insert(L, -num_of_arguments - 2);

    const int top_prior_to_pcall = lua_gettop(L);
    if (lua_pcall(L, num_of_arguments, LUA_MULTRET, -num_of_arguments - 2) != 0) {
      lua_pop(L, 1);
      return -1; }

    lua_remove(L, -(lua_gettop(L) - top_prior_to_pcall) - 1);
    return (lua_gettop(L) - top_prior_to_pcall);
  }

  void Script::set_on_error_handler(
    OnErrorHandler handler,
    void* closure )
  {
    _on_error.handler = handler;
    _on_error.closure = closure;
  }
} // butane
