// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application_if.h>
#include <butane/application.h>

namespace butane {
  namespace script_interface {
    static size_t platform(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.platform");
      script.stack().push(Application::platform());
      return 1;
    }

    static size_t architecture(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.architecture");
      script.stack().push(Application::architecture());
      return 1;
    }

    static size_t build(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.build");
      script.stack().push(Application::build());
      return 1;
    }

    static size_t pause(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.pause");
      Application::pause();
      return 0;
    }

    static size_t unpause(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.unpause");
      Application::unpause();
      return 0;
    }

    static size_t quit(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.quit");
      Application::quit();
      __builtin_unreachable();
      return 0;
    }
  } // script_interface
} // butane

namespace butane {
namespace Application {
  void expose(
    butane::Script& script )
  {
    script.expose("Application.platform", &script_interface::platform);
    script.expose("Application.architecture", &script_interface::architecture);
    script.expose("Application.build", &script_interface::build);
    script.expose("Application.pause", &script_interface::pause);
    script.expose("Application.unpause", &script_interface::unpause);
    script.expose("Application.quit", &script_interface::quit);
  }
} // Application
} // butane
