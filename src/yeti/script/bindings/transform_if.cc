//===-- yeti/script/bindings/transform_if.cc ------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/transform_if.h"

#include "yeti/script.h"

// To work with entity and component handles.
#include "yeti/script/bindings/entity_if.h"
#include "yeti/script/bindings/component_if.h"

// Bindings are intimately tied to worlds.
#include "yeti/world.h"

// Interfaces with entirety of Entity-Component-System.
#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

#include "yeti/components/transform.h"

namespace yeti {

namespace transform_if {
  static const u32 TYPE = component_registry::id_from_name("transform");

  static bool check(lua_State *L, int idx) {
    return component_if::check(L, idx, TYPE);
  }

  static Transform::Handle cast(lua_State *L, int idx) {
    return component_if::cast(L, idx, TYPE).handle;
  }

  static void push(lua_State *L, Transform::Handle handle) {
    component_if::push(L, TYPE, handle);
  }

  namespace {
    // static int create(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world;
    //   Entity entity;

    //   Vec3 position = Vec3(0.f, 0.f, 0.f);
    //   Quaternion rotation = Quaternion::IDENTITY;
    //   Vec3 scale = Vec3(1.f, 1.f, 1.f);

    //   switch (lua_gettop(L)) {
    //     default:
    //       return luaL_error(L, "Transform.create expects two to five arguments.");

    //     case 5:
    //       scale = script->to_a<Vec3>(5);

    //     case 4:
    //       rotation = script->to_a<Quaternion>(4);

    //     case 3:
    //       position = script->to_a<Vec3>(3);

    //     case 2:
    //       world = script->to_a<World *>(1);
    //       entity = script->to_a<Entity>(2);
    //   }

    //   const Transform::Handle transform =
    //     world->transforms()->create(entity, position, rotation, scale);

    //   const Handle handle = { world->transforms(), entity, 0 };

    //   component_if::push(L, handle);

    //   return 1;
    // }

    // static int destroy(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);

    //   if (script->is_a<Entity>(2)) {
    //     const Entity entity = script->to_a<Entity>(2);
    //     world->transforms()->destroy(entity);
    //   } else if (script->is_a<Transform::Handle>(2)) {
    //     const Transform::Handle handle = script->to_a<Transform::Handle>(2);
    //     world->transforms()->destroy(handle);
    //   } else {
    //     return luaL_argerror(L, 1, "Expected an `Entity` or a `Transform`.");
    //   }

    //   return 0;
    // }

    static int lookup(lua_State *L) {
      World *world = world_if::cast(L, 1);
      const Entity entity = entity_if::cast(L, 2).entity;

      if (world->transforms()->has(entity))
        transform_if::push(L, world->transforms()->lookup(entity));
      else
        lua_pushnil(L);

      return 1;
    }

    // static int get_local_position(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   script->push<Vec3>(world->transforms()->get_local_position(handle));

    //   return 1;
    // }

    // static int get_local_rotation(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   script->push<Quaternion>(world->transforms()->get_local_rotation(handle));

    //   return 1;
    // }

    // static int get_local_scale(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   script->push<Vec3>(world->transforms()->get_local_scale(handle));

    //   return 1;
    // }

    // static int set_local_position(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   world->transforms()->set_local_position(handle, script->to_a<Vec3>(3));

    //   return 0;
    // }

    // static int set_local_rotation(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   world->transforms()->set_local_rotation(handle, script->to_a<Quaternion>(3));

    //   return 0;
    // }

    // static int set_local_scale(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   world->transforms()->set_local_scale(handle, script->to_a<Vec3>(3));

    //   return 0;
    // }

    // static int get_local_pose(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   script->push<Mat4>(world->transforms()->get_local_pose(handle));

    //   return 1;
    // }

    // static int get_world_pose(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   script->push<Mat4>(world->transforms()->get_world_pose(handle));

    //   return 1;
    // }

    // static int link(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world;
    //   Transform::Handle child;
    //   Transform::Handle parent;

    //   Vec3 position = Vec3(0.f, 0.f, 0.f);
    //   Quaternion rotation = Quaternion::IDENTITY;
    //   Vec3 scale = Vec3(1.f, 1.f, 1.f);

    //   switch (lua_gettop(L)) {
    //     default:
    //       return luaL_error(L, "Transform.link expects three to six arguments.");

    //     case 6:
    //       scale = script->to_a<Vec3>(6);

    //     case 5:
    //       rotation = script->to_a<Quaternion>(5);

    //     case 4:
    //       position = script->to_a<Vec3>(4);

    //     case 3:
    //       world = script->to_a<World *>(1);
    //       child = script->to_a<Transform::Handle>(2);
    //       parent = script->to_a<Transform::Handle>(3);
    //   }

    //   world->transforms()->link(child, parent, position, rotation, scale);

    //   return 0;
    // }

    // static int unlink(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   world->transforms()->unlink(handle);

    //   return 0;
    // }

    // static int recompute(lua_State *L) {
    //   Script *script = Script::recover(L);

    //   World *world = script->to_a<World *>(1);
    //   const Transform::Handle handle = script->to_a<Transform::Handle>(2);

    //   world->transforms()->recompute(handle);

    //   return 0;
    // }
  }
} // transform_if

void transform_if::expose(Script *script) {
  script->add_module("TransformSystem");

  // script->add_module_function("TransformSystem", "create", &create);
  // script->add_module_function("TransformSystem", "destroy", &destroy);

  script->add_module_function("TransformSystem", "lookup", &lookup);

  // script->add_module_function("TransformSystem", "position", &get_local_position);
  // script->add_module_function("TransformSystem", "rotation", &get_local_rotation);
  // script->add_module_function("TransformSystem", "scale", &get_local_scale);

  // script->add_module_function("TransformSystem", "set_position", &set_local_position);
  // script->add_module_function("TransformSystem", "set_rotation", &set_local_rotation);
  // script->add_module_function("TransformSystem", "set_scale", &set_local_scale);

  // script->add_module_function("TransformSystem", "local", &get_local_pose);
  // script->add_module_function("TransformSystem", "world", &get_world_pose);

  // script->add_module_function("TransformSystem", "link", &link);
  // script->add_module_function("TransformSystem", "unlink", &unlink);

  // script->add_module_function("TransformSystem", "recompute", &recompute);
}

} // yeti
