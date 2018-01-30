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

#include "yeti/runtime/manifest.h"
#include "yeti/runtime/standard_application.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

static yeti::Config config_from_manifest(yeti::runtime::Manifest *manifest) {
  yeti::Config config;

  config.app.id = manifest->app.id;
  config.app.publisher = manifest->app.publisher;

  config.user.settings = manifest->user.settings;
  config.user.saves = manifest->user.saves;

  config.resources.database = manifest->resources.database;
  config.resources.autoload = manifest->resources.autoload;

  config.keyboard.raw = manifest->keyboard.raw;
  config.mouse.raw = manifest->mouse.raw;

  config.debug.floating_point_exceptions = manifest->debug.floating_point_exceptions;
  config.debug.memory = false;

  // Spawn a worker thread for each logical core, minus one for the main thread.
  config.workers = -1;

#if 0
  config.graphics.enabled = true;
  config.graphics.settings.backend = yeti::graphics::engine::OPENGL;
  config.graphics.settings.fast_and_loose = manifest->graphics.fast_and_loose;
#endif

  return config;
}

// TODO(mtwilliams): Preemptively start logging infrastructure.

int main(int argc, const char *argv[]) {
  ::setlocale(LC_ALL, "en_US.UTF-8");

  char current_working_directory[256];
  yeti::core::path::cwd(current_working_directory, sizeof(current_working_directory));

  yeti::runtime::Manifest *manifest =
    yeti::runtime::manifest::load_from_path("config.ini");

  if (!manifest) {
    fprintf(stderr, "Failed to load manifest from `%s%c%s`!\n",
                     current_working_directory,
                     yeti::core::path::seperator(),
                     "config.ini");

    return EXIT_FAILURE;
  }

  yeti::Config config = config_from_manifest(manifest);

  for (const char **arg = &argv[1], **end = &argv[argc]; arg < end; ++arg) {
    if (strcmp(*arg, "--debug") == 0) {
    #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
        YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
      config.debug.floating_point_exceptions = true;
      config.debug.memory = true;
    #else
      fprintf(stderr, "Can't run in debug mode unless a debug or development build!\n");
    #endif
    } else if (strcmp(*arg, "--workers") == 0) {
      config.workers = strtol(*++arg, NULL, 10);
    } else {
      fprintf(stderr, "Unknown command-line argument '%s'.", *arg);
    }
  }

  yeti::boot(config);

  // Let the games begin.
  yeti::runtime::StandardApplication app(manifest);
  app.run();

  return EXIT_FAILURE;
}
