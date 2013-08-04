// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/input_if.h>
#include <butane/input.h>

namespace butane {
  namespace {
    static int lua_mouse_button( lua_State* L ) {
      switch (lua_type(L, 1)) {
        case LUA_TNUMBER:
          return luaL_error(L, "button ids aren't supported yet");
        case LUA_TSTRING:
          if (strcmp("left", luaL_checkstring(L, 1)) == 0)
            lua_pushboolean(L, Mouse::button(Mouse::LEFT_BUTTON));
          else if (strcmp("middle", luaL_checkstring(L, 1)) == 0)
            lua_pushboolean(L, Mouse::button(Mouse::MIDDLE_BUTTON));
          else if (strcmp("right", luaL_checkstring(L, 1)) == 0)
            lua_pushboolean(L, Mouse::button(Mouse::RIGHT_BUTTON));
          else
            luaL_argerror(L, 1, "expected 'left', 'middle', or 'right'");
          break;
        default:
          return luaL_error(L, "expected a button id or name"); }
      return 1;
    }
  }
} // butane

namespace butane {
  int luaopen_mouse( lua_State* L )
  {
    lua_newtable(L);
    lua_pushcfunction(L, &lua_mouse_button);
    lua_setfield(L, -2, "button");
    return 1;
  }

  int luaopen_keyboard( lua_State* L )
  {
    lua_newtable(L);
    return 1;
  }
} // butane
