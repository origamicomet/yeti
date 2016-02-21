//===-- yeti/script/application_if.cc ---------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/script/application_if.h"

#include "yeti/script.h"
#include "yeti/application.h"
#include "yeti/window.h"
#include "yeti/time_step_policy.h"

namespace yeti {

namespace application_if {
  namespace {
    static Application *instance(lua_State *L) {
      // TODO(mtwilliams): Refactor singleton management into yeti::Script.
      lua_getglobal(L, "Application");
      lua_getfield(L, -1, "__instance__");
      yeti_assertf_development(lua_islightuserdata(L, -1), "Expected Application.__instance__ to be a light user-data reference to a yeti::Application.");
      Application *app = (Application *)lua_touserdata(L, -1);
      lua_pop(L, 2);
      return app;
    }

    static int platform(lua_State *L) {
      lua_pushstring(L, Application::platform());
      return 1;
    }

    static int architecture(lua_State *L) {
      lua_pushstring(L, Application::architecture());
      return 1;
    }

    static int build(lua_State *L) {
      lua_pushstring(L, Application::build());
      return 1;
    }

    static int pause(lua_State *L) {
      Application *app = instance(L);
      app->pause();
      return 0;
    }

    static int unpause(lua_State *L) {
      Application *app = instance(L);
      app->unpause();
      return 0;
    }

    static int quit(lua_State *L) {
      Application *app = instance(L);
      app->quit();
      return 0;
    }

    static int window(lua_State *L) {
      Application *app = instance(L);
      lua_pushlightuserdata(L, (void *)app->windows()[0]);
      return 1;
    }

    static int windows(lua_State *L) {
      Application *app = instance(L);
      yeti_assertf(0, "Not implemented yet.");
      return 0;
    }

    static int time_step_policy(lua_State *L) {
      Application *app = instance(L);
      const TimeStepPolicy::Description &time_step_policy_desc = app->time_step_policy()->desc();

      switch (time_step_policy_desc.type) {
        case TimeStepPolicy::VARIABLE: {
          lua_pushstring(L, "variable");
        } break;

        case TimeStepPolicy::FIXED: {
          lua_pushstring(L, "fixed");
        } break;

        case TimeStepPolicy::SMOOTHED: {
          if (!time_step_policy_desc.config.smoothed.payback)
            lua_pushstring(L, "smoothed");
          else
            lua_pushstring(L, "smoothed_with_debt_payback");
        } break;

        default:
          YETI_UNREACHABLE();
      }

      return 1;
    }

    static int set_time_step_policy(lua_State *L) {
      Application *app = instance(L);

      TimeStepPolicy::Description time_step_policy_desc; {
        const char *policy = luaL_checkstring(L, 1);
        if (strcmp("variable", policy) == 0) {
          time_step_policy_desc.type = TimeStepPolicy::VARIABLE;
        } else if (strcmp("fixed", policy) == 0) {
          time_step_policy_desc.type = TimeStepPolicy::FIXED;
          time_step_policy_desc.config.fixed.delta_time_per_step = luaL_checknumber(L, 2);
        } else if (strcmp("smoothed", policy) == 0) {
          yeti_assertf(0, "Not implemented yet.");
        } else if (strcmp("smoothed_with_debt_payback", policy) == 0) {
          yeti_assertf(0, "Not implemented yet.");
        } else {
          luaL_argerror(L, 1, "expected 'variable', 'fixed', 'smoothed', or 'smoothed_with_debt_payback'");
        }
      }

      app->set_time_step_policy(TimeStepPolicy::create(time_step_policy_desc));

      return 0;
    }
  }
} // application_if

void application_if::expose(Script *script, Application *app) {
  script->add_module("Application");

  lua_getglobal(script->state(), "Application");
  lua_pushlightuserdata(script->state(), (void *)app);
  lua_setfield(script->state(), -2, "__instance__");
  lua_pop(script->state(), 1);

  script->add_module_function("Application", "platform", &platform);
  script->add_module_function("Application", "architecture", &architecture);
  script->add_module_function("Application", "build", &build);

  script->add_module_function("Application", "pause", &pause);
  script->add_module_function("Application", "unpause", &unpause);

  script->add_module_function("Application", "quit", &quit);

  script->add_module_function("Application", "window", &window);
  script->add_module_function("Application", "windows", &windows);

  script->add_module_function("Application", "time_step_policy", &time_step_policy);
  script->add_module_function("Application", "set_time_step_policy", &set_time_step_policy);
}

} // yeti
