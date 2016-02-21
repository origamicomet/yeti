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

bool Script::call(const char *fn, u32 n, ...) {
  lua_pushlightuserdata(L, (void *)this);
  lua_pushcclosure(L, &Script::__error_handler, 1);

  lua_getglobal(L, fn);

  va_list ap;
  va_start(ap, n);

  for (u32 arg = 0; arg < n; ++arg) {
    const Type type = (Type)va_arg(ap, int);
    switch (type) {
      case T_NIL:      lua_pushnil(L); break;
      case T_BOOLEAN:  lua_pushboolean(L, va_arg(ap, int)); break;
      case T_INTEGER:  lua_pushnumber(L, va_arg(ap, int)); break;
      case T_FLOAT:    lua_pushnumber(L, va_arg(ap, double)); break;
      case T_STRING:   lua_pushstring(L, va_arg(ap, const char *)); break;
      case T_ARRAY:    yeti_assertf(0, "Not implemented yet."); break;
      case T_MAP:      yeti_assertf(0, "Not implemented yet."); break;
      case T_POINTER:  lua_pushlightuserdata(L, va_arg(ap, void *)); break;
      case T_FUNCTION: lua_pushcfunction(L, (lua_CFunction)va_arg(ap, void *)); break;
      default: {
        yeti_assertf(0, "Invalid or unknown type.");
      } break;
    }
  }

  va_end(ap);

  if (lua_pcall(L, n, 0, -(int)(n + 2)) != 0) {
    lua_pop(L, 1);
    return false;
  }

  return true;
}

} // yeti
