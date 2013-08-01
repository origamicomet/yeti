// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_UNIT_INTERFACE_H_
#define _BUTANE_UNIT_INTERFACE_H_

#include <butane/butane.h>
#include <butane/script.h>
#include <butane/unit.h>
#include <butane/scene_graph.h>

namespace butane {
  extern BUTANE_EXPORT Unit::Reference* lua_newunitref( lua_State* L );
  extern BUTANE_EXPORT Unit& lua_checkunit( lua_State* L, int idx );
  extern BUTANE_EXPORT SceneGraph::Node& lua_checknode( lua_State* L, int idx );
  extern BUTANE_EXPORT int luaopen_unit( lua_State* L );
} // butane

#endif // _BUTANE_UNIT_INTERFACE_H_
