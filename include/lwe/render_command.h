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

#ifndef _LWE_RENDER_COMMAND_H_
#define _LWE_RENDER_COMMAND_H_

#include <lwe/foundation.h>
#include <lwe/vertex_declaration.h>
#include <lwe/viewport.h>

struct lwe_swap_chain;
struct lwe_render_target_t;
struct lwe_depth_stencil_target_t;
struct lwe_index_buffer_t;
struct lwe_vertex_buffer_t;
struct lwe_constant_buffer_t;
struct lwe_material_t;

typedef enum lwe_render_cmd_type_t {
  LWE_RENDER_COMMAND_TYPE_INVALID                  = 0x00000000u,
  LWE_RENDER_COMMAND_TYPE_FORCE_UINT32             = 0xFFFFFFFFu,

  LWE_RENDER_COMMAND_TYPE_SET_RENDER_TARGETS       = 0x00000001u,
  LWE_RENDER_COMMAND_TYPE_SET_VIEWPORTS            = 0x00000002u,
  LWE_RENDER_COMMAND_TYPE_CLEAR                    = 0x00000003u,
  LWE_RENDER_COMMAND_TYPE_GENERATE_MIPS            = 0x00000004u,
  LWE_RENDER_COMMAND_TYPE_DRAW                     = 0x00000005u,
  LWE_RENDER_COMMAND_TYPE_PRESENT                  = 0x00000006u
} lwe_render_cmd_type_t;

typedef struct lwe_render_cmd_sort_key_t {
  union {
    struct {
      unsigned render_layer : 8;
      unsigned pass         : 8;
      unsigned depth        : 16;
      unsigned user         : 32;
    };

    uint32_t composite;
  };

  lwe_offset_t offset;
} lwe_render_cmd_sort_key_t;

typedef struct lwe_render_cmd_t {
  lwe_render_cmd_type_t type;
} lwe_render_cmd_t;

typedef struct lwe_set_render_targets_cmd_t
  : public lwe_render_cmd_t
{
  lwe_size_t num_targets;
  lwe_depth_stencil_target_t* dst;
  lwe_render_target_t* targets[1];
} lwe_set_render_target_cmd_t;

typedef struct lwe_set_viewports_cmd_t
  : public lwe_render_cmd_t
{
  lwe_size_t num_viewports;
  lwe_viewport_t viewports[1];
} lwe_set_viewports_cmd_t;

typedef struct lwe_clear_cmd_t
  : public lwe_render_cmd_t
{
  bool clear_render_targets : 1;
  bool clear_depth_target : 1;
  bool clear_stencil_target : 1;
  unsigned reserved : 5;
  float rgba[4];
  float depth;
  uint32_t stencil;
} lwe_clear_cmd_t;

typedef struct lwe_generate_mips_cmd_t
  : public lwe_render_cmd_t
{
  lwe_render_target_t* target;
} lwe_generate_mips_cmd_t;

typedef struct lwe_draw_cmd_t
  : public lwe_render_cmd_t
{
  lwe_material_t* material;
  lwe_vertex_declaration_t vertex_decl;
  lwe_size_t num_indicies;
  lwe_size_t num_vertices;
  lwe_index_buffer_t* indicies;
  lwe_vertex_buffer_t* vertices;
} lwe_draw_cmd_t;

typedef struct lwe_present_cmd_t
  : public lwe_render_cmd_t
{
} lwe_present_cmd_t;

#endif // _LWE_RENDER_COMMAND_H_