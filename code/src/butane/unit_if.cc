// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/unit_if.h>
#include <butane/unit.h>

#include <butane/script_if.h>

namespace butane {
  Unit::Reference* lua_newunitref( lua_State* L ) {
    return ((Unit::Reference*)lua_newuserdata(L, sizeof(Unit::Reference)));
  }

  Unit& lua_checkunit( lua_State* L, int idx ) {
    const Unit::Reference& ref = *((Unit::Reference*)luaL_checkuserdata(L, idx));
    luaL_argcheck(L, ref.valid(), idx, "invalid unit reference");
    luaL_argcheck(L, ref.is_unit(), idx, "expected a unit--not node--reference");
    return ref.to_unit();
  }

  SceneGraph::Node& lua_checknode( lua_State* L, int idx ) {
    const Unit::Reference& ref = *((Unit::Reference*)luaL_checkuserdata(L, idx));
    luaL_argcheck(L, ref.valid(), idx, "invalid unit reference");
    luaL_argcheck(L, ref.is_node(), idx, "expected a node--not unit--reference");
    return ref.to_node();
  }

  namespace {
    static int lua_unit_node( lua_State* L ) {
      const Unit& unit = lua_checkunit(L, 1);
      switch (lua_type(L, 2)) {
        case LUA_TNUMBER: {
          const SceneGraph::Node::Id id = luaL_checkinteger(L, 2);
          luaL_argcheck(L, (id < unit.scene_graph().num_of_nodes()), 2, "out-of-bounds");
          *lua_newunitref(L) = Unit::Reference(unit.world(), unit.id(), id);
        } break;
        case LUA_TSTRING: {
          const SceneGraph::Node::Id node =
            unit.scene_graph().find_node_by_name(luaL_checkstring(L, 2));
          if (node == SceneGraph::Node::invalid) lua_pushnil(L);
          else *lua_newunitref(L) = Unit::Reference(unit.world(), unit.id(), node);
        } break;
        default:
          return luaL_error(L, "expected an id or name"); }
      return 1;
    }

    static int lua_unit_node_typed( lua_State* L, const SceneGraph::Node::Type type ) {
      const Unit& unit = lua_checkunit(L, 1);
      const SceneGraph::Node::Id node =
        unit.scene_graph().find_node_by_name(luaL_checkstring(L, 2));
      if ((node == SceneGraph::Node::invalid) || (unit.scene_graph().types()[node] != type))
        lua_pushnil(L);
      else
        *lua_newunitref(L) = Unit::Reference(unit.world(), unit.id(), node);
      return 1;
    }

    static int lua_unit_empty( lua_State* L ) {
      return lua_unit_node_typed(L, SceneGraph::Node::EMPTY);
    }

    static int lua_unit_camera( lua_State* L ) {
      return lua_unit_node_typed(L, SceneGraph::Node::CAMERA);
    }

    static int lua_unit_mesh( lua_State* L ) {
      return lua_unit_node_typed(L, SceneGraph::Node::MESH);
    }

    static int lua_unit_local_position( lua_State* L ) {
      const Unit& unit = lua_checkunit(L, 1);
      *lua_newvec3(L) = unit.scene_graph().local_poses()[0].position;
      return 1;
    }

    static int lua_unit_set_local_position( lua_State* L ) {
      Unit& unit = lua_checkunit(L, 1);
      unit.scene_graph().local_poses()[0].position = *lua_checkvec3(L, 2);
      return 0;
    }

    static int lua_unit_local_rotation( lua_State* L ) {
      const Unit& unit = lua_checkunit(L, 1);
      *lua_newquat(L) = unit.scene_graph().local_poses()[0].rotation;
      return 1;
    }

    static int lua_unit_set_local_rotation( lua_State* L ) {
      Unit& unit = lua_checkunit(L, 1);
      unit.scene_graph().local_poses()[0].rotation = *lua_checkquat(L, 2);
      return 0;
    }

    static int lua_unit_local_scale( lua_State* L ) {
      const Unit& unit = lua_checkunit(L, 1);
      *lua_newvec3(L) = unit.scene_graph().local_poses()[0].scale;
      return 1;
    }

    static int lua_unit_set_local_scale( lua_State* L ) {
      Unit& unit = lua_checkunit(L, 1);
      unit.scene_graph().local_poses()[0].scale = *lua_checkvec3(L, 2);
      return 0;
    }

    static int lua_unit_is_a( lua_State* L ) {
      const Unit& unit = lua_checkunit(L, 1);
      lua_pushboolean(L, unit.type().id() == Resource::Id(UnitResource::type(), luaL_checkstring(L, 2)));
      return 1;
    }
  }
} // butane

namespace butane {
  int luaopen_unit( lua_State* L )
  {
    luaL_newmetatable(L, "Unit");
    lua_pushcfunction(L, &lua_unit_node);
    lua_setfield(L, -2, "node");
    lua_pushcfunction(L, &lua_unit_empty);
    lua_setfield(L, -2, "empty");
    lua_pushcfunction(L, &lua_unit_camera);
    lua_setfield(L, -2, "camera");
    lua_pushcfunction(L, &lua_unit_mesh);
    lua_setfield(L, -2, "mesh");
    lua_pushcfunction(L, &lua_unit_local_position);
    lua_setfield(L, -2, "local_position");
    lua_pushcfunction(L, &lua_unit_set_local_position);
    lua_setfield(L, -2, "set_local_position");
    lua_pushcfunction(L, &lua_unit_local_rotation);
    lua_setfield(L, -2, "local_rotation");
    lua_pushcfunction(L, &lua_unit_set_local_rotation);
    lua_setfield(L, -2, "set_local_rotation");
    lua_pushcfunction(L, &lua_unit_local_scale);
    lua_setfield(L, -2, "local_scale");
    lua_pushcfunction(L, &lua_unit_set_local_scale);
    lua_setfield(L, -2, "set_local_scale");
    lua_pushcfunction(L, &lua_unit_is_a);
    lua_setfield(L, -2, "is_a");
    return 1;
  }
} // butane
