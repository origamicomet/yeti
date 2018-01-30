//===-- yeti/runtime/manifest.h -------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_RUNTIME_MANIFEST_H_
#define _YETI_RUNTIME_MANIFEST_H_

#include "yeti.h"

namespace yeti {
namespace runtime {

struct Manifest {
  struct {
    // Application identifier.
    const char *id;

    // Publisher identifier.
    const char *publisher;

    // Application version.
    unsigned build;

    // Application name.
    const char *name;

    // Application icon.
    const char *icon;
  } app;

  struct {
    // Path to resource database.
    const char *database;

    // Toggles automatic loading of resources as required rather than loading
    // through packages. This only affects debug and development builds.
    bool autoload;
  } resources;

  struct {
    // Initial package to load prior to booting.
    const char *package;

    // Boot script.
    const char *script;
  } boot;

  struct {
    // Render configuration to use.
    const char *config;

    // Resolved path to render settings.
    const char *settings;

    // Default rendering resolution.
    unsigned width;
    unsigned height;

    // Default to starting windowed, fullscreen, or borderless.
    bool fullscreen;
    bool borderless;

    // Default to vertical synchronization.
    bool vsync;

    // Allow abuse of undefined behaviour to increase performance.
    bool fast_and_loose;
  } graphics;

  struct {
    // Use raw input, if available, for keyboard input.
    bool raw;
  } keyboard;

  struct {
    // Use raw input, if available, for mouse input.
    bool raw;
  } mouse;

  struct {
    // Resolved path to user settings.
    const char *settings;

    // Root directory for saves.
    const char *saves;
  } user;

  struct {
    // Toggles networked console.
    bool enabled;

    // Port to listen on.
    unsigned port;

    // Path to save logs to.
    const char *log;
  } console;

  struct {
    // Toggles raising of floating-point exceptions when a math operation fails.
    bool floating_point_exceptions;
  } debug;
};

namespace manifest {
  Manifest *load_from_path(const char *path, bool lax = false);
  Manifest *load_from_file(core::File *file, bool lax = false);
}

} // runtime
} // yeti

#endif // _YETI_RUNTIME_MANIFEST_H_
