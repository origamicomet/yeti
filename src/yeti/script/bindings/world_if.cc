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

#include "yeti/world.h"

namespace yeti {

template <> bool Script::is_a<World *>(int index) {
  World *world = (World *)lua_touserdata(L, index);

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

template <> World *Script::to_a<World *>(int index) {
  if (!is_a<World *>(index))
    luaL_argerror(L, index, "Expected a light user-data reference to a `World`.");

  return (World *)lua_touserdata(L, index);
}

namespace world_if {
  namespace {
    static int create(lua_State *L) {
      Application *app = application_if::instance(L);

      // We manage worlds through the application so we can track them.
      World *world = app->create_a_world();

      lua_pushlightuserdata(L, (void *)world);

      return 1;
    }

    static int destroy(lua_State *L) {
      Script *script = Script::recover(L);
      Application *app = application_if::instance(L);

      World *world = script->to_a<World *>(1);

      app->destroy_a_world(world);

      return 0;
    }

    static int update(lua_State *L) {
      Script *script = Script::recover(L);
      Application *app = application_if::instance(L);

      World *world = script->to_a<World *>(1);

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
  }
} // world_if

void world_if::expose(Script *script) {
  script->add_module("World");

  script->add_module_function("World", "create", &create);
  script->add_module_function("World", "destroy", &destroy);

  script->add_module_function("World", "update", &update);
  script->add_module_function("World", "render", &render);

#if 0
  script->add_module_function("World", "unit_by_id", &unit_by_id);
  script->add_module_function("World", "unit_by_name", &unit_by_name);

  script->add_module_function("World", "spawn", &spawn);
  script->add_module_function("World", "despawn", &despawn);

  script->add_module_function("World", "link", &link);
  script->add_module_function("World", "unlink", &unlink);
#endif

#if 0
  script->add_module_function("World", "load", &load);
  script->add_module_function("World", "unload", &unload);
#endif
}

} // yeti
