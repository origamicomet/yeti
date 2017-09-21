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

#include "yeti/resource_compiler/runner.h"

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

int main(int argc, const char *argv[]) {
  ::setlocale(LC_ALL, "en_US.UTF-8");

  yeti::Config config;

  config.app.id = NULL;
  config.app.publisher = NULL;

  config.user.settings = NULL;
  config.user.saves = NULL;

  config.resources.database = NULL;
  config.resources.autoload = false;

  config.keyboard.raw = true;
  config.mouse.raw = true;

  config.debug.floating_point_exceptions = true;
  config.debug.memory = false;

  // Spawn a worker thread for each logical core, minus one for the main thread.
  config.workers = -1;

  yeti::boot(config);

  yeti::resource_compiler::Runner resource_compiler_runner;
  resource_compiler_runner.setup(&argv[1], argc - 1);
  resource_compiler_runner.run();

  yeti::shutdown();

  return EXIT_SUCCESS;
}
