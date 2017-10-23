//===-- yeti/script/bindings/light_if.cc ----------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/light_if.h"

#include "yeti/script.h"

// Light system is accessed through `World`.
#include "yeti/world.h"

// Interfaces with entirety of Entity-Component-System.
#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

#include "yeti/components/light.h"

namespace yeti {

#if 0
template <> bool Script::is_a<Light::Handle>(int index) {
  if (!is_a<Reference>(index))
    return false;

  // Assume it's a reference to an light.
  return true;
}

template <> Light::Handle Script::to_a<Light::Handle>(int index) {
  if (!is_a<Light::Handle>(index))
    luaL_argerror(L, index, "Expected a reference to a `Light`.");

  return {to_a<Reference>(index).opaque};
}

template <> void Script::push<Light::Handle>(Light::Handle light) {
  push<Reference>({light.instance});
}

namespace light_if {
  namespace {
    static int create(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      Entity entity = script->to_a<Entity>(2);

      const Light::Handle light =
        world->lights()->create(entity);

      script->push(light);

      return 1;
    }

    static int destroy(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);

      if (script->is_a<Entity>(2)) {
        const Entity entity = script->to_a<Entity>(2);
        world->lights()->destroy(entity);
      } else if (script->is_a<Light::Handle>(2)) {
        const Light::Handle handle = script->to_a<Light::Handle>(2);
        world->lights()->destroy(handle);
      } else {
        return luaL_argerror(L, 1, "Expected an `Entity` or a `Light`.");
      }

      return 0;
    }

    static int lookup(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Entity entity = script->to_a<Entity>(2);

      Light::Handle handle;

      if (lua_gettop(L) >= 3) {
        const unsigned instance = (unsigned)luaL_checkinteger(L, 3);
        handle = world->lights()->lookup(entity, instance);
      } else {
        handle = world->lights()->lookup(entity);
      }

      script->push<Light::Handle>(handle);

      return 1;
    }

    static int get_type(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      lua_pushinteger(L, (lua_Integer)world->lights()->get_type(handle));

      return 1;
    }

    static int get_radius(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      lua_pushnumber(L, (lua_Number)world->lights()->get_radius(handle));

      return 1;
    }

    static int get_angle(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      lua_pushnumber(L, (lua_Number)world->lights()->get_angle(handle));

      return 1;
    }

    static int get_color(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      const Color color = world->lights()->get_color(handle);

      lua_pushnumber(L, (lua_Number)color.r);
      lua_pushnumber(L, (lua_Number)color.g);
      lua_pushnumber(L, (lua_Number)color.b);

      return 1;
    }

    static int get_intensity(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      lua_pushnumber(L, (lua_Number)world->lights()->get_intensity(handle));

      return 1;
    }

    static int set_type(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      const Light::Type type = (Light::Type)luaL_checkinteger(L, 3);

      switch (type) {
        default:
          return luaL_argerror(L, 3, "expected `Light.DIRECTIONAL`, `Light.POINT`, or `Light.SPOT`");

        case Light::DIRECTIONAL:
        case Light::POINT:
        case Light::SPOT:
          world->lights()->set_type(handle, type);
      }

      return 0;
    }

    static int set_radius(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      world->lights()->set_radius(handle, luaL_checknumber(L, 3));

      return 0;
    }

    static int set_angle(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      world->lights()->set_angle(handle, luaL_checknumber(L, 3));

      return 0;
    }

    static int set_color(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      const lua_Number red = luaL_checknumber(L, 3);
      const lua_Number green = luaL_checknumber(L, 4);
      const lua_Number blue = luaL_checknumber(L, 5);

      world->lights()->set_color(handle, Color(red, green, blue));

      return 0;
    }

    static int set_intensity(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      world->lights()->set_intensity(handle, luaL_checknumber(L, 3));

      return 0;
    }

    static int is_enabled(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      const u32 flags = world->lights()->flags(handle);

      lua_pushboolean(L, !!(flags & Light::ENABLED));

      return 1;
    }

    static int is_disabled(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      const u32 flags = world->lights()->flags(handle);

      lua_pushboolean(L, !(flags & Light::ENABLED));

      return 1;
    }

    static int enable(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      world->lights()->set_flags(handle, Light::ENABLED, Light::ENABLED);

      return 0;
    }

    static int disable(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      world->lights()->set_flags(handle, 0, Light::ENABLED);

      return 0;
    }

    static int describe(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Light::Handle handle = script->to_a<Light::Handle>(2);

      const Light light = world->lights()->describe(handle);

      const bool is_enabled = !!(light.flags & Light::ENABLED);
      const bool is_disabled = !(light.flags & Light::ENABLED);
      const bool is_baked = !!(light.flags & Light::BAKED);
      const bool is_virtual = !!(light.flags & Light::VIRTUAL);
      const bool is_shadow_casting = !!(light.flags & Light::SHADOWS);
      const bool is_volumetric = !!(light.flags & Light::VOLUMETRIC);

      lua_createtable(L, 0, 16);

      lua_pushinteger(L, (lua_Integer)light.type);
      lua_setfield(L, -2, "type");

      lua_pushboolean(L, is_enabled);
      lua_setfield(L, -2, "enabled");

      lua_pushboolean(L, is_disabled);
      lua_setfield(L, -2, "disabled");

      lua_pushboolean(L, is_baked);
      lua_setfield(L, -2, "baked");

      lua_pushboolean(L, is_virtual);
      lua_setfield(L, -2, "virtual");

      lua_pushboolean(L, is_shadow_casting);
      lua_setfield(L, -2, "shadows");

      lua_pushboolean(L, is_volumetric);
      lua_setfield(L, -2, "volumetric");

      if (light.type == Light::DIRECTIONAL) {
        lua_createtable(L, 0, 3);

        lua_pushnumber(L, (lua_Number)light.directional.color.r);
        lua_setfield(L, -2, "r");
        lua_pushnumber(L, (lua_Number)light.directional.color.g);
        lua_setfield(L, -2, "g");
        lua_pushnumber(L, (lua_Number)light.directional.color.b);
        lua_setfield(L, -2, "b");

        lua_setfield(L, -2, "color");

        lua_pushnumber(L, (lua_Number)light.directional.intensity);
        lua_setfield(L, -2, "intensity");
      } else if (light.type == Light::POINT) {
        lua_pushnumber(L, (lua_Number)light.point.radius);
        lua_setfield(L, -2, "radius");

        lua_createtable(L, 0, 3);

        lua_pushnumber(L, (lua_Number)light.point.color.r);
        lua_setfield(L, -2, "r");
        lua_pushnumber(L, (lua_Number)light.point.color.g);
        lua_setfield(L, -2, "g");
        lua_pushnumber(L, (lua_Number)light.point.color.b);
        lua_setfield(L, -2, "b");

        lua_setfield(L, -2, "color");

        lua_pushnumber(L, (lua_Number)light.point.intensity);
        lua_setfield(L, -2, "intensity");
      } else if (light.type == Light::SPOT) {
        lua_pushnumber(L, (lua_Number)light.spot.radius);
        lua_setfield(L, -2, "radius");

        lua_pushnumber(L, (lua_Number)light.spot.angle);
        lua_setfield(L, -2, "angle");

        lua_createtable(L, 0, 3);

        lua_pushnumber(L, (lua_Number)light.spot.color.r);
        lua_setfield(L, -2, "r");
        lua_pushnumber(L, (lua_Number)light.spot.color.g);
        lua_setfield(L, -2, "g");
        lua_pushnumber(L, (lua_Number)light.spot.color.b);
        lua_setfield(L, -2, "b");

        lua_setfield(L, -2, "color");

        lua_pushnumber(L, (lua_Number)light.spot.intensity);
        lua_setfield(L, -2, "intensity");
      }

      return 1;
    }
  }
} // light_if
#endif

void light_if::expose(Script *script) {
  script->add_module("Light");

#if 0
  script->add_module_constant("Light", "DIRECTIONAL", Light::DIRECTIONAL);
  script->add_module_constant("Light", "POINT", Light::POINT);
  script->add_module_constant("Light", "SPOT", Light::SPOT);

  script->add_module_function("Light", "create", &create);
  script->add_module_function("Light", "destroy", &destroy);

  script->add_module_function("Light", "lookup", &lookup);

  script->add_module_function("Light", "type", &get_type);
  script->add_module_function("Light", "radius", &get_radius);
  script->add_module_function("Light", "angle", &get_angle);
  script->add_module_function("Light", "color", &get_color);
  script->add_module_function("Light", "intensity", &get_intensity);

  script->add_module_function("Light", "set_type", &set_type);
  script->add_module_function("Light", "set_radius", &set_radius);
  script->add_module_function("Light", "set_angle", &set_angle);
  script->add_module_function("Light", "set_color", &set_color);
  script->add_module_function("Light", "set_intensity", &set_intensity);

  script->add_module_function("Light", "enabled", &is_enabled);
  script->add_module_function("Light", "disabled", &is_disabled);

  script->add_module_function("Light", "enable", &enable);
  script->add_module_function("Light", "disable", &enable);

  script->add_module_function("Light", "describe", &describe);
#endif
}

} // yeti
