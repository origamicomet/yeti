// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/viewport_if.h>
#include <butane/graphics/viewport.h>

namespace butane {
  namespace {
    static int lua_viewport_create( lua_State* L ) {
      float* viewport = (float*)Allocators::heap().alloc(sizeof(float[4]));
      viewport[0] = luaL_checknumber(L, 1);
      luaL_argcheck(L, ((viewport[0] >= -1.0f) && (viewport[0] <= 1.0f)), 1, "expected viewport bounds in normalized device coordinates");
      viewport[1] = luaL_checknumber(L, 2);
      luaL_argcheck(L, ((viewport[1] >= -1.0f) && (viewport[1] <= 1.0f)), 2, "expected viewport bounds in normalized device coordinates");
      viewport[2] = luaL_checknumber(L, 3);
      luaL_argcheck(L, ((viewport[2] >= -1.0f) && (viewport[2] <= 1.0f)), 3, "expected viewport bounds in normalized device coordinates");
      viewport[3] = luaL_checknumber(L, 4);
      luaL_argcheck(L, ((viewport[3] >= -1.0f) && (viewport[3] <= 1.0f)), 4, "expected viewport bounds in normalized device coordinates");
      lua_pushlightuserdata(L, (void*)viewport);
      return 1;
    }

    static int lua_viewport_destroy( lua_State* L ) {
      float* viewport = (float*)luaL_checklightuserdata(L, 0);
      Allocators::heap().free((void*)viewport);
      return 0;
    }

    static int lua_viewport_update( lua_State* L ) {
      float* viewport = (float*)luaL_checklightuserdata(L, 1);
      viewport[0] = luaL_checknumber(L, 2);
      luaL_argcheck(L, ((viewport[0] >= -1.0f) && (viewport[0] <= 1.0f)), 2, "expected viewport bounds in normalized device coordinates");
      viewport[1] = luaL_checknumber(L, 3);
      luaL_argcheck(L, ((viewport[1] >= -1.0f) && (viewport[1] <= 1.0f)), 3, "expected viewport bounds in normalized device coordinates");
      viewport[2] = luaL_checknumber(L, 4);
      luaL_argcheck(L, ((viewport[2] >= -1.0f) && (viewport[2] <= 1.0f)), 4, "expected viewport bounds in normalized device coordinates");
      viewport[3] = luaL_checknumber(L, 5);
      luaL_argcheck(L, ((viewport[3] >= -1.0f) && (viewport[3] <= 1.0f)), 5, "expected viewport bounds in normalized device coordinates");
      return 0;
    }
  }
} // butane

namespace butane {
  int luaopen_viewport( lua_State* L )
  {
    lua_createtable(L, 0, 3);
    lua_pushcfunction(L, &lua_viewport_create);
    lua_setfield(L, -2, "create");
    lua_pushcfunction(L, &lua_viewport_destroy);
    lua_setfield(L, -2, "destroy");
    lua_pushcfunction(L, &lua_viewport_update);
    lua_setfield(L, -2, "update");
    return 1;
  }
} // butane
