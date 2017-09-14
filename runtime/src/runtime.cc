//===-- yeti/runtime.cc ---------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti.h"

#include "yeti/runtime/standard_application.h"
#include "yeti/runtime/manifest.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #include <crtdbg.h>
#endif

static void debug() {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

  // Check memory integrity every time memory is allocated or freed.
  flags |= _CRTDBG_CHECK_ALWAYS_DF;

  // Check for memory leaks at program exit.
  flags |= _CRTDBG_LEAK_CHECK_DF;

  _CrtSetDbgFlag(flags);
#endif
}

static yeti::Config config_from_manifest(yeti::runtime::Manifest *manifest) {
  yeti::Config config;

  config.resources.database = manifest->resources.database;

  // Spawn a worker thread for each logical core, sans one for the main thread.
  config.threading.workers = -1;

  config.graphics.enabled = true;
  config.graphics.settings.backend = yeti::graphics::engine::OPENGL;
  config.graphics.settings.fast_and_loose = manifest->graphics.fast_and_loose;

  return config;
}

int main(int argc, const char *argv[]) {
  ::setlocale(LC_ALL, "en_US.UTF-8");

  yeti::runtime::Manifest *manifest =
    yeti::runtime::manifest::load_from_path("config.ini");

  yeti::Config config = config_from_manifest(manifest);

  for (const char **arg = &argv[1], **end = &argv[argc]; arg < end; ++arg) {
    if (strcmp(*arg, "--debug") == 0) {
    #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
        YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
      debug();
    #else
      fprintf(stderr, "Can't run in debug mode unless a debug or development build!\n");
    #endif
    } else if (strcmp(*arg, "--workers") == 0) {
      config.threading.workers = strtol(*++arg, NULL, 10);
    } else {
      fprintf(stderr, "Unknown command-line argument '%s'.", *arg);
    }
  }

  // Let the games begin.
  yeti::initialize(config);
  yeti::runtime::StandardApplication app(manifest);
  app.run();

  return EXIT_FAILURE;
}
