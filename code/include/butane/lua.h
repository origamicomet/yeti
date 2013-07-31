// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_LUA_H_
#define _BUTANE_LUA_H_

#include <butane/butane.h>

extern "C" {
  #include <lua.h>
  #include <lualib.h>
  #include <lauxlib.h>

  extern BUTANE_EXPORT void *luaL_checkuserdata(lua_State *L, int ud);
  extern BUTANE_EXPORT void *luaL_checkuserdata2(lua_State *L, int ud, const char* tname);
  extern BUTANE_EXPORT void *luaL_checklightuserdata(lua_State *L, int lud);
}

#endif // _BUTANE_LUA_H_
