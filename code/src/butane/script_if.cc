// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/script_if.h>

#include <butane/math.h>
#include <butane/resource.h>
#include <butane/resources/script.h>

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
    static Vec3f* lua_newvec3( lua_State* L ) {
      Vec3f* v = ((Vec3f*)lua_newtemporary(L));
      luaL_getmetatable(L, "Vec3");
      lua_setmetatable(L, -2);
      return v;
    }

    static Vec3f* lua_checkvec3( lua_State* L, int idx ) {
      return *((Vec3f**)luaL_checkuserdata2(L, idx, "Vec3"));
    }

    static int lua_vec3__call( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 1: {
          *lua_newvec3(L) = Vec3f();
        } break;
        case 4:
          *lua_newvec3(L) = Vec3f(
            luaL_checknumber(L, 2),
            luaL_checknumber(L, 3),
            luaL_checknumber(L, 4));
          break;
        default:
          return luaL_error(L, "expected zero or three arguments"); }
      return 1;
    }

    static int lua_vec3__gc( lua_State* L ) {
      return 0;
    }

    static int lua_vec3__add( lua_State* L ) {
      const Vec3f& lhs = *lua_checkvec3(L, 1);
      const Vec3f& rhs = *lua_checkvec3(L, 2);
      *lua_newvec3(L) = lhs + rhs;
      return 1;
    }

    static int lua_vec3__sub( lua_State* L ) {
      const Vec3f& lhs = *lua_checkvec3(L, 1);
      const Vec3f& rhs = *lua_checkvec3(L, 2);
      *lua_newvec3(L) = lhs - rhs;
      return 1;
    }

    static int lua_vec3__mul( lua_State* L ) {
      const Vec3f& lhs = *lua_checkvec3(L, 1);
      const Vec3f& rhs = *lua_checkvec3(L, 2);
      *lua_newvec3(L) = lhs * rhs;
      return 1;
    }

    static int lua_vec3__div( lua_State* L ) {
      const Vec3f& lhs = *lua_checkvec3(L, 1);
      const Vec3f& rhs = *lua_checkvec3(L, 2);
      *lua_newvec3(L) = lhs / rhs;
      return 1;
    }

    static int lua_vec3_length( lua_State* L ) {
      const Vec3f& self = *lua_checkvec3(L, 1);
      lua_pushnumber(L, self.length());
      return 1;
    }

    static int lua_vec3_magnitude( lua_State* L ) {
      const Vec3f& self = *lua_checkvec3(L, 1);
      lua_pushnumber(L, self.magnitude());
      return 1;
    }

    static int lua_vec3_dot( lua_State* L ) {
      const Vec3f& lhs = *lua_checkvec3(L, 1);
      const Vec3f& rhs = *lua_checkvec3(L, 2);
      lua_pushnumber(L, lhs.dot(rhs));
      return 1;
    }

    static int lua_vec3_cross( lua_State* L ) {
      const Vec3f& lhs = *lua_checkvec3(L, 1);
      const Vec3f& rhs = *lua_checkvec3(L, 2);
      *lua_newvec3(L) = lhs.cross(rhs);
      return 1;
    }

    static int lua_vec3_normalize( lua_State* L ) {
      const Vec3f& self = *lua_checkvec3(L, 1);
      *lua_newvec3(L) = self.normalize();
      return 1;
    }

    static int lua_vec3__index( lua_State* L ) {
      const Vec3f& self = *lua_checkvec3(L, 1);
      if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.x);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.y);
      else if (strcmp("z", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.z);
      else {
        luaL_getmetatable(L, "Vec3");
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        lua_remove(L, -2);
        return 1; }
      return 1;
    }

    static int lua_vec3__newindex( lua_State* L ) {
      Vec3f& self = *((Vec3f*)lua_checkvec3(L, 1));
      if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        self.x = luaL_checknumber(L, 3);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        self.y = luaL_checknumber(L, 3);
      else if (strcmp("z", luaL_checkstring(L, 2)) == 0)
        self.z = luaL_checknumber(L, 3);
      return 0;
    }
  }

  namespace {
    static Vec4f* lua_newvec4( lua_State* L ) {
      Vec4f* v = ((Vec4f*)lua_newtemporary(L));
      luaL_getmetatable(L, "Vec4");
      lua_setmetatable(L, -2);
      return v;
    }

    static Vec4f* lua_checkvec4( lua_State* L, int idx ) {
      return *((Vec4f**)luaL_checkuserdata2(L, idx, "Vec4"));
    }

    static int lua_vec4__call( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 1: {
          *lua_newvec4(L) = Vec4f();
        } break;
        case 5:
          *lua_newvec4(L) = Vec4f(
            luaL_checknumber(L, 2),
            luaL_checknumber(L, 3),
            luaL_checknumber(L, 4),
            luaL_checknumber(L, 5));
          break;
        default:
          return luaL_error(L, "expected zero or four arguments"); }
      return 1;
    }

    static int lua_vec4__gc( lua_State* L ) {
      return 0;
    }

    static int lua_vec4__add( lua_State* L ) {
      const Vec4f& lhs = *lua_checkvec4(L, 1);
      const Vec4f& rhs = *lua_checkvec4(L, 2);
      *lua_newvec4(L) = lhs + rhs;
      return 1;
    }

    static int lua_vec4__sub( lua_State* L ) {
      const Vec4f& lhs = *lua_checkvec4(L, 1);
      const Vec4f& rhs = *lua_checkvec4(L, 2);
      *lua_newvec4(L) = lhs - rhs;
      return 1;
    }

    static int lua_vec4__mul( lua_State* L ) {
      const Vec4f& lhs = *lua_checkvec4(L, 1);
      const Vec4f& rhs = *lua_checkvec4(L, 2);
      *lua_newvec4(L) = lhs * rhs;
      return 1;
    }

    static int lua_vec4__div( lua_State* L ) {
      const Vec4f& lhs = *lua_checkvec4(L, 1);
      const Vec4f& rhs = *lua_checkvec4(L, 2);
      *lua_newvec4(L) = lhs / rhs;
      return 1;
    }

    static int lua_vec4_length( lua_State* L ) {
      const Vec4f& self = *lua_checkvec4(L, 1);
      lua_pushnumber(L, self.length());
      return 1;
    }

    static int lua_vec4_magnitude( lua_State* L ) {
      const Vec4f& self = *lua_checkvec4(L, 1);
      lua_pushnumber(L, self.magnitude());
      return 1;
    }

    static int lua_vec4_dot( lua_State* L ) {
      const Vec4f& lhs = *lua_checkvec4(L, 1);
      const Vec4f& rhs = *lua_checkvec4(L, 2);
      lua_pushnumber(L, lhs.dot(rhs));
      return 1;
    }

    static int lua_vec4_normalize( lua_State* L ) {
      const Vec4f& self = *lua_checkvec4(L, 1);
      *lua_newvec4(L) = self.normalize();
      return 1;
    }

    static int lua_vec4__index( lua_State* L ) {
      const Vec4f& self = *lua_checkvec4(L, 1);
      if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.x);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.y);
      else if (strcmp("z", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.z);
      else if (strcmp("w", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.w);
      else {
        luaL_getmetatable(L, "Vec4");
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        lua_remove(L, -2);
        return 1; }
      return 1;
    }

    static int lua_vec4__newindex( lua_State* L ) {
      Vec4f& self = *((Vec4f*)lua_checkvec4(L, 1));
      if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        self.x = luaL_checknumber(L, 3);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        self.y = luaL_checknumber(L, 3);
      else if (strcmp("z", luaL_checkstring(L, 2)) == 0)
        self.z = luaL_checknumber(L, 3);
      else if (strcmp("w", luaL_checkstring(L, 2)) == 0)
        self.w = luaL_checknumber(L, 3);
      return 0;
    }
  }

  namespace {
    static Quatf* lua_newquat( lua_State* L ) {
      Quatf* v = ((Quatf*)lua_newtemporary(L));
      luaL_getmetatable(L, "Quat");
      lua_setmetatable(L, -2);
      return v;
    }

    static Quatf* lua_checkquat( lua_State* L, int idx ) {
      return *((Quatf**)luaL_checkuserdata2(L, idx, "Quat"));
    }

    static int lua_quat__call( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 1: {
          *lua_newquat(L) = Quatf();
        } break;
        case 5:
          *lua_newquat(L) = Quatf(
            luaL_checknumber(L, 2),
            luaL_checknumber(L, 3),
            luaL_checknumber(L, 4),
            luaL_checknumber(L, 5));
          break;
        default:
          return luaL_error(L, "expected zero or four arguments"); }
      return 1;
    }

    static int lua_quat_from_axis_angle( lua_State* L ) {
      *lua_newquat(L) = Quatf::from_axis_angle(
        *lua_checkvec3(L, 1),
        luaL_checknumber(L, 2));
      return 1;
    }

    static int lua_quat_from_euler_angles( lua_State* L ) {
      *lua_newquat(L) = Quatf::from_euler_angles(
        luaL_checknumber(L, 1),
        luaL_checknumber(L, 2),
        luaL_checknumber(L, 3));
      return 1;
    }

    static int lua_quat__gc( lua_State* L ) {
      return 0;
    }

    static int lua_quat__add( lua_State* L ) {
      const Quatf& lhs = *lua_checkquat(L, 1);
      const Quatf& rhs = *lua_checkquat(L, 2);
      *lua_newquat(L) = lhs + rhs;
      return 1;
    }

    static int lua_quat__sub( lua_State* L ) {
      const Quatf& lhs = *lua_checkquat(L, 1);
      const Quatf& rhs = *lua_checkquat(L, 2);
      *lua_newquat(L) = lhs - rhs;
      return 1;
    }

    static int lua_quat__mul( lua_State* L ) {
      const Quatf& lhs = *lua_checkquat(L, 1);
      const Quatf& rhs = *lua_checkquat(L, 2);
      *lua_newquat(L) = lhs * rhs;
      return 1;
    }

    static int lua_quat__div( lua_State* L ) {
      const Quatf& lhs = *lua_checkquat(L, 1);
      const Quatf& rhs = *lua_checkquat(L, 2);
      *lua_newquat(L) = lhs / rhs;
      return 1;
    }

    static int lua_quat_length( lua_State* L ) {
      const Quatf& self = *lua_checkquat(L, 1);
      lua_pushnumber(L, self.length());
      return 1;
    }

    static int lua_quat_magnitude( lua_State* L ) {
      const Quatf& self = *lua_checkquat(L, 1);
      lua_pushnumber(L, self.magnitude());
      return 1;
    }

    static int lua_quat_dot( lua_State* L ) {
      const Quatf& lhs = *lua_checkquat(L, 1);
      const Quatf& rhs = *lua_checkquat(L, 2);
      lua_pushnumber(L, lhs.dot(rhs));
      return 1;
    }

    static int lua_quat_normalize( lua_State* L ) {
      const Quatf& self = *lua_checkquat(L, 1);
      *lua_newquat(L) = self.normalize();
      return 1;
    }

    static int lua_quat_inverse( lua_State* L ) {
      const Quatf& self = *lua_checkquat(L, 1);
      *lua_newquat(L) = self.inverse();
      return 1;
    }

    static int lua_quat__index( lua_State* L ) {
      const Quatf& self = *lua_checkquat(L, 1);
      if (strcmp("w", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.w);
      else if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.x);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.y);
      else if (strcmp("z", luaL_checkstring(L, 2)) == 0)
        lua_pushnumber(L, self.z);
      else {
        luaL_getmetatable(L, "Quat");
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        lua_remove(L, -2);
        return 1; }
      return 1;
    }

    static int lua_quat__newindex( lua_State* L ) {
      Quatf& self = *((Quatf*)lua_checkquat(L, 1));
      if (strcmp("w", luaL_checkstring(L, 2)) == 0)
        self.w = luaL_checknumber(L, 3);
      else if (strcmp("x", luaL_checkstring(L, 2)) == 0)
        self.x = luaL_checknumber(L, 3);
      else if (strcmp("y", luaL_checkstring(L, 2)) == 0)
        self.y = luaL_checknumber(L, 3);
      else if (strcmp("z", luaL_checkstring(L, 2)) == 0)
        self.z = luaL_checknumber(L, 3);
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

    static int lua_script_num_of_temporaries( lua_State* L ) {
      lua_pushlightuserdata(L, (void*)&__lua_temporaries_key);
      lua_rawget(L, LUA_REGISTRYINDEX);
      Array<lua_Temporary>& temporaries = *((Array<lua_Temporary>*)luaL_checkuserdata(L, -1));
      lua_pop(L, 1);
      lua_pushinteger(L, temporaries.size());
      return 1;
    }

    static int lua_script_set_num_of_temporaries( lua_State* L ) {
      lua_pushlightuserdata(L, (void*)&__lua_temporaries_key);
      lua_rawget(L, LUA_REGISTRYINDEX);
      Array<lua_Temporary>& temporaries = *((Array<lua_Temporary>*)luaL_checkuserdata(L, -1));
      lua_pop(L, 1);
      temporaries.resize(luaL_checkinteger(L, 1));
      return 0;
    }

    static int lua_require( lua_State* L ) {
      const char* path = luaL_checkstring(L, 1);
      luaL_getsubtable(L, LUA_REGISTRYINDEX, "_LOADED");
      lua_getfield(L, -1, path);
      if (!lua_isnil(L, -1)) {
        lua_remove(L, -2);
        return 1; }
      lua_pop(L, 1);
      const Resource::Handle<ScriptResource> resource =
        Resource::Id(ScriptResource::type(), path);
      if (luaL_loadbuffer(L, (const char*)resource->byte_code().raw(), resource->byte_code().size(), path) != 0)
        return luaL_error(L, lua_tostring(L, -1));
      lua_call(L, 0, 1);
      lua_pushvalue(L, -1);
      lua_setfield(L, -3, path);
      return 1;
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

      luaL_newmetatable(L, "Vec3");
      lua_pushcfunction(L, &lua_vec3__call);
      lua_setfield(L, -2, "__call");
      lua_pushcfunction(L, &lua_vec3__gc);
      lua_setfield(L, -2, "__gc");
      lua_pushcfunction(L, &lua_vec3__add);
      lua_setfield(L, -2, "__add");
      lua_pushcfunction(L, &lua_vec3__sub);
      lua_setfield(L, -2, "__sub");
      lua_pushcfunction(L, &lua_vec3__mul);
      lua_setfield(L, -2, "__mul");
      lua_pushcfunction(L, &lua_vec3__div);
      lua_setfield(L, -2, "__div");
      lua_pushcfunction(L, &lua_vec3_length);
      lua_setfield(L, -2, "length");
      lua_pushcfunction(L, &lua_vec3_magnitude);
      lua_setfield(L, -2, "magnitude");
      lua_pushcfunction(L, &lua_vec3_dot);
      lua_setfield(L, -2, "dot");
      lua_pushcfunction(L, &lua_vec3_cross);
      lua_setfield(L, -2, "cross");
      lua_pushcfunction(L, &lua_vec3_normalize);
      lua_setfield(L, -2, "normalize");
      lua_pushcfunction(L, &lua_vec3__index);
      lua_setfield(L, -2, "__index");
      lua_pushcfunction(L, &lua_vec3__newindex);
      lua_setfield(L, -2, "__newindex");
      lua_pushvalue(L, -1);
      lua_setmetatable(L, -2);
      lua_setfield(L, -2, "Vec3");

      luaL_newmetatable(L, "Vec4");
      lua_pushcfunction(L, &lua_vec4__call);
      lua_setfield(L, -2, "__call");
      lua_pushcfunction(L, &lua_vec4__gc);
      lua_setfield(L, -2, "__gc");
      lua_pushcfunction(L, &lua_vec4__add);
      lua_setfield(L, -2, "__add");
      lua_pushcfunction(L, &lua_vec4__sub);
      lua_setfield(L, -2, "__sub");
      lua_pushcfunction(L, &lua_vec4__mul);
      lua_setfield(L, -2, "__mul");
      lua_pushcfunction(L, &lua_vec4__div);
      lua_setfield(L, -2, "__div");
      lua_pushcfunction(L, &lua_vec4_length);
      lua_setfield(L, -2, "length");
      lua_pushcfunction(L, &lua_vec4_magnitude);
      lua_setfield(L, -2, "magnitude");
      lua_pushcfunction(L, &lua_vec4_dot);
      lua_setfield(L, -2, "dot");
      lua_pushcfunction(L, &lua_vec4_normalize);
      lua_setfield(L, -2, "normalize");
      lua_pushcfunction(L, &lua_vec4__index);
      lua_setfield(L, -2, "__index");
      lua_pushcfunction(L, &lua_vec4__newindex);
      lua_setfield(L, -2, "__newindex");
      lua_pushvalue(L, -1);
      lua_setmetatable(L, -2);
      lua_setfield(L, -2, "Vec4");

      luaL_newmetatable(L, "Quat");
      lua_pushcfunction(L, &lua_quat__call);
      lua_setfield(L, -2, "__call");
      lua_pushcfunction(L, &lua_quat_from_axis_angle);
      lua_setfield(L, -2, "from_axis_angle");
      lua_pushcfunction(L, &lua_quat_from_euler_angles);
      lua_setfield(L, -2, "from_euler_angles");
      lua_pushcfunction(L, &lua_quat__gc);
      lua_setfield(L, -2, "__gc");
      lua_pushcfunction(L, &lua_quat__add);
      lua_setfield(L, -2, "__add");
      lua_pushcfunction(L, &lua_quat__sub);
      lua_setfield(L, -2, "__sub");
      lua_pushcfunction(L, &lua_quat__mul);
      lua_setfield(L, -2, "__mul");
      lua_pushcfunction(L, &lua_quat__div);
      lua_setfield(L, -2, "__div");
      lua_pushcfunction(L, &lua_quat_length);
      lua_setfield(L, -2, "length");
      lua_pushcfunction(L, &lua_quat_magnitude);
      lua_setfield(L, -2, "magnitude");
      lua_pushcfunction(L, &lua_quat_dot);
      lua_setfield(L, -2, "dot");
      lua_pushcfunction(L, &lua_quat_normalize);
      lua_setfield(L, -2, "normalize");
      lua_pushcfunction(L, &lua_quat_inverse);
      lua_setfield(L, -2, "inverse");
      lua_pushcfunction(L, &lua_quat__index);
      lua_setfield(L, -2, "__index");
      lua_pushcfunction(L, &lua_quat__newindex);
      lua_setfield(L, -2, "__newindex");
      lua_pushvalue(L, -1);
      lua_setmetatable(L, -2);
      lua_setfield(L, -2, "Quat");

      lua_pushcfunction(L, &lua_require);
      lua_setfield(L, -2, "require");
    } lua_pop(L, 1);

    lua_createtable(L, 0, 3);
    lua_pushcfunction(L, &lua_script_log);
    lua_setfield(L, -2, "log");
    lua_pushcfunction(L, &lua_script_warn);
    lua_setfield(L, -2, "warn");
    lua_pushcfunction(L, &lua_script_fail);
    lua_setfield(L, -2, "fail");
    lua_pushcfunction(L, &lua_script_num_of_temporaries);
    lua_setfield(L, -2, "num_of_temporaries");
    lua_pushcfunction(L, &lua_script_set_num_of_temporaries);
    lua_setfield(L, -2, "set_num_of_temporaries");
    return 1;
  }
} // butane
