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

  yeti::Window *main_window = yeti::Window::open({"Yeti", {1280, 720}});
  yeti::input::from(main_window);
  main_window->show();

  while (true) {
    main_window->update(&main_window_event_handler, NULL);

  #if 0
    if (yeti::Mouse::pressed(yeti::MouseButtons::LEFT)) {
      fprintf(stdout, "mouse.buttons[left] was pressed!\n");
      main_window->clip();
    } else if (yeti::Mouse::released(yeti::MouseButtons::LEFT)) {
      fprintf(stdout, "mouse.buttons[left] was released!\n");
      main_window->unclip();
    }
  #endif

  #if 0
    const yeti::Vec3 mouse_absolute = yeti::Mouse::axis(yeti::MouseAxes::ABSOLUTE);
    const yeti::Vec3 mouse_relative = yeti::Mouse::axis(yeti::MouseAxes::RELATIVE);
    const yeti::Vec3 mouse_delta = yeti::Mouse::axis(yeti::MouseAxes::DELTA);
    fprintf(stdout,
      "mouse.absolute.x=%.2f, mouse.absolute.y=%.2f, "
      "mouse.relative.x=%.2f, mouse.relative.y=%.2f, "
      "mouse.delta.x=%.2f, mouse.delta.y=%.2f\n",
      mouse_absolute.x, mouse_absolute.y,
      mouse_relative.x, mouse_relative.y,
      mouse_delta.x, mouse_delta.y);
  #endif

    yeti::Keyboard::update();
    yeti::Mouse::update();
  }

  return EXIT_SUCCESS;
}
