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

int main(int argc, const char *argv[]) {
  ::setlocale(LC_ALL, "en_US.UTF-8");

  yeti::Config config;
  config.resources.database = NULL;
  config.threading.workers = -1;
  config.graphics.enabled = false;

  yeti::initialize(config);

  yeti::resource_compiler::Runner resource_compiler_runner;
  resource_compiler_runner.setup(&argv[1], argc - 1);
  resource_compiler_runner.run();

  yeti::shutdown();

  return EXIT_SUCCESS;
}
