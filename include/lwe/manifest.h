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

#ifndef _LWE_MANIFEST_H_
#define _LWE_MANIFEST_H_

#include <lwe/foundation.h>
#include <lwe/time_step_policy.h>

typedef struct lwe_manifest_t {
  struct {
    lwe_str_t title;
    uint32_t width;
    uint32_t height;
  } window;

  struct {
    uint32_t adapter;

    struct {
      uint32_t width;
      uint32_t height;
    } resolution;

    bool fullscreen;
    bool vertical_sync;
  } graphics;

  lwe_str_t save_directory;

  struct {
    lwe_time_step_policy_t type;
    lwe_time_step_policy_params_t params;
  } time_step_policy;

  lwe_str_t boot_package;
  lwe_str_t boot_script;
} lwe_manifest_t;

extern lwe_manifest_t* lwe_manifest_load(
  lwe_str_t path );

extern void lwe_manifest_destroy(
  lwe_manifest_t* manifest );

#endif // _LWE_MANIFEST_H_