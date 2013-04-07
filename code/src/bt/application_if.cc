// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/application_if.h>

namespace bt {
namespace ScriptInterface {
  static size_t quit( bt::Script* script, bt::Script::Arguments& args ) {
    (void)script;
    if (args > 0)
      log("Superfluous arguments ignored in call to Application.quit\n");
    bt::Application::quit();
    return 0;
  }

  namespace Application {
    void expose( bt::Script* script ) {
      script->expose("Application.quit", &quit);
    }
  } // Application
} // ScriptInterface
} // bt