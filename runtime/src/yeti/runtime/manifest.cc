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
  if (core::File *file = core::fs::open(path, core::File::READ)) {
    Manifest *manifest = load_from_file(file);
    core::fs::close(file);
    return manifest;
  }

  return NULL;
}

// TODO(mtwilliams): Actually load from |file|.

Manifest *manifest::load_from_file(core::File *file) {
  Manifest *manifest = YETI_NEW(Manifest, core::global_heap_allocator());

  manifest->app.id = "vanguard";
  manifest->app.publisher = "origamicomet";

  manifest->app.build = 1;

  manifest->app.name = "Vanguard™";
  manifest->app.icon = "default.ico";

  manifest->resources.database = "resource.db";

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
