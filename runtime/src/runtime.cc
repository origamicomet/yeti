//===-- runtime.cc ----------------------------------------*- mode: C++ -*-===//
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

#include <stdlib.h>
#include <stdio.h>

#define YETI_STARTUP_BANNER \
  "                                                                              \n" \
  "                               __ __     _   _                                \n" \
  "                              |  |  |___| |_|_|                               \n" \
  "                              |_   _| -_|  _| |                               \n" \
  "                                |_| |___|_| |_|                               \n" \
  "                                                                              \n" \
  "      Copyright 2013-2016 Origami Comet Games, Inc. All rights reserved.      \n" \
  "                                                                              \n"

int main(int argc, const char *argv[]) {
  ::fprintf(stdout, YETI_STARTUP_BANNER);
  return EXIT_SUCCESS;
}
