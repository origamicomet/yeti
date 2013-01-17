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

#ifndef _LWE_RENDER_COMMAND_STREAM_H_
#define _LWE_RENDER_COMMAND_STREAM_H_

#include <lwe/foundation.h>
#include <lwe/render_command.h>

typedef struct lwe_render_cmd_stream_t {
  lwe_size_t num_keys;
  lwe_render_cmd_sort_key_t* keys;
  lwe_size_t cmd_buffer_size;
  uint8_t* cmd_buffer;
  lwe_offset_t next_key;
  lwe_offset_t next_cmd_offset;
} lwe_render_cmd_stream_t;

extern lwe_render_cmd_stream_t* lwe_render_cmd_stream_create(
  lwe_size_t num_keys,
  lwe_size_t buffer_size );

extern lwe_render_cmd_sort_key_t* lwe_render_cmd_stream_set_render_targets(
  lwe_render_cmd_stream_t* cmd_stream,
  lwe_size_t num_render_targets,
  lwe_render_target_t** render_targets,
  lwe_depth_stencil_target_t* depth_stencil_target );

extern lwe_render_cmd_sort_key_t* lwe_render_cmd_stream_clear(
  lwe_render_cmd_stream_t* cmd_stream,
  bool clear_render_targets,
  const float* rgba,
  bool clear_depth_target,
  float depth,
  bool clear_stencil_target,
  uint32_t stencil );

extern lwe_render_cmd_sort_key_t* lwe_render_cmd_stream_present(
  lwe_render_cmd_stream_t* cmd_stream );

extern void lwe_render_cmd_stream_destroy(
  lwe_render_cmd_stream_t* cmd_stream );

#endif // _LWE_RENDER_COMMAND_STREAM_H_