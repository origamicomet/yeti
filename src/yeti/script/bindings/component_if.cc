//===-- yeti/script/bindings/component_if.cc ------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/component_if.h"

#include "yeti/script.h"

namespace yeti {

namespace component_if {
  // TODO(mtwilliams): Associate with metatable to aid type checking.

  bool check(lua_State *L, int idx, Component::Id type) {
    if (!lua_isuserdata(L, idx))
      return false;
    return true;
  }

  const Handle &cast(lua_State *L, int idx, Component::Id type) {
    void *ud = lua_touserdata(L, idx);

  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
      YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
    if (!ud)
      luaL_typerror(L, idx, component_registry::component_by_id(type)->name);
  #endif

    return *((const Handle *)ud);
  }

  void push(lua_State *L, Component::Id type, Component::Handle handle) {
    void *ud = lua_newuserdata(L, sizeof(Handle));
    Handle *h = (Handle *)ud;
    h->type = type;
    h->handle = handle;
  }
} // component_if

void component_if::expose(Script *script) {
}

} // yeti
