// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/lua.h>

extern "C" {
  void *luaL_checkuserdata(lua_State *L, int ud) {
    luaL_checktype(L, ud, LUA_TUSERDATA);
    return lua_touserdata(L, ud);
  }

  void *luaL_checkuserdata2(lua_State *L, int ud, const char* tname) {
    return luaL_checkudata(L, ud, tname);
  }

  void *luaL_checklightuserdata(lua_State *L, int lud) {
    luaL_checktype(L, lud, LUA_TLIGHTUSERDATA);
    return lua_touserdata(L, lud);
  }
}
