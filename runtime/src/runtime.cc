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

// TODO(mtwilliams): Install assertion handler.

static void main_window_event_handler(void *ctx) {
  YETI_UNUSED(ctx);
}

int main(int argc, const char *argv[]) {
  ::setlocale(LC_ALL, "en_US.UTF-8");
  ::fprintf(stdout, YETI_STARTUP_BANNER);

  static const yeti::Window::Description main_window_desc = {
    /* .title = */ "Yeti",
    /* .dimensions = */ {
      /* .width = */ 1280,
      /* .height = */ 720
    }
  };

  yeti::Window *main_window = yeti::Window::open({"Yeti", {1280, 720}});
  main_window->show();
  while (true) {
    main_window->update(&main_window_event_handler, NULL);
  }

  return EXIT_SUCCESS;
}
