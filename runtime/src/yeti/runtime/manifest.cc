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

// TODO(mtwilliams): Flesh out error handling.
// TODO(mtwilliams): Resolve paths.
// TODO(mtwilliams): Reasonable defaults.
// TODO(mtwilliams): Extract remaining settings.
// TODO(mtwilliams): Resolve paths.

namespace yeti {
namespace runtime {

Manifest *manifest::load_from_path(const char *path, bool lax) {
  if (core::File *file = core::fs::open(path, core::File::READ)) {
    Manifest *manifest = load_from_file(file, lax);
    core::fs::close(file);
    return manifest;
  }

  return NULL;
}

namespace manifest {
  namespace {
    // NOTE(mtwilliams): Leaks memory. Doesn't matter however, as only a single
    // manifest is loaded for the lifetime of the application.
    static const char *duplicate(const char *string) {
      const size_t length = strlen(string);
      char *copy = (char *)core::global_heap_allocator().allocate(length + 1, 4);
      core::memory::copy((const void *)string, (void *)copy, length + 1);
      return copy;
    }

    static bool boolean(const char *in, bool *out) {
      if (strcmp(in, "true") == 0)
        *out = true;
      else if (strcmp(in, "false") == 0)
        *out = false;
      else
        return false;
      return true;
    }
  }


  static bool is_valid_identifier(const char *candidate) {
    while (const char ch = *candidate++)
      if (!((ch == '_') || (ch >= 'A' && ch <= 'z')))
        return false;
    return true;
  }

  #define ERROR(Format, ...) \
    return fprintf(stderr, Format, ##__VA_ARGS__), false;

  static bool handle_application_identifier(Manifest *manifest,
                                            const char *id) {
    if (!is_valid_identifier(id))
      ERROR("Invalid application identifier!");
    manifest->app.id = duplicate(id);
    return true;
  }

  static bool handle_application_publisher(Manifest *manifest,
                                           const char *publisher) {
    if (!is_valid_identifier(publisher))
      ERROR("Invalid publisher!");
    manifest->app.publisher = duplicate(publisher);
    return true;
  }

  static bool handle_application_build(Manifest *manifest,
                                       const char *build) {
    if (sscanf(build, "%u", &manifest->app.build) != 1)
      ERROR("Invalid build number!");
    return true;
  }

  #undef ERROR

  static bool callback(const char *section,
                       const char *key,
                       const char *value,
                       Manifest *manifest)
  {
    if (strcmp("app", section) == 0) {
      if (strcmp("id", key) == 0)
        return handle_application_identifier(manifest, value);
      else if (strcmp("publisher", key) == 0)
        return handle_application_publisher(manifest, value);
      else if (strcmp("build", key) == 0)
        return handle_application_build(manifest, value);
      else if (strcmp("name", key) == 0)
        manifest->app.name = duplicate(value);
      else if (strcmp("icon", key) == 0)
        manifest->app.icon = duplicate(value);
    } else if (strcmp("resources", section) == 0) {
      if (strcmp("database", key) == 0)
        manifest->resources.database = duplicate(value);
      else if (strcmp("autoload", key) == 0)
        return boolean(value, &manifest->resources.autoload);
    } else if (strcmp("boot", section) == 0) {
      if (strcmp("package", key) == 0)
        manifest->boot.package = duplicate(value);
      else if (strcmp("script", key) == 0)
        manifest->boot.script = duplicate(value);
    } else if (strcmp("graphics", section) == 0) {
      if (strcmp("config", key) == 0)
        manifest->graphics.config = duplicate(value);
      else if (strcmp("settings", key) == 0)
        manifest->graphics.settings = duplicate(value);
      else if (strcmp("width", key) == 0)
        return (sscanf(value, "%u", &manifest->graphics.width) == 1);
      else if (strcmp("height", key) == 0)
        return (sscanf(value, "%u", &manifest->graphics.height) == 1);
      else if (strcmp("fullscreen", key) == 0)
        return boolean(value, &manifest->graphics.fullscreen);
      else if (strcmp("borderless", key) == 0)
        return boolean(value, &manifest->graphics.borderless);
      else if (strcmp("vsync", key) == 0)
        return boolean(value, &manifest->graphics.vsync);
    }

    return true;
  }
}

Manifest *manifest::load_from_file(core::File *file, bool lax) {
  Manifest *manifest = YETI_NEW(Manifest, core::global_heap_allocator());

  manifest->app.id = "untitled";
  manifest->app.publisher = "unknown";
  manifest->app.build = 0;
  manifest->app.name = "Untitled";
  manifest->app.icon = "default.ico";

  manifest->resources.database = "resource.db";
  manifest->resources.autoload = true;

  manifest->boot.package = "core/boot";
  manifest->boot.script = "core/boot";

  manifest->graphics.config = NULL;
  manifest->graphics.settings = NULL;
  manifest->graphics.width = 1280;
  manifest->graphics.height = 720;
  manifest->graphics.fullscreen = false;
  manifest->graphics.borderless = false;
  manifest->graphics.vsync = true;
  manifest->graphics.fast_and_loose = false;

  manifest->keyboard.raw = true;
  manifest->mouse.raw = true;

  manifest->user.settings = NULL;
  manifest->user.saves = NULL;

  manifest->console.enabled = true;
  manifest->console.port = 6543;
  manifest->console.log = "log.txt";

  manifest->debug.floating_point_exceptions = true;

  const u32 options = lax ? core::ini::CONTINUE_ON_ERROR : 0;

  if (core::ini::parse(file, (core::ini::Callback)&callback, (void *)manifest, options))
    return manifest;

  YETI_DELETE(Manifest, core::global_heap_allocator(), manifest);

  return NULL;
}

} // runtime
} // yeti
