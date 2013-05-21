// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/run.h>

#include <butane/butane.h>
#include <butane/script.h>
#include <butane/lua/script.h>
#include <butane/resource.h>
#include <butane/resource/config.h>

namespace butane {
namespace Application {
  static void on_compile_log(
    void* closure,
    const char* format, ... )
  {
  }

  void run(
    const Array<const char*>& args )
  {
    const Resource::Compiler::Status result = Resource::Compiler::compile(
      "data", "data_src",
      "data_src/manifest.config",
      &on_compile_log);

    ConfigResource* manifest = 
      (ConfigResource*)Resource::load(ConfigResource::type, "manifest");
    manifest->reference();

    const char* saves_directory;
    if (manifest->find("application.win32.saves_directory", saves_directory))
      log("saves_directory = '%s'", saves_directory);

    manifest->dereference();

    for(;;);
  }
} // Application
} // butane
