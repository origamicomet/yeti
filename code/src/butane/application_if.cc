// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application_if.h>
#include <butane/application.h>

namespace butane {
namespace script_interface {
  namespace Application {
    static size_t platform(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.platform");
      script.stack().push(butane::Application::platform());
      return 1;
    }

    static size_t architecture(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.architecture");
      script.stack().push(butane::Application::architecture());
      return 1;
    }

    static size_t build(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.build");
      script.stack().push(butane::Application::build());
      return 1;
    }

    static size_t pause(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.pause");
      butane::Application::pause();
      return 0;
    }

    static size_t unpause(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.unpause");
      butane::Application::unpause();
      return 0;
    }

    static size_t quit(
      Script& script,
      const Script::Arguments& arguments )
    {
      (void)script;
      if (arguments > 0)
        log("Superfluous arguments ignored in call to Application.quit");
      butane::Application::quit();
      __builtin_unreachable();
      return 0;
    }
  } // Application
} // script_interface
} // butane

namespace butane {
namespace script_interface {
namespace Application {
  void expose(
    butane::Script& script )
  {
    script.expose("Application.platform", &platform);
    script.expose("Application.architecture", &architecture);
    script.expose("Application.build", &build);
    script.expose("Application.pause", &pause);
    script.expose("Application.unpause", &unpause);
    script.expose("Application.quit", &quit);
  }
} // Application
} // script_interface
} // butane
