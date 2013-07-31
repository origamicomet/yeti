// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/world_if.h>
#include <butane/world.h>

#include <butane/application.h>
#include <butane/graphics/swap_chain.h>
#include <butane/graphics/viewport.h>
#include <butane/tied_resources.h>

namespace butane {
  namespace {
    static World* lua_checkworld( lua_State* L, int idx ) {
      return (World*)luaL_checklightuserdata(L, idx);
    }

    static int lua_world_create( lua_State* L ) {
      World* world = World::create();
      Application::worlds() += world;
      lua_pushlightuserdata(L, (void*)world);
      return 1;
    }

    static int lua_world_destroy( lua_State* L ) {
      World* world = lua_checkworld(L, 1);
      Application::worlds().remove(Application::worlds().find(world));
      world->destroy();
      return 0;
    }

    static int lua_world_spawn_unit( lua_State* L ) {
      World* world = lua_checkworld(L, 1);
      const Resource::Handle<UnitResource> type =
        Resource::Id(UnitResource::type(), luaL_checkstring(L, 2));
      Unit::Id unit_id;
      switch (lua_gettop(L)) {
        case 2: {
          unit_id = world->spawn_unit(type);
        } break;
        case 5: {
          const Vec3f& position = **((const Vec3f**)luaL_checkuserdata2(L, 3, "Vec3"));
          const Quatf& rotation = **((const Quatf**)luaL_checkuserdata2(L, 4, "Quat"));
          const Vec3f& scale = **((const Vec3f**)luaL_checkuserdata2(L, 5, "Vec3"));
          unit_id = world->spawn_unit(type, position, rotation, scale);
        } break;
        default:
          return luaL_error(L, "expected two or five arguments"); }
      Unit::Reference& ref = *((Unit::Reference*)lua_newuserdata(L, sizeof(Unit::Reference)));
      ref = Unit::Reference(*world, unit_id);
      return 1;
    }

    static int lua_world_destroy_unit( lua_State* L ) {
      World* world = lua_checkworld(L, 1);
      const Unit::Reference& ref = *((const Unit::Reference*)luaL_checkuserdata(L, 2));
      luaL_argcheck(L, ref.valid(), 2, "invalid reference");
      luaL_argcheck(L, ref.is_unit(), 2, "expected a unit--not node--reference");
      const Unit& unit = ref.to_unit();
      luaL_argcheck(L, (&unit.world() == world), 2, "unit does not belong to world");
      world->destroy_unit(unit.id());
      return 0;
    }

    static int lua_world_update( lua_State* L ) {
      World* world = lua_checkworld(L, 1);
      const float dt = luaL_checknumber(L, 2);
      world->update(dt);
      return 0;
    }

    static const TiedResources* find_swap_chain_and_resources( const SwapChain* swap_chain ) {
      for (auto iter = Application::tied_resources().begin(); iter != Application::tied_resources().end(); ++iter)
        if ((*iter)->swap_chain() == swap_chain)
          return *iter;
      return nullptr;
    }

    static int lua_world_render( lua_State* L ) {
      World* world = lua_checkworld(L, 1);
      const Unit::Reference& camera = *((const Unit::Reference*)luaL_checkuserdata(L, 2));
      const Viewport& viewport = *((const Viewport*)luaL_checkuserdata(L, 3));
      const SwapChain* swap_chain = ((const SwapChain*)luaL_checklightuserdata(L, 4));
      const TiedResources* swap_chain_and_resources = find_swap_chain_and_resources(swap_chain);
      world->render(camera, viewport, swap_chain_and_resources);
      return 0;
    }

    static int lua_world_units__index( lua_State* L ) {
      World* world = *((World**)luaL_checkuserdata(L, 1));
      const int idx = luaL_checkinteger(L, 2);
      luaL_argcheck(L, ((idx > 0) && (idx < (int)world->num_of_units())), 2, "out-of-bounds");
      *((Unit::Reference*)lua_newuserdata(L, sizeof(Unit::Reference))) =
        Unit::Reference(*world, world->units()[idx]->id());
      return 1;
    }

    static int lua_world_units__newindex( lua_State* L ) {
      return luaL_error(L, "invalid operation (read-only)");
    }

    static int lua_world_units__len( lua_State* L ) {
      const World* world = *((World**)luaL_checkuserdata(L, 1));
      lua_pushinteger(L, world->num_of_units());
      return 1;
    }

    static int lua_world_units( lua_State* L ) {
      World* world = lua_checkworld(L, 1);
      *((World**)lua_newuserdata(L, sizeof(World*))) = world;
      lua_pushvalue(L, lua_upvalueindex(0));
      lua_setmetatable(L, -2);
      return 1;
    }
  }
} // butane

namespace butane {
  int luaopen_world( lua_State* L )
  {
    lua_createtable(L, 0, 7);
    lua_pushcfunction(L, &lua_world_create);
    lua_setfield(L, -2, "create");
    lua_pushcfunction(L, &lua_world_destroy);
    lua_setfield(L, -2, "destroy");
    lua_pushcfunction(L, &lua_world_spawn_unit);
    lua_setfield(L, -2, "spawn_unit");
    lua_pushcfunction(L, &lua_world_destroy_unit);
    lua_setfield(L, -2, "destroy_unit");
    lua_pushcfunction(L, &lua_world_update);
    lua_setfield(L, -2, "update");
    lua_pushcfunction(L, &lua_world_render);
    lua_setfield(L, -2, "render");
    lua_createtable(L, 0, 3);
    lua_pushcfunction(L, &lua_world_units__index);
    lua_setfield(L, -2, "__index");
    lua_pushcfunction(L, &lua_world_units__newindex);
    lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, &lua_world_units__len);
    lua_setfield(L, -2, "__len");
    lua_pushcclosure(L, &lua_world_units, 1);
    lua_setfield(L, -2, "units");
    return 1;
  }
} // butane
