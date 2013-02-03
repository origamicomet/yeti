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
#include <lwe/asset_manager.h>
#include <lwe/window.h>
#include <lwe/swap_chain.h>
#include <lwe/render_stream.h>
#include <lwe/render_device.h>
#include <lwe/render_target.h>
#include <lwe/depth_stencil_target.h>
#include <lwe/assets/blend_state.h>
#include <lwe/assets/depth_stencil_state.h>
#include <lwe/assets/rasterizer_state.h>
#include <lwe/assets/model.h>

lwe_array_t<lwe_window_t*>* lwe_application_windows() {
  static lwe_array_t<lwe_window_t*> windows;
  return &windows;
}

lwe_array_t<lwe_swap_chain_t*>* lwe_application_swap_chains() {
  static lwe_array_t<lwe_swap_chain_t*> swap_chains;
  return &swap_chains;
}

static void lwe_application_update()
{
}

static lwe_blend_state_t* _blend_state = NULL;
static lwe_depth_stencil_state_t* _depth_stencil_state = NULL;
static lwe_rasterizer_state_t* _rasterizer_state = NULL;
static lwe_model_t* _suzanne = NULL;
static lwe_constant_buffer_t* _model_constants = NULL;

static void lwe_application_render(
  lwe_window_t* window,
  lwe_swap_chain_t* swap_chain,
  lwe_depth_stencil_target_t* depth_stencil )
{
  static lwe_render_stream_t* render_stream =
    lwe_render_stream_create(65535);

  static const float clear_color[4] =
    { 1.0f, 0.0f, 0.6f, 1.0f };

  static float rotation = 0.0f;
  rotation += (1.0f / (180.0f / LWE_PI)) * 0.01f;

  static const mat4_t model_mat =
    _suzanne->meshes[0].transform *
    mat3_t::rotate_x(LWE_PI / -2.0f);

  const mat4_t view_mat =
    mat4_t::look_at(
      vec3_t(cos(rotation) * 2, sin(rotation) * 2, 2.0f),
      vec3_t(0.0f, 0.0f, 0.0f),
      vec3_t(0.0f, 1.0f, 0.0f)
    );

  static const mat4_t proj_mat =
    mat4_t::perspective(45.0f / (180.0f / LWE_PI), 1280.0f / 720.0f, 1.0f, 100.0f);

  const mat4_t model_view_proj_mat =
    model_mat * view_mat * proj_mat;

  {
    void* constants = lwe_constant_buffer_map(
      _model_constants, LWE_MAP_FLAGS_WRITE | LWE_MAP_FLAGS_DISCARD
    );

    memcpy(constants, (void*)&model_view_proj_mat, sizeof(mat4_t));
    
    lwe_constant_buffer_unmap(_model_constants);
  }

  lwe_render_stream_reset(render_stream);

  lwe_render_stream_set_render_targets(
    render_stream,
    1, &swap_chain->render_target,
    depth_stencil
  );

  static const lwe_viewport_t viewport = {
    0, 0, 1280, 720
  };

  lwe_render_stream_set_viewports(
    render_stream,
    1, &viewport
  );

  lwe_render_stream_clear(
    render_stream,
    LWE_CLEAR_COLOR | LWE_CLEAR_DEPTH,
    &clear_color[0],
    1.0f,
    0x00000000u
  );

  lwe_render_stream_set_blend_state(
    render_stream, _blend_state
  );

  lwe_render_stream_set_depth_stencil_state(
    render_stream, _depth_stencil_state, 0
  );

  lwe_render_stream_set_rasterizer_state(
    render_stream, _rasterizer_state
  );

  lwe_render_stream_draw(
    render_stream,
    &_suzanne->meshes[0],
    1, &_model_constants
  );

  lwe_render_stream_present(
    render_stream,
    swap_chain
  );

  lwe_render_device_dispatch(
    1, (const lwe_render_stream_t**)&render_stream
  );
}

void lwe_application_run(
  lwe_size_t num_args,
  lwe_const_str_t* args )
{
  lwe_asset_register_types();
  lwe_render_device_create(0);

  lwe_window_t* window =
    lwe_window_open("Engine", 1280, 720);
  lwe_array_push(lwe_application_windows(), &window);

  lwe_swap_chain_t* swap_chain =
    lwe_swap_chain_create(window, LWE_PIXEL_FORMAT_R8G8B8A8, 1280, 720, false, false);
  lwe_array_push(lwe_application_swap_chains(), &swap_chain);

  lwe_depth_stencil_target_t* depth_stencil =
    lwe_depth_stencil_target_create(LWE_PIXEL_FORMAT_D24_S8, 1280, 720);

  lwe_window_show(window);

  _blend_state =
    (lwe_blend_state_t*)lwe_asset_manager_load(
      "states/none.blend_state"
    );

  _depth_stencil_state =
    (lwe_depth_stencil_state_t*)lwe_asset_manager_load(
      "states/geometry.depth_stencil_state"
    );

  _rasterizer_state =
    (lwe_rasterizer_state_t*)lwe_asset_manager_load(
      "states/geometry.rasterizer_state"
    );

  _suzanne =
    (lwe_model_t*)lwe_asset_manager_load(
      "models/suzanne/suzanne.model"
    );

  {
    static const mat4_t identity = mat4_t::identity();

    _model_constants = lwe_constant_buffer_create(
      0, sizeof(mat4_t), (void*)&identity
    );
  }

  while (true) {
    lwe_message_pump();
    lwe_application_update();
    lwe_application_render(window, swap_chain, depth_stencil);
  }
}