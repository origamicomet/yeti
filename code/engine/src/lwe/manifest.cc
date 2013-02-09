// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#include <lwe/manifest.h>

#include <libconfig.h>

lwe_manifest_t* lwe_manifest_load(
  lwe_const_str_t path )
{
  lwe_assert(path != NULL);

  config_t cfg;
  config_init(&cfg); {
    FILE* fh = fopen(path, "rb");

    lwe_fail_if(
      !config_read(&cfg, fh),
      "Unable to load manifest `%s`",
      path
    );

    fclose(fh);
  }

  config_setting_t* app =
    config_lookup(&cfg, "application");

  lwe_fail_if(
    app == NULL,
    "Malformed manifest, unable to find `application`"
  );

  config_setting_t* window =
    config_setting_get_member(app, "window");

  lwe_fail_if(
    window == NULL,
    "Malformed manifest, unable to find `application.window`"
  );

  config_setting_t* graphics =
    config_setting_get_member(app, "graphics");

  lwe_fail_if(
    graphics == NULL,
    "Malformed manifest, unable to find `application.graphics`"
  );

  config_setting_t* resolution =
    config_setting_get_member(graphics, "resolution");

  lwe_fail_if(
    resolution == NULL,
    "Malformed manifest, unable to find `application.graphics.resolution`"
  );

  config_setting_t* platform =
    config_setting_get_member(app, LWE_PLATFORM_STRING);

  lwe_fail_if(
    platform == NULL,
    "Malformed manifest, unable to find `application." LWE_PLATFORM_STRING "`"
  );

  config_setting_t* time_step_policy =
    config_setting_get_member(platform, "time_step_policy");

  lwe_fail_if(
    time_step_policy == NULL,
    "Malformed manifest, unable to find `application`"
  );

  lwe_manifest_t m;

  /* m.window.title */ {
    lwe_str_t title = NULL;

    if (!config_setting_lookup_string(window, "title", (const char**)&title))
      lwe_fail("Malformed manifest, non-string `application.window.title`");

    const lwe_size_t len = strlen(title);
    m.window.title = (lwe_const_str_t)lwe_alloc(len + 1);
    strncpy((char*)m.window.title, title, len);
  }

  /* m.window.width */ {
    int width = NULL;

    if (!config_setting_lookup_int(window, "width", (int*)&width))
      lwe_fail("Malformed manifest, non-integer `application.window.width`");

    m.window.width = width;
  }

  /* m.window.height */ {
    int height = NULL;

    if (!config_setting_lookup_int(window, "height", (int*)&height))
      lwe_fail("Malformed manifest, non-integer `application.window.height`");

    m.window.height = height;
  }

  /* m.graphics.adapter */ {
    int adapter = NULL;

    if (!config_setting_lookup_int(graphics, "adapter", (int*)&adapter))
      lwe_fail("Malformed manifest, non-integer `application.graphics.adapter`");

    m.graphics.adapter = adapter;
  }

  /* m.graphics.resolution.width */ {
    int width = NULL;

    if (!config_setting_lookup_int(resolution, "width", (int*)&width))
      lwe_fail("Malformed manifest, non-integer `application.graphics.resolution.width`");

    m.graphics.resolution.width = width;
  }

  /* m.graphics.resolution.height */ {
    int height = NULL;

    if (!config_setting_lookup_int(resolution, "height", (int*)&height))
      lwe_fail("Malformed manifest, non-integer `application.graphics.resolution.height`");

    m.graphics.resolution.height = height;
  }

  /* m.graphics.fullscreen */ {
    int fullscreen = FALSE;

    if (!config_setting_lookup_bool(graphics, "fullscreen", (int*)&fullscreen))
      lwe_fail("Malformed manifest, non-boolean `application.graphics.fullscreen`");

    m.graphics.fullscreen = (fullscreen == TRUE);
  }

  /* m.graphics.vertical_sync */ {
    int vertical_sync = FALSE;

    if (!config_setting_lookup_bool(graphics, "vertical_sync", (int*)&vertical_sync))
      lwe_fail("Malformed manifest, non-boolean `application.graphics.vertical_sync`");

    m.graphics.vertical_sync = (vertical_sync == TRUE);
  }

  /* m.save_directory */ {
    lwe_str_t save_dir = NULL;

    if (!config_setting_lookup_string(platform, "save_directory", (const char**)&save_dir))
      lwe_fail("Malformed manifest, non-string `application." LWE_PLATFORM_STRING ".save_directory`");

    const lwe_size_t len = strlen(save_dir);
    m.save_directory = (lwe_const_str_t)lwe_alloc(len + 1);
    strncpy((char*)m.save_directory, save_dir, len);
  }

  /* m.time_step_policy.type */ {
    lwe_str_t type = NULL;

    if (!config_setting_lookup_string(time_step_policy, "type", (const char**)&type))
      lwe_fail("Malformed manifest, non-string `application." LWE_PLATFORM_STRING ".time_step_policy.type`");

    m.time_step_policy.type = lwe_string_to_time_step_policy(type);
  }

  /* m.time_step_policy.data */ {
    // TODO
  }

  /* m.boot_package */ {
    lwe_str_t boot_pkg = NULL;

    if (!config_setting_lookup_string(app, "boot_package", (const char**)&boot_pkg))
      lwe_fail("Malformed manifest, non-string `application.boot_package`");

    const lwe_size_t len = strlen(boot_pkg);
    m.boot_package = (lwe_const_str_t)lwe_alloc(len + 1);
    strncpy((char*)m.boot_package, boot_pkg, len);
  }

  /* m.boot_script */ {
    lwe_str_t boot_script = NULL;

    if (!config_setting_lookup_string(app, "boot_script", (const char**)&boot_script))
      lwe_fail("Malformed manifest, non-string `application.boot_script`");

    const lwe_size_t len = strlen(boot_script);
    m.boot_script = (lwe_const_str_t)lwe_alloc(len + 1);
    strncpy((char*)m.boot_script, boot_script, len);
  }

  config_destroy(&cfg);

  lwe_manifest_t* manifest =
    (lwe_manifest_t*)lwe_alloc(sizeof(lwe_manifest_t));

  memcpy((void*)manifest, (const void*)&m, sizeof(lwe_manifest_t));

  return manifest;
}

void lwe_manifest_destroy(
  lwe_manifest_t* manifest )
{
  lwe_assert(manifest != NULL);

  lwe_free((void*)manifest->window.title);
  lwe_free((void*)manifest->save_directory);
  lwe_free((void*)manifest->boot_package);
  lwe_free((void*)manifest->boot_script);

  lwe_free((void*)manifest);
}