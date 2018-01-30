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

namespace yeti {

namespace entity_if {
  // TODO(mtwilliams): Associate with metatable to aid type checking.

  bool check(lua_State *L, int idx) {
    if (!lua_isuserdata(L, idx))
      return false;
    return true;
  }

  const Handle &cast(lua_State *L, int idx) {
    void *ud = lua_touserdata(L, idx);

  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
      YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
    if (!ud)
      luaL_typerror(L, idx, "Entity");
  #endif

    return *((const Handle *)ud);
  }

  void push(lua_State *L, const Handle &handle) {
    void *ud = lua_newuserdata(L, sizeof(Handle));
    *((Handle *)ud) = handle;
  }
} // entity_if

void entity_if::expose(Script *script) {
}

} // yeti
