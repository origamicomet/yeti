//===-- yeti/script/bindings/math_if.cc -----------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/math_if.h"

#include "yeti/script.h"

#include "yeti/math.h"

namespace yeti {

namespace math_if {
  namespace {
    static int vec2(lua_State *L) {
      Vec2 *v = Script::recover(L)->environment()->allocate<Vec2>();

      switch (lua_gettop(L) - 1) {
        default:
          return luaL_error(L, "Vec2 expects x/y to be passed as a table or via arguments.");

        case 1: {
          lua_getfield(L, 2, "x");
          lua_getfield(L, 2, "y");

        #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
            YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
          if (!lua_isnumber(L, -2))
            return luaL_argerror(L, 2, "Expected `x` to be a number.");
          if (!lua_isnumber(L, -1))
            return luaL_argerror(L, 2, "Expected `y` to be a number.");
        #endif

          v->x = (f32)lua_tonumber(L, -2);
          v->y = (f32)lua_tonumber(L, -1);
        } break;

        case 2: {
        #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
            YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
          if (!lua_isnumber(L, 2))
            return luaL_argerror(L, 2, "Expected `x` to be a number.");
          if (!lua_isnumber(L, 3))
            return luaL_argerror(L, 3, "Expected `y` to be a number.");
        #endif

          v->x = (f32)lua_tonumber(L, 2);
          v->y = (f32)lua_tonumber(L, 3);
        } break;
      }

      lua_pushlightuserdata(L, (void *)v);

      return 1;
    }

    static int vec2_x(lua_State *L) {
      if (!lua_islightuserdata(L, 1))
        return luaL_argerror(L, 1, "Expected a `Vec2`.");
      lua_pushnumber(L, ((const Vec2 *)lua_touserdata(L, 1))->x);
      return 1;
    }

    static int vec2_y(lua_State *L) {
      if (!lua_islightuserdata(L, 1))
        return luaL_argerror(L, 1, "Expected a `Vec2`.");
      lua_pushnumber(L, ((const Vec2 *)lua_touserdata(L, 1))->y);
      return 1;
    }

    static int vec3(lua_State *L) {
      Vec3 *v = Script::recover(L)->environment()->allocate<Vec3>();

      switch (lua_gettop(L) - 1) {
        default:
          return luaL_error(L, "Vec3 expects x/y/z to be passed as a table or via arguments.");

        case 1: {
          lua_getfield(L, 2, "x");
          lua_getfield(L, 2, "y");
          lua_getfield(L, 2, "z");

        #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
            YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
          if (!lua_isnumber(L, -3))
            return luaL_argerror(L, 2, "Expected `x` to be a number.");
          if (!lua_isnumber(L, -2))
            return luaL_argerror(L, 2, "Expected `y` to be a number.");
          if (!lua_isnumber(L, -1))
            return luaL_argerror(L, 2, "Expected `z` to be a number.");
        #endif

          v->x = (f32)lua_tonumber(L, -3);
          v->y = (f32)lua_tonumber(L, -2);
          v->z = (f32)lua_tonumber(L, -1);
        } break;

        case 3: {
        #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
            YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
          if (!lua_isnumber(L, 2))
            return luaL_argerror(L, 2, "Expected `x` to be a number.");
          if (!lua_isnumber(L, 3))
            return luaL_argerror(L, 3, "Expected `y` to be a number.");
          if (!lua_isnumber(L, 4))
            return luaL_argerror(L, 4, "Expected `z` to be a number.");
        #endif

          v->x = (f32)lua_tonumber(L, 2);
          v->y = (f32)lua_tonumber(L, 3);
          v->z = (f32)lua_tonumber(L, 4);
        } break;
      }

      lua_pushlightuserdata(L, (void *)v);

      return 1;
    }
  }

  static int vec3_x(lua_State *L) {
    if (!lua_islightuserdata(L, 1))
      return luaL_argerror(L, 1, "Expected a `Vec3`.");
    lua_pushnumber(L, ((const Vec3 *)lua_touserdata(L, 1))->x);
    return 1;
  }

  static int vec3_y(lua_State *L) {
    if (!lua_islightuserdata(L, 1))
      return luaL_argerror(L, 1, "Expected a `Vec3`.");
    lua_pushnumber(L, ((const Vec3 *)lua_touserdata(L, 1))->y);
    return 1;
  }

  static int vec3_z(lua_State *L) {
    if (!lua_islightuserdata(L, 1))
      return luaL_argerror(L, 1, "Expected a `Vec3`.");
    lua_pushnumber(L, ((const Vec3 *)lua_touserdata(L, 1))->z);
    return 1;
  }
} // math_if

void math_if::expose(Script *script) {
  script->add_module("Vec2");
  script->add_module_constructor("Vec2", &vec2);

  script->add_module_function("Vec2", "x", &vec2_x);
  script->add_module_function("Vec2", "y", &vec2_y);

  script->add_module("Vec3");
  script->add_module_constructor("Vec3", &vec3);

  script->add_module_function("Vec3", "x", &vec3_x);
  script->add_module_function("Vec3", "y", &vec3_y);
  script->add_module_function("Vec3", "z", &vec3_z);
}

} // yeti
