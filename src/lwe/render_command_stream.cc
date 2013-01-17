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

#include <lwe/render_command_stream.h>

lwe_render_cmd_stream_t* lwe_render_cmd_stream_create(
  lwe_size_t num_keys,
  lwe_size_t buffer_size )
{
  lwe_assert(num_keys > 0);
  lwe_assert(buffer_size > 0);

  lwe_render_cmd_stream_t* cmd_stream =
    (lwe_render_cmd_stream_t*)lwe_alloc(sizeof(lwe_render_cmd_stream_t));

  cmd_stream->num_keys = num_keys;
  cmd_stream->keys = (lwe_render_cmd_sort_key_t*)
    lwe_alloc(num_keys * sizeof(lwe_render_cmd_sort_key_t));

  cmd_stream->cmd_buffer_size = buffer_size;
  cmd_stream->cmd_buffer = (uint8_t*)lwe_alloc(buffer_size);
  cmd_stream->next_key = 0;
  cmd_stream->next_cmd_offset = 0;

  return cmd_stream;
}

lwe_render_cmd_sort_key_t* lwe_render_cmd_stream_set_render_targets(
  lwe_render_cmd_stream_t* cmd_stream,
  lwe_size_t num_render_targets,
  lwe_render_target_t** render_targets,
  lwe_depth_stencil_target_t* depth_stencil_target )
{
  lwe_assert(cmd_stream != NULL);

  const lwe_offset_t cmd_offset = cmd_stream->next_cmd_offset;

  lwe_set_render_targets_cmd_t* cmd =
    (lwe_set_render_targets_cmd_t*)(cmd_stream->cmd_buffer + cmd_offset);

  cmd->type = LWE_RENDER_COMMAND_TYPE_SET_RENDER_TARGETS;
  cmd->num_targets = num_render_targets;
  cmd->dst = depth_stencil_target;

  memcpy(
    (void*)&cmd->targets[0],
    (void*)&render_targets[0],
    num_render_targets * sizeof(lwe_render_target_t*)
  );

  cmd_stream->next_cmd_offset +=
    sizeof(lwe_set_render_targets_cmd_t);

  if (num_render_targets == 0) {
    cmd_stream->next_cmd_offset -= sizeof(lwe_render_target_t*);
  } else {
    cmd_stream->next_cmd_offset +=
      (num_render_targets - 1) * sizeof(lwe_render_target_t*);
  }

  lwe_render_cmd_sort_key_t* sort_key =
    &cmd_stream->keys[cmd_stream->next_key++];

  sort_key->offset = cmd_offset;
  return sort_key;
}

lwe_render_cmd_sort_key_t* lwe_render_cmd_stream_clear(
  lwe_render_cmd_stream_t* cmd_stream,
  bool clear_render_targets,
  const float* rgba,
  bool clear_depth_target,
  float depth,
  bool clear_stencil_target,
  uint32_t stencil )
{
  lwe_assert(cmd_stream != NULL);

  const lwe_offset_t cmd_offset = cmd_stream->next_cmd_offset;

  lwe_clear_cmd_t* cmd =
    (lwe_clear_cmd_t*)(cmd_stream->cmd_buffer + cmd_offset);

  cmd->type = LWE_RENDER_COMMAND_TYPE_CLEAR;
  cmd->clear_render_targets = clear_render_targets;
  cmd->clear_depth_target = clear_depth_target;
  cmd->clear_stencil_target = clear_stencil_target;

  if (rgba)
    memcpy((void*)&cmd->rgba[0], (void*)&rgba[0], 4 * sizeof(float));
  else
    cmd->rgba[0] = cmd->rgba[1] = cmd->rgba[2] = cmd->rgba[3] = 0.0f;

  cmd->depth = depth;
  cmd->stencil = stencil;

  cmd_stream->next_cmd_offset +=
    sizeof(lwe_clear_cmd_t);

  lwe_render_cmd_sort_key_t* sort_key =
    &cmd_stream->keys[cmd_stream->next_key++];

  sort_key->offset = cmd_offset;
  return sort_key;
}

lwe_render_cmd_sort_key_t* lwe_render_cmd_stream_present(
  lwe_render_cmd_stream_t* cmd_stream )
{
  lwe_assert(cmd_stream != NULL);

  const lwe_offset_t cmd_offset = cmd_stream->next_cmd_offset;

  lwe_present_cmd_t* cmd =
    (lwe_present_cmd_t*)(cmd_stream->cmd_buffer + cmd_offset);

  cmd->type = LWE_RENDER_COMMAND_TYPE_PRESENT;

  cmd_stream->next_cmd_offset +=
    sizeof(lwe_present_cmd_t);

  lwe_render_cmd_sort_key_t* sort_key =
    &cmd_stream->keys[cmd_stream->next_key++];

  sort_key->offset = cmd_offset;
  return sort_key;
}

void lwe_render_cmd_stream_destroy(
  lwe_render_cmd_stream_t* cmd_stream )
{
  lwe_assert(cmd_stream != NULL);

  lwe_free((void*)cmd_stream->keys);
  lwe_free((void*)cmd_stream->cmd_buffer);
  lwe_free((void*)cmd_stream);
}