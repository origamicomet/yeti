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

#ifndef _LWE_RENDER_STREAM_H_
#define _LWE_RENDER_STREAM_H_

#include <lwe/foundation.h>
#include <lwe/render_command.h>

typedef struct lwe_render_stream_t {
  lwe_size_t size;
  lwe_offset_t next_command;
  uint8_t buffer[1];
} lwe_render_stream_t;

extern lwe_render_stream_t* lwe_render_stream_create(
  lwe_size_t size );

extern void lwe_render_stream_set_render_targets(
  lwe_render_stream_t* stream,
  lwe_size_t num_render_targets,
  lwe_render_target_t** render_targets,
  lwe_depth_stencil_target_t* depth_stencil_target );

extern void lwe_render_stream_set_blend_state(
  lwe_render_stream_t* stream,
  lwe_blend_state_t* blend_state );

extern void lwe_render_stream_set_depth_stencil_state(
  lwe_render_stream_t* stream,
  lwe_depth_stencil_state_t* depth_stencil_state,
  uint32_t stencil_ref );

extern void lwe_render_stream_set_rasterizer_state(
  lwe_render_stream_t* stream,
  lwe_rasterizer_state_t* rasterizer_state );

extern void lwe_render_stream_clear(
  lwe_render_stream_t* stream,
  uint32_t flags,
  const float* rgba,
  float depth,
  uint32_t stencil );

extern void lwe_render_stream_present(
  lwe_render_stream_t* stream,
  lwe_swap_chain_t* swap_chain );

extern void lwe_render_stream_destroy(
  lwe_render_stream_t* stream );

#endif // _LWE_RENDER_STREAM_H_