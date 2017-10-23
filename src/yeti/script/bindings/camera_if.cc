//===-- yeti/script/bindings/camera_if.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/camera_if.h"

#include "yeti/script.h"

// Camera system is accessed through `World`.
#include "yeti/world.h"

// Interfaces with entirety of Entity-Component-System.
#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

#include "yeti/components/camera.h"

namespace yeti {

#if 0
template <> bool Script::is_a<Camera::Handle>(int index) {
  if (!is_a<Reference>(index))
    return false;

  // Assume it's a reference to an camera.
  return true;
}

template <> Camera::Handle Script::to_a<Camera::Handle>(int index) {
  if (!is_a<Camera::Handle>(index))
    luaL_argerror(L, index, "Expected a reference to a `Camera`.");

  return {to_a<Reference>(index).opaque};
}

template <> void Script::push<Camera::Handle>(Camera::Handle camera) {
  push<Reference>({camera.instance});
}

namespace camera_if {
  namespace {
    static int create(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      Entity entity = script->to_a<Entity>(2);

      const Camera::Handle camera =
        world->cameras()->create(entity);

      script->push(camera);

      return 1;
    }

    static int destroy(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);

      if (script->is_a<Entity>(2)) {
        const Entity entity = script->to_a<Entity>(2);
        world->cameras()->destroy(entity);
      } else if (script->is_a<Camera::Handle>(2)) {
        const Camera::Handle handle = script->to_a<Camera::Handle>(2);
        world->cameras()->destroy(handle);
      } else {
        return luaL_argerror(L, 1, "Expected an `Entity` or a `Camera`.");
      }

      return 0;
    }

    static int lookup(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Entity entity = script->to_a<Entity>(2);

      Camera::Handle handle;

      if (lua_gettop(L) >= 3) {
        const unsigned instance = (unsigned)luaL_checkinteger(L, 3);
        handle = world->cameras()->lookup(entity, instance);
      } else {
        handle = world->cameras()->lookup(entity);
      }

      script->push<Camera::Handle>(handle);

      return 1;
    }

    static int get_type(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      lua_pushinteger(L, (lua_Integer)world->cameras()->get_type(handle));

      return 1;
    }

    static int get_field_of_view(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      lua_pushnumber(L, (lua_Number)world->cameras()->get_field_of_view(handle));

      return 1;
    }

    static int get_box(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      f32 top, left, bottom, right;

      world->cameras()->get_box(handle, &top, &left, &bottom, &right);

      lua_pushnumber(L, (lua_Number)top);
      lua_pushnumber(L, (lua_Number)left);
      lua_pushnumber(L, (lua_Number)bottom);
      lua_pushnumber(L, (lua_Number)right);

      return 4;
    }

    static int get_planes(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const f32 near = world->cameras()->get_near_plane(handle);
      const f32 far = world->cameras()->get_far_plane(handle);

      lua_pushnumber(L, (lua_Number)near);
      lua_pushnumber(L, (lua_Number)far);

      return 2;
    }

    static int set_type(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const Camera::Type type = (Camera::Type)luaL_checkinteger(L, 3);

      switch (type) {
        default:
          return luaL_argerror(L, 3, "expected `Camera.PERSPECTIVE` or `Camera.ORTHOGRAPHIC`");

        case Camera::PERSPECTIVE:
        case Camera::ORTHOGRAPHIC:
          world->cameras()->set_type(handle, type);
      }

      return 0;
    }

    static int set_field_of_view(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const f32 field_of_view = luaL_checknumber(L, 3);

      if (field_of_view <= 0.f)
        return luaL_argerror(L, 3, "Field-of-view must be greater than 0°!");

      if (fov > 180.f)
        return luaL_argerror(L, 3, "Field-of-view must be lesser than 180°!");

      world->cameras()->set_field_of_view(handle, field_of_view);

      return 0;
    }

    static int set_box(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const f32 top = luaL_checknumber(L, 3);
      const f32 left = luaL_checknumber(L, 4);
      const f32 bottom = luaL_checknumber(L, 5);
      const f32 right = luaL_checknumber(L, 6);

      world->cameras()->set_box(handle, top, left, bottom, right);

      return 0;
    }

    static int set_planes(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const f32 near = luaL_checknumber(L, 3);
      const f32 far = luaL_checknumber(L, 4);

      if (near < 0.f)
        return luaL_argerror(L, 3, "Near plane can't be behind the camera!");

      if (far < 0.0f)
        return luaL_argerror(L, 4, "Far plane can't be behind the camera!");

      world->cameras()->set_near_plane(handle, near);
      world->cameras()->set_far_plane(handle, far);

      return 0;
    }

    static int set_near_plane(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const f32 near = luaL_checknumber(L, 3);

      world->cameras()->set_near_plane(handle, near);

      return 0;
    }

    static int set_far_plane(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const f32 far = luaL_checknumber(L, 3);

      world->cameras()->set_far_plane(handle, far);

      return 0;
    }

    static int describe(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Camera::Handle handle = script->to_a<Camera::Handle>(2);

      const Camera camera = world->cameras()->describe(handle);

      lua_createtable(L, 0, 4);

      lua_pushinteger(L, (lua_Integer)camera.type);
      lua_setfield(L, -2, "type");

      if (camera.type == Camera::PERSPECTIVE) {
        lua_pushinteger(L, (lua_Integer)camera.perspective.fov);
        lua_setfield(L, -2, "field_of_view");
      } else if (camera.type == Camera::ORTHOGRAPHIC) {
        lua_createtable(L, 0, 4);

        lua_pushinteger(L, (lua_Integer)camera.orthographic.top);
        lua_setfield(L, -2, "top");
        lua_pushinteger(L, (lua_Integer)camera.orthographic.left);
        lua_setfield(L, -2, "left");
        lua_pushinteger(L, (lua_Integer)camera.orthographic.bottom);
        lua_setfield(L, -2, "bottom");
        lua_pushinteger(L, (lua_Integer)camera.orthographic.right);
        lua_setfield(L, -2, "right");

        lua_setfield(L, -2, "box");
      }

      lua_pushinteger(L, (lua_Integer)camera.near);
      lua_setfield(L, -2, "near");

      lua_pushinteger(L, (lua_Integer)camera.far);
      lua_setfield(L, -2, "far");

      return 1;
    }
  }
} // camera_if
#endif

void camera_if::expose(Script *script) {
  script->add_module("Camera");

#if 0
  script->add_module_constant("Camera", "PERSPECTIVE", Camera::PERSPECTIVE);
  script->add_module_constant("Camera", "ORTHOGRAPHIC", Camera::ORTHOGRAPHIC);

  script->add_module_function("Camera", "create", &create);
  script->add_module_function("Camera", "destroy", &destroy);

  script->add_module_function("Camera", "lookup", &lookup);

  script->add_module_function("Camera", "type", &get_type);
  script->add_module_function("Camera", "field_of_view", &get_field_of_view);
  script->add_module_function("Camera", "box", &get_box);
  script->add_module_function("Camera", "planes", &get_planes);

  script->add_module_function("Camera", "set_type", &set_type);
  script->add_module_function("Camera", "set_field_of_view", &set_field_of_view);
  script->add_module_function("Camera", "set_box", &set_box);
  script->add_module_function("Camera", "set_planes", &set_planes);
  script->add_module_function("Camera", "set_near_plane", &set_near_plane);
  script->add_module_function("Camera", "set_far_plane", &set_far_plane);

  script->add_module_function("Camera", "describe", &describe);
#endif
}

} // yeti
