// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SCRIPT_INTERFACE_H_
#define _BUTANE_SCRIPT_INTERFACE_H_

#include <butane/butane.h>
#include <butane/script.h>

namespace butane {
  extern BUTANE_EXPORT int luaopen_script( lua_State* L );
} // butane

#endif // _BUTANE_SCRIPT_INTERFACE_H_
