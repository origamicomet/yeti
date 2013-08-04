// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/window_if.h>
#include <butane/window.h>

#include <butane/application.h>
#include <butane/graphics/swap_chain.h>
#include <butane/tied_resources.h>

namespace butane {
  namespace {
    static int lua_window_open( lua_State* L ) {
      Window* window;
      SwapChain* swap_chain;
      TiedResources* swap_chain_and_resources;
      switch (lua_gettop(L)) {
        case 3:
          Application::create_window_and_swap_chain_and_resources(
            luaL_checkstring(L, 1),
            luaL_checkinteger(L, 2), luaL_checkinteger(L, 3),
            window, swap_chain, swap_chain_and_resources);
          break;
        default:
          return luaL_error(L, "expected three or four arguments"); }
      lua_pushlightuserdata(L, (void*)window);
      return 1;
    }

    static int lua_window_close( lua_State* L ) {
      Application::destroy_window_and_swap_chain_and_resources(
        (Window*)luaL_checklightuserdata(L, 1));
      return 0;
    }

    static int lua_window_show( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          Application::windows()[0]->show();
          break;
        case 1:
          ((Window*)luaL_checklightuserdata(L, 1))->show();
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 0;
    }

    static int lua_window_hide( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          Application::windows()[0]->hide();
          break;
        case 1:
          ((Window*)luaL_checklightuserdata(L, 1))->hide();
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 0;
    }

    static int lua_window_minimize( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          Application::windows()[0]->minimize();
          break;
        case 1:
          ((Window*)luaL_checklightuserdata(L, 1))->minimize();
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 0;
    }

    static int lua_window_maximize( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          Application::windows()[0]->maximize();
          break;
        case 1:
          ((Window*)luaL_checklightuserdata(L, 1))->maximize();
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 0;
    }

    static int lua_window_focused( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          lua_pushboolean(L, Application::windows()[0]->focused());
          break;
        case 1:
          lua_pushboolean(L, ((Window*)luaL_checklightuserdata(L, 1))->focused());
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 1;
    }

    static int lua_window_title( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          lua_pushstring(L, Application::windows()[0]->title().raw());
          break;
        case 1:
          lua_pushstring(L, ((Window*)luaL_checklightuserdata(L, 1))->title().raw());
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 1;
    }

    static int lua_window_set_title( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 1:
          Application::windows()[0]->set_title(luaL_checkstring(L, 1));
          break;
        case 2:
          ((Window*)luaL_checklightuserdata(L, 1))->set_title(luaL_checkstring(L, 2));
          break;
        default:
          return luaL_error(L, "expected one or two arguments"); }
      return 0;
    }

    static int lua_window_dimensions( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          lua_pushinteger(L, Application::windows()[0]->width());
          lua_pushinteger(L, Application::windows()[0]->height());
          break;
        case 1:
          lua_pushinteger(L, ((Window*)luaL_checklightuserdata(L, 1))->width());
          lua_pushinteger(L, ((Window*)luaL_checklightuserdata(L, 1))->height());
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 2;
    }

    static int lua_window_set_dimensions( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 2:
          Application::windows()[0]->set_dimensions(
            luaL_checkinteger(L, 1),
            luaL_checkinteger(L, 2));
          break;
        case 3:
          ((Window*)luaL_checklightuserdata(L, 1))->set_dimensions(
            luaL_checkinteger(L, 2),
            luaL_checkinteger(L, 3));
          break;
        default:
          return luaL_error(L, "expected two or three arguments"); }
      return 0;
    }

    static int lua_window_fullscreen( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 0:
          lua_pushboolean(L, Application::windows()[0]->fullscreen());
          break;
        case 1:
          lua_pushboolean(L, ((Window*)luaL_checklightuserdata(L, 1))->fullscreen());
          break;
        default:
          return luaL_error(L, "expected zero or one arguments"); }
      return 1;
    }

    static int lua_window_set_fullscreen( lua_State* L ) {
      switch (lua_gettop(L)) {
        case 1:
          Application::windows()[0]->set_fullscreen(luaL_checkboolean(L, 1));
          break;
        case 2:
          ((Window*)luaL_checklightuserdata(L, 1))->set_fullscreen(luaL_checkboolean(L, 2));
          break;
        default:
          return luaL_error(L, "expected one or two arguments"); }
      return 0;
    }
  }
} // butane

namespace butane {
  int luaopen_window( lua_State* L )
  {
    lua_createtable(L, 0, 12);
    lua_pushcfunction(L, &lua_window_open);
    lua_setfield(L, -2, "open");
    lua_pushcfunction(L, &lua_window_close);
    lua_setfield(L, -2, "close");
    lua_pushcfunction(L, &lua_window_show);
    lua_setfield(L, -2, "show");
    lua_pushcfunction(L, &lua_window_hide);
    lua_setfield(L, -2, "hide");
    lua_pushcfunction(L, &lua_window_minimize);
    lua_setfield(L, -2, "minimize");
    lua_pushcfunction(L, &lua_window_maximize);
    lua_setfield(L, -2, "maximize");
    lua_pushcfunction(L, &lua_window_focused);
    lua_setfield(L, -2, "focused");
    lua_pushcfunction(L, &lua_window_title);
    lua_setfield(L, -2, "title");
    lua_pushcfunction(L, &lua_window_set_title);
    lua_setfield(L, -2, "set_title");
    lua_pushcfunction(L, &lua_window_dimensions);
    lua_setfield(L, -2, "dimensions");
    lua_pushcfunction(L, &lua_window_set_dimensions);
    lua_setfield(L, -2, "set_dimensions");
    lua_pushcfunction(L, &lua_window_fullscreen);
    lua_setfield(L, -2, "fullscreen");
    lua_pushcfunction(L, &lua_window_set_fullscreen);
    lua_setfield(L, -2, "set_fullscreen");
    return 1;
  }
} // butane
