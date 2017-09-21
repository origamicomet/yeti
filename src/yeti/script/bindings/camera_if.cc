//===-- yeti/script/bindings/camera_if.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/camera_if.h"

#include "yeti/script.h"

namespace yeti {

namespace camera_if {
  namespace {
    static int create(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int destroy(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int lookup(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int describe(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int set_type(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int set_field_of_view(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int set_box(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int set_near_plane(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }

    static int set_far_plane(lua_State *L) {
      luaL_error(L, "Not implemented yet.");
      return 0;
    }
  }
} // camera_if

void camera_if::expose(Script *script) {
  script->add_module("Camera");

  script->add_module_function("Camera", "create", &create);
  script->add_module_function("Camera", "destroy", &destroy);

  script->add_module_function("Camera", "lookup", &lookup);

  script->add_module_function("Camera", "describe", &describe);

  script->add_module_function("Camera", "set_type", &set_type);

  script->add_module_function("Camera", "set_field_of_view", &set_field_of_view);
  script->add_module_function("Camera", "set_box", &set_box);

  script->add_module_function("Camera", "set_near_plane", &set_near_plane);
  script->add_module_function("Camera", "set_far_plane", &set_far_plane);
}

} // yeti
