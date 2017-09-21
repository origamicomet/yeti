//===-- yeti/script/bindings/viewport_if.cc -------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/viewport_if.h"

#include "yeti/script.h"

// To recover `Application *` from `lua_State *`.
#include "yeti/application.h"
#include "yeti/script/bindings/application_if.h"

namespace yeti {

namespace viewport_if {
  namespace {
    static int create(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int destroy(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }
  }
} // viewport_if

void viewport_if::expose(Script *script) {
  script->add_module("Viewport");

  script->add_module_function("Viewport", "create", &create);
  script->add_module_function("Viewport", "destroy", &destroy);
}

} // yeti
