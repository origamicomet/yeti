// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/scene_graph_if.h>
#include <butane/scene_graph.h>

#include <butane/script_if.h>
#include <butane/unit_if.h>

namespace butane {
  namespace {
    static int lua_node_local_position( lua_State* L ) {
      const SceneGraph::Node& node = lua_checknode(L, 1);
      *lua_newvec3(L) = node.local_pose().position;
      return 1;
    }

    static int lua_node_set_local_position( lua_State* L ) {
      SceneGraph::Node& node = lua_checknode(L, 1);
      node.local_pose().position = *lua_checkvec3(L, 2);
      node.set_moved(true);
      return 0;
    }

    static int lua_node_local_rotation( lua_State* L ) {
      const SceneGraph::Node& node = lua_checknode(L, 1);
      *lua_newquat(L) = node.local_pose().rotation;
      return 1;
    }

    static int lua_node_set_local_rotation( lua_State* L ) {
      SceneGraph::Node& node = lua_checknode(L, 1);
      node.local_pose().rotation = *lua_checkquat(L, 2);
      node.set_moved(true);
      return 0;
    }

    static int lua_node_local_scale( lua_State* L ) {
      const SceneGraph::Node& node = lua_checknode(L, 1);
      *lua_newvec3(L) = node.local_pose().scale;
      return 1;
    }

    static int lua_node_set_local_scale( lua_State* L ) {
      SceneGraph::Node& node = lua_checknode(L, 1);
      node.local_pose().scale = *lua_checkvec3(L, 2);
      node.set_moved(true);
      return 0;
    }
  }

  namespace {
    static SceneGraph::Node::Camera& lua_checkcamera( lua_State* L, int idx ) {
      SceneGraph::Node& node = lua_checknode(L, idx);
      luaL_argcheck(L, node.is_camera(), idx, "expected a Camera node");
      return *node.camera();
    }

    static int lua_camera_type( lua_State* L ) {
      const SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      switch (camera.type()) {
        case SceneGraph::Node::Camera::ORTHOGRAPHIC:
          lua_pushstring(L, "orthographic"); break;
        case SceneGraph::Node::Camera::PERSPECTIVE:
          lua_pushstring(L, "perspective"); break;
        default:
          __builtin_unreachable(); }
      return 1;
    }

    static int lua_camera_set_type( lua_State* L ) {
      SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      if (strcmp("orthographic", luaL_checkstring(L, 2)) == 0)
        camera.set_type(SceneGraph::Node::Camera::ORTHOGRAPHIC);
      else if (strcmp("perspective", luaL_checkstring(L, 2)) == 0)
        camera.set_type(SceneGraph::Node::Camera::PERSPECTIVE);
      else
        return luaL_argerror(L, 2, "expected 'orthographic' or 'perspective'");
      return 0;
    }

    static int lua_camera_near( lua_State* L ) {
      const SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      lua_pushnumber(L, camera.near());
      return 1;
    }

    static int lua_camera_set_near( lua_State* L ) {
      SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      camera.set_near(luaL_checknumber(L, 2));
      return 0;
    }

    static int lua_camera_far( lua_State* L ) {
      const SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      lua_pushnumber(L, camera.far());
      return 1;
    }

    static int lua_camera_set_far( lua_State* L ) {
      SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      camera.set_far(luaL_checknumber(L, 2));
      return 0;
    }

    static int lua_camera_view( lua_State* L ) {
      const SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      Vec2f min, max; {
        camera.view(min, max); }
      lua_pushnumber(L, min.y);
      lua_pushnumber(L, min.x);
      lua_pushnumber(L, max.y);
      lua_pushnumber(L, max.x);
      return 4;
    }

    static int lua_camera_set_view( lua_State* L ) {
      SceneGraph::Node::Camera& camera = lua_checkcamera(L, 1);
      camera.set_view(
        Vec2f(luaL_checknumber(L, 2), luaL_checknumber(L, 3)),
        Vec2f(luaL_checknumber(L, 4), luaL_checknumber(L, 5)));
      return 0;
    }
  }

  namespace {
    static SceneGraph::Node::Mesh& lua_checkmesh( lua_State* L, int idx ) {
      SceneGraph::Node& node = lua_checknode(L, idx);
      luaL_argcheck(L, node.is_mesh(), idx, "expected a Mesh node");
      return *node.mesh();
    }
  }
} // butane

namespace butane {
  int luaopen_node( lua_State* L )
  {
    luaL_newmetatable(L, "Node");
    lua_pushcfunction(L, &lua_node_local_position);
    lua_setfield(L, -2, "local_position");
    lua_pushcfunction(L, &lua_node_set_local_position);
    lua_setfield(L, -2, "set_local_position");
    lua_pushcfunction(L, &lua_node_local_rotation);
    lua_setfield(L, -2, "local_rotation");
    lua_pushcfunction(L, &lua_node_set_local_rotation);
    lua_setfield(L, -2, "set_local_rotation");
    lua_pushcfunction(L, &lua_node_local_scale);
    lua_setfield(L, -2, "local_scale");
    lua_pushcfunction(L, &lua_node_set_local_scale);
    lua_setfield(L, -2, "set_local_scale");
    return 1;
  }

  int luaopen_camera( lua_State* L )
  {
    luaL_newmetatable(L, "Camera");
    lua_pushcfunction(L, &lua_node_local_position);
    lua_setfield(L, -2, "local_position");
    lua_pushcfunction(L, &lua_node_set_local_position);
    lua_setfield(L, -2, "set_local_position");
    lua_pushcfunction(L, &lua_node_local_rotation);
    lua_setfield(L, -2, "local_rotation");
    lua_pushcfunction(L, &lua_node_set_local_rotation);
    lua_setfield(L, -2, "set_local_rotation");
    lua_pushcfunction(L, &lua_node_local_scale);
    lua_setfield(L, -2, "local_scale");
    lua_pushcfunction(L, &lua_node_set_local_scale);
    lua_setfield(L, -2, "set_local_scale");
    lua_pushcfunction(L, &lua_camera_type);
    lua_setfield(L, -2, "type");
    lua_pushcfunction(L, &lua_camera_set_type);
    lua_setfield(L, -2, "set_type");
    lua_pushcfunction(L, &lua_camera_near);
    lua_setfield(L, -2, "near");
    lua_pushcfunction(L, &lua_camera_set_near);
    lua_setfield(L, -2, "set_near");
    lua_pushcfunction(L, &lua_camera_far);
    lua_setfield(L, -2, "far");
    lua_pushcfunction(L, &lua_camera_set_far);
    lua_setfield(L, -2, "set_far");
    lua_pushcfunction(L, &lua_camera_view);
    lua_setfield(L, -2, "view");
    lua_pushcfunction(L, &lua_camera_set_view);
    lua_setfield(L, -2, "set_view");
    // lua_pushcfunction(L, &lua_camera_field_of_view);
    // lua_setfield(L, -2, "field_of_view");
    // lua_pushcfunction(L, &lua_camera_set_field_of_view);
    // lua_setfield(L, -2, "set_field_of_view");
    // lua_pushcfunction(L, &lua_camera_aspect_ratio);
    // lua_setfield(L, -2, "aspect_ratio");
    // lua_pushcfunction(L, &lua_camera_set_aspect_ratio);
    // lua_setfield(L, -2, "set_aspect_ratio");
    return 1;
  }

  int luaopen_mesh( lua_State* L )
  {
    luaL_newmetatable(L, "Mesh");
    lua_pushcfunction(L, &lua_node_local_position);
    lua_setfield(L, -2, "local_position");
    lua_pushcfunction(L, &lua_node_set_local_position);
    lua_setfield(L, -2, "set_local_position");
    lua_pushcfunction(L, &lua_node_local_rotation);
    lua_setfield(L, -2, "local_rotation");
    lua_pushcfunction(L, &lua_node_set_local_rotation);
    lua_setfield(L, -2, "set_local_rotation");
    lua_pushcfunction(L, &lua_node_local_scale);
    lua_setfield(L, -2, "local_scale");
    lua_pushcfunction(L, &lua_node_set_local_scale);
    lua_setfield(L, -2, "set_local_scale");
    return 1;
  }
} // butane
