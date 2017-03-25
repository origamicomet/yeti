//===-- yeti/runtime.cc -----------------------------------*- mode: C++ -*-===//
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

#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char *argv[]) {
  ::setlocale(LC_ALL, "en_US.UTF-8");

  // TODO(mtwilliams): Gather most configuration from `config.ini`.
  yeti::Config config;
  config.resources.database = "resources.db";
  // TODO(mtwilliams): Determine number of cores.
  config.threading.workers = 8 - 1;

  // Let the games begin.
  yeti::initialize(config);
  yeti::runtime::StandardApplication app;
  app.run();

  return EXIT_FAILURE;
}
