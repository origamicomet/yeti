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

  // Let the games begin.
  yeti::initialize(config_from_manifest(manifest));
  yeti::runtime::StandardApplication app(manifest);
  app.run();

  return EXIT_FAILURE;
}
