// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/script_if.h>
#include <butane/math.h>

namespace butane {
  namespace {
    struct lua_Temporary {
      union {
        uint8_t __Vec2f[sizeof(Vec2f)];
        uint8_t __Vec3f[sizeof(Vec3f)];
        uint8_t __Vec4f[sizeof(Vec4f)];
        uint8_t __Quatf[sizeof(Quatf)];
      };
    };

    static const int __lua_temporaries_key = 0;

    static int lua_temporaries__gc( lua_State* L ) {
      Array<lua_Temporary>& temporaries = *((Array<lua_Temporary>*)luaL_checkuserdata(L, 1));
      temporaries.~Array();
      return 0;
    }

    static lua_Temporary* lua_newtemporary( lua_State* L ) {
      lua_pushlightuserdata(L, (void*)&__lua_temporaries_key);
      lua_rawget(L, LUA_REGISTRYINDEX);
      Array<lua_Temporary>& temporaries = *((Array<lua_Temporary>*)luaL_checkuserdata(L, -1));
      lua_pop(L, 1);
      if (temporaries.size() == temporaries.reserved())
        luaL_error(L, "maximum number of temporaries reached");
      return (*((lua_Temporary**)lua_newuserdata(L, sizeof(lua_Temporary*))) =
        &temporaries[temporaries.size()]);
    }

    static lua_Temporary* lua_checktemporary( lua_State* L ) {
      return *((lua_Temporary**)luaL_checkuserdata(L, 1));
    }
  }

  namespace {
    static Vec2f* lua_newvec2( lua_State* L ) {
      Vec2f* v = ((Vec2f*)lua_newtemporary(L));
      luaL_getmetatable(L, "Vec2");
      lua_setmetatable(L, -2);
      return v;
    }

    static Vec2f* lua_checkvec2( lua_State* L, int idx ) {
      return *((Vec2f**)luaL_checkuserdata2(L, idx, "Vec2"));
    }

    static int lua_vec2__call( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 1: {
          *lua_newvec2(L) = Vec2f();
        } break;
        case 3:
          *lua_newvec2(L) = Vec2f(luaL_checknumber(L, 2), luaL_checknumber(L, 3));
          break;
        default:
          return luaL_error(L, "expected zero or two arguments"); }
      return 1;
    }

    static int lua_vec2__gc( lua_State* L ) {
      return 0;
    }

    static int lua_vec2__add( lua_State* L ) {
      const Vec2f& lhs = *lua_checkvec2(L, 1);
      const Vec2f& rhs = *lua_checkvec2(L, 2);
      *lua_newvec2(L) = lhs + rhs;
      return 1;
    }

    static int lua_vec2__sub( lua_State* L ) {
      const Vec2f& lhs = *lua_checkvec2(L, 1);
      const Vec2f& rhs = *lua_checkvec2(L, 2);
      *lua_newvec2(L) = lhs - rhs;
      return 1;
    }

    static int lua_vec2__mul( lua_State* L ) {
      const Vec2f& lhs = *lua_checkvec2(L, 1);
      const Vec2f& rhs = *lua_checkvec2(L, 2);
      *lua_newvec2(L) = lhs * rhs;
      return 1;
    }

    static int lua_vec2__div( lua_State* L ) {
      const Vec2f& lhs = *lua_checkvec2(L, 1);
      const Vec2f& rhs = *lua_checkvec2(L, 2);
      *lua_newvec2(L) = lhs / rhs;
      return 1;
    }

    static int lua_vec2_length( lua_State* L ) {
      const Vec2f& self = *lua_checkvec2(L, 1);
      lua_pushnumber(L, self.length());
      return 1;
    }

    static int lua_vec2_magnitude( lua_State* L ) {
      const Vec2f& self = *lua_checkvec2(L, 1);
      lua_pushnumber(L, self.magnitude());
      return 1;
    }

    static int lua_vec2_dot( lua_State* L ) {
      const Vec2f& lhs = *lua_checkvec2(L, 1);
      const Vec2f& rhs = *lua_checkvec2(L, 2);
      lua_pushnumber(L, lhs.dot(rhs));
      return 1;
    }

    static int lua_vec2_normalize( lua_State* L ) {
      const Vec2f& self = *lua_checkvec2(L, 1);
      *lua_newvec2(L) = self.normalize();
      return 1;
    }

    static int lua_vec2__index( lua_State* L ) {
      const Vec2f& self = *lua_checkvec2(L, 1);
      if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.x);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.y);
      else {
        luaL_getmetatable(L, "Vec2");
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        lua_remove(L, -2);
        return 1; }
      return 1;
    }

    static int lua_vec2__newindex( lua_State* L ) {
      Vec2f& self = *((Vec2f*)lua_checkvec2(L, 1));
      if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        self.x = luaL_checknumber(L, 3);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        self.y = luaL_checknumber(L, 3);
      return 0;
    }
  }

  namespace {
    static int lua_script_log( lua_State* L ) {
      const LogScope _("Script");
      log(luaL_checkstring(L, 1));
      return 0;
    }

    static int lua_script_warn( lua_State* L ) {
      const LogScope _("Script");
      warn(luaL_checkstring(L, 1));
      return 0;
    }

    static int lua_script_fail( lua_State* L ) {
      const LogScope _("Script");
      fail(luaL_checkstring(L, 1));
      return 0;
    }
  }
} // butane

namespace butane {
  int luaopen_script( lua_State* L ) {
    lua_pushlightuserdata(L, (void*)&__lua_temporaries_key);
    Array<lua_Temporary>* temporaries = new (lua_newuserdata(L, sizeof(Array<lua_Temporary>))) Array<lua_Temporary>(
      Allocators::heap(), BT_SCRIPT_MAXIMUM_NUM_OF_TEMPORARY_MATH_OBJECTS);
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, &lua_temporaries__gc);
    lua_setfield(L, -2, "__gc");
    lua_setmetatable(L, -2);
    lua_rawset(L, LUA_REGISTRYINDEX);

    lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS); {
      luaL_newmetatable(L, "Vec2");
      lua_pushcfunction(L, &lua_vec2__call);
      lua_setfield(L, -2, "__call");
      lua_pushcfunction(L, &lua_vec2__gc);
      lua_setfield(L, -2, "__gc");
      lua_pushcfunction(L, &lua_vec2__add);
      lua_setfield(L, -2, "__add");
      lua_pushcfunction(L, &lua_vec2__sub);
      lua_setfield(L, -2, "__sub");
      lua_pushcfunction(L, &lua_vec2__mul);
      lua_setfield(L, -2, "__mul");
      lua_pushcfunction(L, &lua_vec2__div);
      lua_setfield(L, -2, "__div");
      lua_pushcfunction(L, &lua_vec2_length);
      lua_setfield(L, -2, "length");
      lua_pushcfunction(L, &lua_vec2_magnitude);
      lua_setfield(L, -2, "magnitude");
      lua_pushcfunction(L, &lua_vec2_dot);
      lua_setfield(L, -2, "dot");
      lua_pushcfunction(L, &lua_vec2_normalize);
      lua_setfield(L, -2, "normalize");
      lua_pushcfunction(L, &lua_vec2__index);
      lua_setfield(L, -2, "__index");
      lua_pushcfunction(L, &lua_vec2__newindex);
      lua_setfield(L, -2, "__newindex");
      lua_pushvalue(L, -1);
      lua_setmetatable(L, -2);
      lua_setfield(L, -2, "Vec2");
    } lua_pop(L, 1);

    lua_createtable(L, 0, 3);
    lua_pushcfunction(L, &lua_script_log);
    lua_setfield(L, -2, "log");
    lua_pushcfunction(L, &lua_script_warn);
    lua_setfield(L, -2, "warn");
    lua_pushcfunction(L, &lua_script_fail);
    lua_setfield(L, -2, "fail");
    return 1;
  }
} // butane
