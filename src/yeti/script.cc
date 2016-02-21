//===-- yeti/script.cc ------------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script.h"

namespace yeti {

Script::Script() {
  L = lua_newstate((lua_Alloc)&Script::__alloc, (void *)this);
}

Script::~Script() {
  lua_close(L);
}

void *Script::__alloc(Script *script, void *ptr, size_t old_sz, size_t new_sz) {
  // TODO(mtwilliams): Track memory usage.
  return (void *)foundation::heap().reallocate((uintptr_t)ptr, new_sz);
}

int Script::__error_handler(lua_State *L) {
  Script *script = (Script *)lua_touserdata(L, lua_upvalueindex(1));
  // TODO(mtwilliams): Forward to a user-defined error handler.
  // TODO(mtwilliams): Use lua_Debug to build a Lua & C/C++ callstack.
  YETI_TRAP();
  return 0;
}

void Script::add_module(const char *module) {
  // TODO(mtwilliams): Support Lua 5.2 by using LUA_REGISTRYINDEX and LUA_RIDX_GLOBALS.
  lua_newtable(L);
  lua_setglobal(L, module);
}

void Script::add_module_function(const char *module,
                                 const char *name,
                                 const lua_CFunction fn) {
  // TODO(mtwilliams): Support Lua 5.2 by using LUA_REGISTRYINDEX and LUA_RIDX_GLOBALS.
  lua_getglobal(L, module);
  lua_pushcfunction(L, fn);
  lua_setfield(L, -2, name);
  lua_pop(L, 1);
}

} // yeti
