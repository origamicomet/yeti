//===-- yeti/script/window_if.cc --------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/window_if.h"

#include "yeti/script.h"
#include "yeti/application.h"
#include "yeti/script/application_if.h"
#include "yeti/window.h"
#include "yeti/input.h"

namespace yeti {

template <>
bool script_if::is_a<Window>(lua_State *L, int idx) {
  Window *window = (Window *)lua_touserdata(L, idx);
  if (window == NULL)
    // If it's not a pointer, then there's no way it's a `Window *`.
    return false;

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  // Since all our windows are stored in |app|, it's easy enough to check if
  // the given pointer is indeed to a `yeti::Window`.
  Application *app = application(L);
  for (Window **I = app->windows().first(); I <= app->windows().last(); ++I)
    if (window == *I)
      return true;

  return false;
#else
  // We'll assume everything's good. It's been tested, right?
  return true;
#endif
}

template <>
Window *script_if::to_a<Window>(lua_State *L, int idx) {
  if (!is_a<Window>(L, idx))
    luaL_argerror(L, idx, "Expected a light user-data reference to a yeti::Window.");
  return (Window *)lua_touserdata(L, idx);
}

namespace window_if {
  namespace {
    static int open(lua_State *L) {
      Application *app = script_if::application(L);
      if (!lua_istable(L, 1))
        luaL_argerror(L, 1, "Expected a table with `title`, `width`, and `height`.");

      lua_getfield(L, 1, "title");
      if (!lua_isstring(L, -1))
        return luaL_argerror(L, 1, "Expected `title` to be a string.");
      const char *title = lua_tostring(L, -1);

      lua_getfield(L, 1, "width");
      if (!lua_isnumber(L, -1))
        return luaL_argerror(L, 1, "Expected `width` to be a number.");
      if (lua_tonumber(L, -1) <= (lua_Number)0)
        return luaL_argerror(L, 1, "Expected `width` to be a positive integer.");
      const u32 width = lua_tonumber(L, -1);

      lua_getfield(L, 1, "height");
      if (!lua_isnumber(L, -1))
        return luaL_argerror(L, 1, "Expected `height` to be a number.");
      if (lua_tonumber(L, -1) <= (lua_Number)0)
        return luaL_argerror(L, 1, "Expected `height` to be a positive integer.");
      const u32 height = lua_tonumber(L, -1);

      Window *window = Window::open({title, {width, height}});
      yeti::input::from(window);
      app->windows().push(window);
      lua_pushlightuserdata(L, (void *)window);
      return 1;
    }

    static int close(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->close();
      return 0;
    }

    static int show(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->show();
      return 0;
    }

    static int hide(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->hide();
      return 0;
    }

    static int minimize(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->minimize();
      return 0;
    }

    static int maximize(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->maximize();
      return 0;
    }

    static int clip(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->clip();
      return 0;
    }

    static int unclip(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->unclip();
      return 0;
    }

    static int restore(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      window->restore();
      return 0;
    }

    static int title(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      char title[256];
      window->title(title);
      lua_pushstring(L, title);
      return 1;
    }

    static int set_title(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      if (!lua_isstring(L, 2))
        return luaL_argerror(L, 2, "Expected `title` to be a string.");
      window->set_title(lua_tostring(L, 2));
      return 0;
    }

    static int has_keyboard_focus(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      lua_pushboolean(L, window->keyboard_focus());
      return 1;
    }

    static int set_keyboard_focus(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      if (!lua_isboolean(L, 2))
        return luaL_argerror(L, 2, "Expected `focus` to be a boolean.");
      window->set_keyboard_focus(lua_toboolean(L, 2));
      return 0;
    }

    static int has_mouse_focus(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      lua_pushboolean(L, window->mouse_focus());
      return 1;
    }

    static int set_mouse_focus(lua_State *L) {
      Window *window = script_if::to_a<Window>(L, 1);
      if (!lua_isboolean(L, 2))
        return luaL_argerror(L, 2, "Expected `focus` to be a boolean.");
      window->set_mouse_focus(lua_toboolean(L, 2));
      return 0;
    }
  }
} // window_if

void window_if::expose(Script *script) {
  script->add_module("Window");

  script->add_module_function("Window", "open", &open);
  script->add_module_function("Window", "close", &close);

  script->add_module_function("Window", "show", &show);
  script->add_module_function("Window", "hide", &hide);

  script->add_module_function("Window", "minimize", &minimize);
  script->add_module_function("Window", "maximize", &maximize);

  script->add_module_function("Window", "restore", &restore);

  // TODO(mtwilliams): Custom cursors.
  // script->add_module_function("Window", "cursor", &cursor);
  // script->add_module_function("Window", "set_cursor", &set_cursor);

  script->add_module_function("Window", "clip", &clip);
  script->add_module_function("Window", "unclip", &unclip);

  script->add_module_function("Window", "title", &title);
  script->add_module_function("Window", "set_title", &set_title);

  script->add_module_function("Window", "has_keyboard_focus", &has_keyboard_focus);
  script->add_module_function("Window", "set_keyboard_focus", &set_keyboard_focus);

  script->add_module_function("Window", "has_mouse_focus", &has_mouse_focus);
  script->add_module_function("Window", "set_mouse_focus", &set_mouse_focus);
}

} // yeti
