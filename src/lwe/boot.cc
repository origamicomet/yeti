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

#include <lwe/foundation.h>
#include <lwe/asset.h>
#include <lwe/asset_compiler.h>
#include <lwe/window.h>
#include <lwe/swap_chain.h>
#include <lwe/render_device.h>
#include <lwe/render_command_stream.h>

typedef void (*lwe_boot_command_t)(
  lwe_size_t num_args,
  lwe_const_str_t* args );

static void _run(
  lwe_size_t num_args,
  lwe_const_str_t* args )
{
  lwe_asset_register_types();

  lwe_window_t* window = NULL;
  lwe_swap_chain_t* swap_chain = NULL;

  lwe_render_device_create(0);

  window = lwe_window_open(
    "Engine", 1280, 720
  );

  swap_chain = lwe_swap_chain_create(
    window, LWE_PIXEL_FORMAT_R8G8B8A8, 1280, 720, false, false
  );

  lwe_render_cmd_stream_t* cmd_stream =
    lwe_render_cmd_stream_create(256, 65535);

  lwe_render_cmd_stream_set_render_targets(
    cmd_stream,
    1, &swap_chain->render_target,
    NULL
  );

  const float clear_color[4] = { 1.0f, 0.0f, 0.6f, 1.0f };

  lwe_render_cmd_stream_clear(
    cmd_stream,
    true, &clear_color[0],
    false, 0.0f,
    false, 0x00000000u
  );

  lwe_render_cmd_stream_present(
    cmd_stream
  );

  lwe_window_show(window);

  while (true) {
    lwe_message_pump();
    lwe_render_device_dispatch(0, NULL, swap_chain, 1, (const lwe_render_cmd_stream_t**)&cmd_stream);
  }
}

static void _compile(
  lwe_size_t num_args,
  lwe_const_str_t* args )
{
  bool file = false;
  lwe_str_t data = "data";
  lwe_str_t data_src = "data_src";
  lwe_str_t path = NULL;

  for (lwe_size_t i = 0; i < num_args; ++i) {
    if (strncmp("--data=", args[i], 7) == 0) {
      data = &args[i][7];
    } else if(strncmp("--data-src=", args[i], 11) == 0) {
      data_src = &args[i][11];
    } else if(strncmp("--path", args[i], 6) == 0) {
      path = &args[i][6];
      file = true;
    }
  }

  lwe_asset_register_types();

  if (file)
    lwe_asset_compiler_compile(data, data_src, path);
  else
    lwe_asset_compiler_compile_dir(data, data_src);
}

int main( lwe_size_t argc, lwe_const_str_t argv[] )
{
  lwe_boot_command_t boot_cmd = &_run;

  if (argc >= 2) {
    if (strcmp("--compile", argv[1]) == 0)
      boot_cmd = &_compile;
  }

  boot_cmd(argc, argv);
  return EXIT_SUCCESS;
}