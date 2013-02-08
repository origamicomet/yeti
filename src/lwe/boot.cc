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
#include <lwe/application.h>

typedef void (*lwe_boot_command_t)(
  lwe_size_t num_args,
  lwe_const_str_t* args );

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
    } else if(strncmp("--path=", args[i], 6) == 0) {
      path = &args[i][7];
      file = true;
    }
  }

  lwe_asset_register_types();

  if (file)
    lwe_asset_compiler_compile(data, data_src, path);
  else
    lwe_asset_compiler_compile_dir(data, data_src);
}

static void _compile_server(
  lwe_size_t num_args,
  lwe_const_str_t* args )
{
  lwe_str_t data = "data";
  lwe_str_t data_src = "data_src";

  for (lwe_size_t i = 0; i < num_args; ++i) {
    if (strncmp("--data=", args[i], 7) == 0) {
      data = &args[i][7];
    } else if(strncmp("--data-src=", args[i], 11) == 0) {
      data_src = &args[i][11];
    }
  }

  lwe_asset_register_types();
  lwe_asset_compiler_watch_dir(data, data_src);
}

int main( lwe_size_t argc, lwe_const_str_t argv[] )
{
  lwe_boot_command_t boot_cmd = &lwe_application_run;

  if (argc >= 2) {
    if (strcmp("--compile", argv[1]) == 0)
      boot_cmd = &_compile;

    if (strcmp("--compile-server", argv[1]) == 0)
      boot_cmd = &_compile_server;
  }

  boot_cmd(argc, argv);
  return EXIT_SUCCESS;
}