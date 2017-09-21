//===-- yeti/script/bindings/mouse_if.h -----------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/mouse_if.h"

#include "yeti/script.h"
#include "yeti/script/environment.h"

#include "yeti/input/mouse.h"

namespace yeti {

namespace mouse_if {
  namespace {
    static const struct { const char *name; MouseButton btn; } button_mappings[MouseButtons::_COUNT] = {
      { "left",    MouseButtons::LEFT    },
      { "middle",  MouseButtons::MIDDLE  },
      { "right",   MouseButtons::RIGHT   },
      { "extra_1", MouseButtons::EXTRA_1 },
      { "extra_2", MouseButtons::EXTRA_2 }
    };

    static MouseButton to_button(lua_State *L, int idx) {
      if (!lua_isstring(L, idx))
        luaL_argerror(L, idx, "Expected a MouseButton.");

      for (u32 btn = 0; btn < MouseButtons::_COUNT; ++btn)
        if (strcmp(button_mappings[btn].name, lua_tostring(L, idx)) == 0)
          return button_mappings[btn].btn;

      luaL_argerror(L, idx, "Expected a MouseButton.");
      return MouseButtons::UNKNOWN;
    }

    static const struct { const char *name; MouseAxis axis; } axes_mappings[MouseAxes::_COUNT] = {
      { "absolute", MouseAxes::ABSOLUTE },
      { "relative", MouseAxes::RELATIVE },
      { "delta",    MouseAxes::DELTA    },
      { "wheel",    MouseAxes::WHEEL    },
    };

    static MouseAxis to_axis(lua_State *L, int idx) {
      if (!lua_isstring(L, idx))
        luaL_argerror(L, idx, "Expected a MouseAxis.");

      for (u32 axis = 0; axis < MouseAxes::_COUNT; ++axis)
        if (strcmp(axes_mappings[axis].name, lua_tostring(L, idx)) == 0)
          return axes_mappings[axis].axis;

      luaL_argerror(L, idx, "Expected a MouseAxis.");
      return MouseAxes::UNKNOWN;
    }

    static int connected(lua_State *L) {
      lua_pushboolean(L, Mouse::connected());
      return 1;
    }

    static int disconnected(lua_State *L) {
      lua_pushboolean(L, Mouse::disconnected());
      return 1;
    }

    static int axis(lua_State *L) {
      Vec3 *v = Script::recover(L)->environment()->allocate<Vec3>();
      *v = Mouse::axis(to_axis(L, 1));
      lua_pushlightuserdata(L, (void *)v);
      return 1;
    }

    static int pressed(lua_State *L) {
      lua_pushboolean(L, Mouse::pressed(to_button(L, 1)));
      return 1;
    }

    static int held(lua_State *L) {
      lua_pushboolean(L, Mouse::held(to_button(L, 1)));
      return 1;
    }

    static int released(lua_State *L) {
      lua_pushboolean(L, Mouse::released(to_button(L, 1)));
      return 1;
    }
  }
} // mouse_if

void mouse_if::expose(Script *script) {
  script->add_module("Mouse");

  script->add_module_function("Mouse", "connected", &connected);
  script->add_module_function("Mouse", "disconnected", &disconnected);

  script->add_module_function("Mouse", "axis", &axis);

  script->add_module_function("Mouse", "pressed", &pressed);
  script->add_module_function("Mouse", "held", &held);
  script->add_module_function("Mouse", "released", &released);
}

} // yeti
