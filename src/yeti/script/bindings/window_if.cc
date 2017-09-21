//===-- yeti/script/bindings/window_if.cc ---------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/bindings/window_if.h"

#include "yeti/script.h"

// To recover `Application *` from `lua_State *`.
#include "yeti/application.h"
#include "yeti/script/bindings/application_if.h"

#include "yeti/window.h"

namespace yeti {

template <> bool Script::is_a<Window *>(int index) {
  Window *window = (Window *)lua_touserdata(L, index);

  if (window == NULL)
    // If it's not a pointer, then there's no way it's a pointer to a `Window`.
    return false;

#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
    YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  Application *app = application_if::instance(L);

  // Since all our windows are referenced by our application, it's easy enough
  // to check if the given pointer is indeed to a `Window`.
  if (app->windows().find(window))
    // Bingo!
    return true;

  return false;
#else
  // We'll assume everything's good. It's been tested, right?
  return true;
#endif
}

template <> Window *Script::to_a<Window *>(int index) {
  if (!is_a<Window *>(index))
    luaL_argerror(L, index, "Expected a light user-data reference to a `Window`.");

  return (Window *)lua_touserdata(L, index);
}

namespace window_if {
  namespace {
    static int open(lua_State *L) {
      Script *script = Script::recover(L);
      Application *app = application_if::instance(L);

      Window::Description wd; {
        if (!lua_istable(L, 1))
          luaL_argerror(L, 1, "Expected a table with `title`, `width`, and `height`.");

        lua_getfield(L, 1, "title");
        if (!lua_isstring(L, -1))
          return luaL_argerror(L, 1, "Expected `title` to be a string.");
        wd.title = lua_tostring(L, -1);

        lua_getfield(L, 1, "width");
        if (!lua_isnumber(L, -1))
          return luaL_argerror(L, 1, "Expected `width` to be a number.");
        if (lua_tonumber(L, -1) <= (lua_Number)0)
          return luaL_argerror(L, 1, "Expected `width` to be a positive integer.");
        wd.dimensions.width = lua_tonumber(L, -1);

        lua_getfield(L, 1, "height");
        if (!lua_isnumber(L, -1))
          return luaL_argerror(L, 1, "Expected `height` to be a number.");
        if (lua_tonumber(L, -1) <= (lua_Number)0)
          return luaL_argerror(L, 1, "Expected `height` to be a positive integer.");
        wd.dimensions.height = lua_tonumber(L, -1);

        lua_getfield(L, 1, "resizeable");
        if (lua_isboolean(L, -1))
          wd.resizeable = !!lua_toboolean(L, -1);
        else if (lua_isnil(L, -1))
          // Windows are resizeable by default.
          wd.resizeable = true;
        else
          return luaL_argerror(L, 1, "Expected `resizeable` to be a boolean.");

        lua_getfield(L, 1, "closable");
        if (lua_isboolean(L, -1))
          wd.closable = !!lua_toboolean(L, -1);
        else if (lua_isnil(L, -1))
          // Windows are closable by default.
          wd.closable = true;
        else
          return luaL_argerror(L, 1, "Expected `closable` to be a boolean.");

        lua_pop(L, 5);
      }

      Window *window = app->open_a_window(wd);

      lua_getfield(L, 1, "hidden");
      if (lua_isboolean(L, -1)) {
        if (!lua_toboolean(L, -1))
          window->show();
      } else if (lua_isnil(L, -1)) {
        // Windows are visible by default.
        window->show();
      } else {
        return luaL_argerror(L, 1, "Expected `hidden` to be a boolean.");
      }

      lua_pop(L, 1);

      lua_pushlightuserdata(L, (void *)window);

      return 1;
    }

    static int close(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      window->close();

      return 0;
    }

    static int show(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      window->show();

      return 0;
    }

    static int hide(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      window->hide();

      return 0;
    }

    static int minimize(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      window->minimize();

      return 0;
    }

    static int maximize(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      window->maximize();

      return 0;
    }

    static int clip(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);
      window->clip();
      return 0;
    }

    static int unclip(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      window->unclip();

      return 0;
    }

    static int restore(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      window->restore();

      return 0;
    }

    static int title(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      char title[256];
      window->title(title);

      lua_pushstring(L, title);

      return 1;
    }

    static int set_title(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      if (!lua_isstring(L, 2))
        return luaL_argerror(L, 2, "Expected `title` to be a string.");

      window->set_title(lua_tostring(L, 2));

      return 0;
    }

    static int has_keyboard_focus(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      lua_pushboolean(L, window->keyboard_focus());

      return 1;
    }

    static int set_keyboard_focus(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      if (!lua_isboolean(L, 2))
        return luaL_argerror(L, 2, "Expected `focus` to be a boolean.");

      window->set_keyboard_focus(!!lua_toboolean(L, 2));

      return 0;
    }

    static int has_mouse_focus(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      lua_pushboolean(L, window->mouse_focus());

      return 1;
    }

    static int set_mouse_focus(lua_State *L) {
      Script *script = Script::recover(L);
      Window *window = script->to_a<Window *>(1);

      if (!lua_isboolean(L, 2))
        return luaL_argerror(L, 2, "Expected `focus` to be a boolean.");

      window->set_mouse_focus(!!lua_toboolean(L, 2));

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

  // TODO(mtwilliams): Custom icons.
  // script->add_module_function("Window", "icon", &icon);
  // script->add_module_function("Window", "set_icon", &set_icon);

  script->add_module_function("Window", "title", &title);
  script->add_module_function("Window", "set_title", &set_title);

  script->add_module_function("Window", "has_keyboard_focus", &has_keyboard_focus);
  script->add_module_function("Window", "set_keyboard_focus", &set_keyboard_focus);

  script->add_module_function("Window", "has_mouse_focus", &has_mouse_focus);
  script->add_module_function("Window", "set_mouse_focus", &set_mouse_focus);
}

} // yeti
