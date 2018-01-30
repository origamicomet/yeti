//===-- yeti/script/bindings/world_if.cc ----------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/world_if.h"

#include "yeti/script.h"

// To recover `Application *` from `lua_State *`.
#include "yeti/application.h"
#include "yeti/script/bindings/application_if.h"

// To deal with levels, entities, and prefabs.
#include "yeti/resource.h"
#include "yeti/resource_manager.h"

// To work with entity and component handles.
#include "yeti/script/bindings/entity_if.h"
#include "yeti/script/bindings/component_if.h"

#include "yeti/world.h"

namespace yeti {

namespace world_if {
  bool check(lua_State *L, int idx) {
    World *world = (World *)lua_touserdata(L, idx);

    if (world == NULL)
      // If it's not a pointer, then there's no way it's a pointer to a `World`.
      return false;

  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
      YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
    Application *app = application_if::instance(L);

    // Since all our worlds are referenced by our application, it's easy enough
    // to check if the given pointer is indeed to a `World`.
    if (app->worlds().find(world))
      // Bingo!
      return true;

    return false;
  #else
    // We'll assume everything's good. It's been tested, right?
    return true;
  #endif
  }

  World *cast(lua_State *L, int idx) {
    if (!check(L, idx))
      luaL_argerror(L, idx, "Expected a light user-data reference to a `World`.");

    return (World *)lua_touserdata(L, idx);
  }

  void push(lua_State *L, World *world) {
    lua_pushlightuserdata(L, (void *)world);
  }

  namespace {
    static int create(lua_State *L) {
      Application *app = application_if::instance(L);

      // We manage worlds through the application so we can track them.
      World *world = app->create_a_world();

      world_if::push(L, world);

      return 1;
    }

    static int destroy(lua_State *L) {
      Application *app = application_if::instance(L);

      World *world = world_if::cast(L, 1);

      app->destroy_a_world(world);

      return 0;
    }

    static int update(lua_State *L) {
      Application *app = application_if::instance(L);

      World *world = world_if::cast(L, 1);

      if (!lua_isnumber(L, 2))
        return luaL_argerror(L, 2, "Expected `delta_time` to be a number.");

      const lua_Number delta_time = lua_tonumber(L, 2);

      if (delta_time < (lua_Number)0)
        return luaL_argerror(L, 2, "Expected `delta_time` to be a positive number.");

      app->update_a_world(world, delta_time);

      return 0;
    }

    static int render(lua_State *L) {
      Application *app = application_if::instance(L);
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int spawn(lua_State *L) {
      Script *script = Script::recover(L);

      World *world;

      const char *name;

      Resource::Id resource;

      Vec3 position = Vec3(0.f, 0.f, 0.f);
      Quaternion rotation = Quaternion::IDENTITY;
      Vec3 scale = Vec3(1.f, 1.f, 1.f);

      switch (lua_gettop(L)) {
        default:
          return luaL_error(L, "World.spawn expects two to five arguments.");

        case 5:
          scale = script->to_a<Vec3>(5);

        case 4:
          rotation = script->to_a<Quaternion>(4);

        case 3:
          position = script->to_a<Vec3>(3);

        case 2:
          if (!lua_isstring(L, 2))
            return luaL_argerror(L, 2, "Expected `resource` to be a resource name.");

          name = lua_tostring(L, 2);

          static const Resource::Type::Id type =
            resource::id_from_type(resource::type_from_name("entity"));

          resource = resource::id_from_name(type, name);

        case 1:
          world = world_if::cast(L, 1);
      }

    #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
        YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
      if (!resource_manager::available(resource))
        return luaL_error(L, "Can't spawn `%s` as it is not available!", name);
    #endif

      const Entity entity = world->spawn(resource, position, rotation, scale);

      entity_if::push(L, { world, entity });

      return 1;
    }

    static int kill(lua_State *L) {
      World *world = world_if::cast(L, 1);

      // TODO(mtwilliams): Vaildate handle against |world|.
      const Entity entity = entity_if::cast(L, 2).entity;

      world->kill(entity);

      return 0;
    }

    // static int entity_by_id(lua_State *L) {
    //   return luaL_error(L, "Not implemented yet.");
    // }

    // static int entity_by_name(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = world_if::cast(L, 1);
    //   const char *name = luaL_checkstring(L, 2);

    //   Entity entity;

    //   if (world->entities()->named(name, &entity))
    //     script->push<Entity>(entity);
    //   else
    //     lua_pushnil(L);

    //   return 1;
    // }
  }
} // world_if

void world_if::expose(Script *script) {
  script->add_module("World");

  script->add_module_function("World", "create", &create);
  script->add_module_function("World", "destroy", &destroy);

  script->add_module_function("World", "update", &update);
  script->add_module_function("World", "render", &render);

  script->add_module_function("World", "spawn", &spawn);
  script->add_module_function("World", "kill", &kill);

  // script->add_module_function("World", "entity_by_id", &entity_by_id);
  // script->add_module_function("World", "entity_by_name", &entity_by_name);

#if 0
  script->add_module_function("World", "load", &load);
  script->add_module_function("World", "unload", &unload);
#endif
}

} // yeti
