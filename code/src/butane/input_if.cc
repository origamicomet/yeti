// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/input_if.h>
#include <butane/input.h>

#include <butane/script_if.h>

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
            return luaL_argerror(L, 1, "expected 'left', 'middle', or 'right'");
          break;
        default:
          return luaL_error(L, "expected a button id or name"); }
      return 1;
    }

    static int lua_mouse_axis( lua_State* L ) {
      switch (lua_type(L, 1)) {
        case LUA_TNUMBER:
          return luaL_error(L, "axis ids aren't supported yet");
        case LUA_TSTRING:
          if (strcmp("x", luaL_checkstring(L, 1)) == 0)
            *lua_newvec3(L) = Mouse::axis(Mouse::X_AXIS);
          else if (strcmp("y", luaL_checkstring(L, 1)) == 0)
            *lua_newvec3(L) = Mouse::axis(Mouse::Y_AXIS);
          else
            return luaL_argerror(L, 1, "expected 'x', or 'y'");
          break;
        default:
          return luaL_error(L, "expected a axis id or name"); }
      return 1;
    }
  }

  namespace {
    static int lua_keyboard_name( lua_State* L ) {
      lua_pushinteger(L, Keyboard::name(luaL_checkstring(L, 1)));
      return 1;
    }

    static int lua_keyboard_button( lua_State* L ) {
      switch (lua_type(L, 1)) {
        case LUA_TNUMBER:
          lua_pushboolean(L, Keyboard::button(luaL_checkinteger(L, 1)));
          break;
        case LUA_TSTRING: {
          const Keyboard::Button btn = Keyboard::name(luaL_checkstring(L, 1));
          luaL_argcheck(L, btn != 0xFFFFFFFFu, 1, "unknown button");
          lua_pushboolean(L, Keyboard::button(btn));
        } break;
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
    lua_pushcfunction(L, &lua_mouse_axis);
    lua_setfield(L, -2, "axis");
    return 1;
  }

  int luaopen_keyboard( lua_State* L )
  {
    lua_newtable(L);
    lua_pushcfunction(L, &lua_keyboard_name);
    lua_setfield(L, -2, "name");
    lua_pushcfunction(L, &lua_keyboard_button);
    lua_setfield(L, -2, "button");
    return 1;
  }
} // butane
