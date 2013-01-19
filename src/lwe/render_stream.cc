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

#include <lwe/render_stream.h>

lwe_render_stream_t* lwe_render_stream_create(
  lwe_size_t size )
{
  lwe_assert(size > 0);

  lwe_render_stream_t* stream =
    (lwe_render_stream_t*)lwe_alloc(sizeof(lwe_render_stream_t) + (size - 1));

  stream->size = 0;
  stream->next_command = 0;

  return stream;
}

void lwe_render_stream_set_render_targets(
  lwe_render_stream_t* stream,
  lwe_size_t num_render_targets,
  lwe_render_target_t** render_targets,
  lwe_depth_stencil_target_t* depth_stencil_target )
{
  lwe_assert(stream != NULL);

  lwe_set_render_targets_cmd_t* cmd =
    (lwe_set_render_targets_cmd_t*)&stream->buffer[stream->next_command];

  stream->next_command +=
    sizeof(lwe_set_render_targets_cmd_t) +
    (num_render_targets - 1) * sizeof(lwe_render_target_t*);

  cmd->type = cmd->type = LWE_RENDER_COMMAND_TYPE_SET_RENDER_TARGETS;
  cmd->num_targets = num_render_targets;
  cmd->dst = depth_stencil_target;
  cmd->num_targets = num_render_targets;

  memcpy(
    (void*)cmd->targets,
    (void*)render_targets,
    num_render_targets * sizeof(lwe_render_target_t*)
  );
}

void lwe_render_stream_clear(
  lwe_render_stream_t* stream,
  uint32_t flags,
  const float* rgba,
  float depth,
  uint32_t stencil )
{
  lwe_assert(stream != NULL);

  lwe_clear_cmd_t* cmd =
    (lwe_clear_cmd_t*)&stream->buffer[stream->next_command];

  stream->next_command +=
    sizeof(lwe_clear_cmd_t);

  cmd->type = LWE_RENDER_COMMAND_TYPE_CLEAR;
  cmd->flags = flags;
  memcpy((void*)&cmd->rgba[0], (void*)&rgba[0], 4 * sizeof(float));
  cmd->depth = depth;
  cmd->stencil = stencil;
}

void lwe_render_stream_present(
  lwe_render_stream_t* stream,
  lwe_swap_chain_t* swap_chain )
{
  lwe_assert(stream != NULL);

  lwe_present_cmd_t* cmd =
    (lwe_present_cmd_t*)&stream->buffer[stream->next_command];

  stream->next_command +=
    sizeof(lwe_present_cmd_t);

  cmd->type = LWE_RENDER_COMMAND_TYPE_PRESENT;
  cmd->swap_chain = swap_chain;
}

void lwe_render_stream_destroy(
  lwe_render_stream_t* stream )
{
  lwe_assert(stream != NULL);
  lwe_free((void*)stream);
}