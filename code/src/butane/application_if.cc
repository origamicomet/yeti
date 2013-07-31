// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application_if.h>
#include <butane/application.h>

namespace butane {
  namespace {
    static int lua_application_platform( lua_State* L ) {
      lua_pushstring(L, Application::platform());
      return 1;
    }

    static int lua_application_architecture( lua_State* L ) {
      lua_pushstring(L, Application::architecture());
      return 1;
    }

    static int lua_application_build( lua_State* L ) {
      lua_pushstring(L, Application::build());
      return 1;
    }

    static int lua_application_pause( lua_State* L ) {
      Application::pause();
      return 0;
    }

    static int lua_application_unpause( lua_State* L ) {
      Application::unpause();
      return 0;
    }

    static int lua_application_quit( lua_State* L ) {
      Application::quit();
      return 0;
    }
  }
} // butane

namespace butane {
  int luaopen_application( lua_State* L )
  {
    lua_createtable(L, 0, 6);
    lua_pushcfunction(L, &lua_application_platform);
    lua_setfield(L, -2, "platform");
    lua_pushcfunction(L, &lua_application_architecture);
    lua_setfield(L, -2, "architecture");
    lua_pushcfunction(L, &lua_application_build);
    lua_setfield(L, -2, "build");
    lua_pushcfunction(L, &lua_application_pause);
    lua_setfield(L, -2, "pause");
    lua_pushcfunction(L, &lua_application_unpause);
    lua_setfield(L, -2, "unpause");
    lua_pushcfunction(L, &lua_application_quit);
    lua_setfield(L, -2, "quit");
    return 1;
  }
} // butane
