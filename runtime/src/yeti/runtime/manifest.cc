//===-- yeti/runtime/manifest.cc ------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/runtime/manifest.h"

namespace yeti {
namespace runtime {

Manifest *manifest::load_from_path(const char *path) {
  if (foundation::fs::File *file = foundation::fs::open(path, foundation::fs::READ)) {
    Manifest *manifest = load_from_file(file);
    foundation::fs::close(file);
    return manifest;
  }

  return NULL;
}

// TODO(mtwilliams): Actually load from |file|.

Manifest *manifest::load_from_file(foundation::fs::File *file) {
  Manifest *manifest = new (foundation::heap()) Manifest();

  manifest->app.id = "vanguard";
  manifest->app.publisher = "origamicomet";

  manifest->app.build = 1;

  manifest->app.name = "Vanguard™";
  manifest->app.icon = "default.ico";

  manifest->resources.database = "resources.db";

  manifest->resources.autoload = true;

  manifest->boot.package = "vanguard/boot";
  manifest->boot.script = "vanguard/boot";

  manifest->graphics.config = "core/base";

  // TODO(mtwilliams): Resolve paths.
  manifest->graphics.settings = NULL;

  manifest->graphics.width = 1280;
  manifest->graphics.height = 720;

  manifest->graphics.fullscreen = false;
  manifest->graphics.borderless = false;

  manifest->graphics.vsync = true;

  manifest->graphics.fast_and_loose = false;

  manifest->keyboard.raw = true;
  manifest->mouse.raw = true;

  // TODO(mtwilliams): Resolve paths.
  manifest->user.settings = NULL;
  manifest->user.saves = NULL;

  manifest->console.enabled = true;
  manifest->console.port = 6543;

  // TODO(mtwilliams): Resolve paths.
  manifest->console.log = "log.txt";

  manifest->debug.floating_point_exceptions = true;

  return manifest;
}

} // runtime
} // yeti
