// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCRIPT_INTERFACE_H_
#define _BUTANE_SCRIPT_INTERFACE_H_

#include <butane/butane.h>
#include <butane/script.h>
#include <butane/math.h>

namespace butane {
  extern BUTANE_EXPORT Vec2f* lua_newvec2( lua_State* L );
  extern BUTANE_EXPORT Vec2f* lua_checkvec2( lua_State* L, int idx );
  extern BUTANE_EXPORT Vec3f* lua_newvec3( lua_State* L );
  extern BUTANE_EXPORT Vec3f* lua_checkvec3( lua_State* L, int idx );
  extern BUTANE_EXPORT Vec4f* lua_newvec4( lua_State* L );
  extern BUTANE_EXPORT Vec4f* lua_checkvec4( lua_State* L, int idx );
  extern BUTANE_EXPORT Quatf* lua_newquat( lua_State* L );
  extern BUTANE_EXPORT Quatf* lua_checkquat( lua_State* L, int idx );
  extern BUTANE_EXPORT int luaopen_script( lua_State* L );
} // butane

#endif // _BUTANE_SCRIPT_INTERFACE_H_
