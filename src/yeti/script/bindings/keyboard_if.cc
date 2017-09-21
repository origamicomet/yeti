//===-- yeti/script/bindings/keyboard_if.h --------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/keyboard_if.h"

#include "yeti/script.h"

#include "yeti/input/keyboard.h"

namespace yeti {

namespace keyboard_if {
  namespace {
    static const struct { const char *name; Key key; } key_mappings[Keys::_COUNT] = {
      #define _(_, Mnemonic, Name) { Name, Keys::Mnemonic },
        #include "yeti/input/keys.inl"
      #undef _
    };

    // OPTIMIZE(mtwilliams): Allow this to be cached in Lua?
    static Key to_key(lua_State *L, int idx) {
      if (!lua_isstring(L, idx))
        luaL_argerror(L, idx, "Expected a Key.");

      for (u32 key = 0; key < Keys::_COUNT; ++key)
        if (strcmp(key_mappings[key].name, lua_tostring(L, idx)) == 0)
          return key_mappings[key].key;

      luaL_argerror(L, idx, "Expected a Key.");
      return Keys::UNKNOWN;
    }

    static int connected(lua_State *L) {
      lua_pushboolean(L, Keyboard::connected());
      return 1;
    }

    static int disconnected(lua_State *L) {
      lua_pushboolean(L, Keyboard::disconnected());
      return 1;
    }

    static int pressed(lua_State *L) {
      lua_pushboolean(L, Keyboard::pressed(to_key(L, 1)));
      return 1;
    }

    static int held(lua_State *L) {
      lua_pushboolean(L, Keyboard::held(to_key(L, 1)));
      return 1;
    }

    static int released(lua_State *L) {
      lua_pushboolean(L, Keyboard::released(to_key(L, 1)));
      return 1;
    }
  }
} // keyboard_if

void keyboard_if::expose(Script *script) {
  script->add_module("Keyboard");

  script->add_module_function("Keyboard", "connected", &connected);
  script->add_module_function("Keyboard", "disconnected", &disconnected);

  script->add_module_function("Keyboard", "pressed", &pressed);
  script->add_module_function("Keyboard", "held", &held);
  script->add_module_function("Keyboard", "released", &released);
}

} // yeti
