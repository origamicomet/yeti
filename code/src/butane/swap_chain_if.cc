// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/swap_chain_if.h>
#include <butane/graphics/swap_chain.h>

namespace butane {
  namespace {
    static int lua_swap_chain_dimensions( lua_State* L ) {
      const SwapChain* swap_chain = (const SwapChain*)luaL_checklightuserdata(L, 1);
      lua_pushinteger(L, swap_chain->width());
      lua_pushinteger(L, swap_chain->height());
      return 2;
    }
  }
} // butane

namespace butane {
  int luaopen_swap_chain( lua_State* L )
  {
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, &lua_swap_chain_dimensions);
    lua_setfield(L, -2, "dimensions");
    return 1;
  }
} // butane
