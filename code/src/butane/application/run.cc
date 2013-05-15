// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/run.h>

#include <butane/butane.h>
#include <butane/script.h>
#include <butane/lua/script.h>

namespace butane {
namespace Application {
  void run(
    const Array<const char*>& args )
  {
    Lua::Script script;
    butane::expose(script);
    log("Hello, World!\n");
    static const char code[] = "log('Hello, World!\\n')\n--Application.quit()";
    script.load("example", &code[0], sizeof(code) - 1);
  }
} // Application
} // butane
