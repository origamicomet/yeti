// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/butane.h>

#include <butane/script.h>
#include <butane/application_if.h>

namespace butane {
  namespace script_interface {
    static size_t log(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments == 0)
        return 0;
      const char* msg;
        arguments.to(1, msg);
      LogScope log_scope("Script");
      foundation::log("%s", msg);
      return 0;
    }

    static size_t warn(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments == 0)
        return 0;
      const char* msg;
        arguments.to(1, msg);
      LogScope log_scope("Script");
      butane::warn("%s", msg);
      return 0;
    }

    static size_t fail(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments == 0)
        butane::fail("Unspecified failure.");
      const char* msg;
        arguments.to(1, msg);
      LogScope log_scope("Script");
      butane::fail("%s", msg);
      __builtin_unreachable();
      return 0;
    }
  } // script_interface
} // butane

namespace butane {
  void expose(
    butane::Script& script )
  {
    script.expose("log", &script_interface::log);
    script.expose("warn", &script_interface::warn);
    script.expose("fail", &script_interface::fail);

    Application::expose(script);
  }
} // butane
