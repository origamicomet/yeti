//===-- yeti/script/bindings/entity_if.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/entity_if.h"

#include "yeti/script.h"

#include "yeti/world.h"

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

namespace yeti {

template <> bool Script::is_a<Entity>(int index) {
  if (!is_a<Reference>(index))
    return false;

  // Assume it's a reference to an entity.
  return true;
}

template <> Entity Script::to_a<Entity>(int index) {
  if (!is_a<Entity>(index))
    luaL_argerror(L, index, "Expected a reference to an `Entity`.");

  return Entity(to_a<Reference>(index).opaque);
}

template <> void Script::push<Entity>(Entity entity) {
  push<Reference>({entity.id});
}

namespace entity_if {
  namespace {
    static int create(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);

      const Entity entity = world->entities()->create();

      script->push<Entity>(entity);

      return 1;
    }

    static int destroy(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Entity entity = script->to_a<Entity>(2);

      world->entities()->destroy(entity);

      return 0;
    }

    static int alive(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Entity entity = script->to_a<Entity>(2);

      lua_pushboolean(L, world->entities()->alive(entity));

      return 1;
    }

    static int dead(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Entity entity = script->to_a<Entity>(2);

      lua_pushboolean(L, world->entities()->dead(entity));

      return 1;
    }

    static int name(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const Entity entity = script->to_a<Entity>(2);
      const char *name = luaL_checkstring(L, 3);

      world->entities()->name(entity, name);

      return 0;
    }

    static int named(lua_State *L) {
      Script *script = Script::recover(L);

      World *world = script->to_a<World *>(1);
      const char *name = luaL_checkstring(L, 2);

      Entity entity;

      if (world->entities()->named(name, &entity))
        script->push<Entity>(entity);
      else
        lua_pushnil(L);

      return 1;
    }
  }
} // entity_if

void entity_if::expose(Script *script) {
  script->add_module("Entity");

  script->add_module_function("Entity", "create", &create);
  script->add_module_function("Entity", "destroy", &destroy);

  script->add_module_function("Entity", "alive", &alive);
  script->add_module_function("Entity", "dead", &dead);

  script->add_module_function("Entity", "name", &name);
  script->add_module_function("Entity", "named", &named);
}

} // yeti
