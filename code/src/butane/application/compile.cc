// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/compile.h>

#include <butane/resource.h>

namespace butane {
namespace Application {
  void compile(
    const Array<const char*>& args )
  {
    const LogScope _("Application::compile");

    if (args.size() < 2) {
      log("Malformed compiler invocation!");
      log("  Expected: compile [source data directory] [data directory] [options]");
      log("                              ^---------------------^-- not supplied");
      Application::quit(); }

    if (!Directory::exists(args[1])) {
      log("Malformed compiler invocation!");
      log("  Expected: compile [source data directory] [data directory] [option]");
      log("                              ^ directory doesn't exist");
      Application::quit(); }

    if (!Directory::exists(args[2])) {
      if (!Directory::create(args[2])) {
        log("Unable to create output directory, aka '%s'", args[2]);
        Application::quit(); }
    }

    bool daemon = false;
    for (auto iter = args.begin(); iter != args.end(); ++iter) {
      if (strcmp("-daemon", *iter) == 0) {
        daemon = true;
      } else if (strcmp("-log-to-network", *iter) == 0) {
        if ((++iter) == args.end()) {
          log("Malformed command-line argument!");
          log("  Expected: -log-to-network [address/hostname]");
          log("                                    ^ not supplied");
          Application::quit(); }
        warn("Network logging is not implemented, yet.");
      }
    }

    Resource::Compiler::run(args[1], args[2], daemon);

    Application::quit();
  }
} // Application
} // butane
