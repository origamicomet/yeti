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

#ifndef _LWE_SWAP_CHAIN_H_
#define _LWE_SWAP_CHAIN_H_

#include <lwe/foundation.h>
#include <lwe/window.h>
#include <lwe/pixel_format.h>

struct lwe_render_target_t;

typedef struct lwe_swap_chain_t {
  lwe_window_t* window;
  bool fullscreen;
  bool vsync;
  struct lwe_render_target_t* render_target;
} lwe_swap_chain_t;

extern lwe_swap_chain_t* lwe_swap_chain_create(
  lwe_window_t* window,
  lwe_pixel_format_t pixel_format,
  uint32_t width,
  uint32_t height,
  bool fullscreen,
  bool vsync );

extern void lwe_swap_chain_resize(
  lwe_swap_chain_t* swap_chain,
  uint32_t new_width,
  uint32_t new_height );

extern void lwe_swap_chain_toggle_fullscreen(
  lwe_swap_chain_t* swap_chain,
  bool fullscreen );

extern void lwe_swap_chain_toggle_vsync(
  lwe_swap_chain_t* swap_chain,
  bool vsync );

extern void lwe_swap_chain_destroy(
  lwe_swap_chain_t* swap_chain );

#endif // _LWE_SWAP_CHAIN_H_