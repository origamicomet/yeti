// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application.h>
#include <butane/application/run.h>
#include <butane/application/compile.h>

namespace butane {
namespace Application {
  void boot(
    size_t num_args,
    const char* args[] )
  {
    add_logger(&foundation::console_logger);
    add_logger(&foundation::debug_logger, (void*)"butane");
    add_logger(&foundation::file_logger, (void*)File::open("butane.log", "wb"));

    static const char* symbol_search_dirs[1] = { "symbols" };
    if (!Symbols::load(&symbol_search_dirs[0], 1))
      log("Unable to load debug symbols!\n");
    set_exception_handler(&foundation::crash_handler);

    Array<const char*> args_(Allocators::heap(), max((size_t)1, num_args) - 1);
    for (size_t i = 1; i < num_args; ++i)
      args_[i - 1] = args[i];

    if (args_.size() >= 1) {
      if (strcmp("compile", args_[0]) == 0)
        compile(args_); }

    run(args_);
  }

  const char* platform()
  {
  #if defined(FOUNDATION_PLATFORM_WINDOWS)
    return "win32";
  #elif defined(FOUNDATION_PLATFORM_MACOSX)
    return "macosx";
  #elif defined(FOUNDATION_PLATFORM_LINUX)
    return "linux";
  #elif defined(FOUNDATION_PLATFORM_FREEBSD)
    return "freebsd";
  #elif defined(FOUNDATION_PLATFORM_NETBSD)
    return "netbsd";
  #elif defined(FOUNDATION_PLATFORM_OPENBSD)
    return "openbsd";
  #elif defined(FOUNDATION_PLATFORM_DRAGONFLYBSD)
    return "dragonflybsd";
  #elif defined(FOUNDATION_PLATFORM_IOS)
    return "ios";
  #elif defined(FOUNDATION_PLATFORM_ANDROID)
    return "android";
  #elif defined(FOUNDATION_PLATFORM_NATIVE_CLIENT)
    return "nacl";
  #endif
  }

  const char* architecture()
  {
  #if defined(FOUNDATION_ARCHITECTURE_X86_64)
    return "x86_64";
  #elif defined(FOUNDATION_ARCHITECTURE_X86)
    return "x86";
  #elif defined(FOUNDATION_ARCHITECTURE_ARM)
    return "arm";
  #endif
  }

  const char* build()
  {
  #if defined(BUTANE_DEBUG_BUILD)
    return "debug";
  #elif defined(BUTANE_DEVELOPMENT_BUILD)
    return "development";
  #elif defined(BUTANE_RELEASE_BUILD)
    return "release";
  #endif
  }

  void pause()
  {
  #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_DEVELOPMENT_BUILD)
  #else
    fail("Calls to Application::pause are illegal in runtime builds!");
  #endif
  }

  void unpause()
  {
  #if defined(BUTANE_DEBUG_BUILD) || defined(BUTANE_DEVELOPMENT_BUILD)
  #else
    warn("Calls to Application::unpause have no effect in runtime builds!");
  #endif
  }

  void quit()
  {
    exit(EXIT_SUCCESS);
  }
} // Application
} // butane
