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
#include <lwe/manifest.h>
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

static lwe_manifest_t* _manifest = NULL;
static lwe_window_t* _window = NULL;
static lwe_swap_chain_t* _swap_chain = NULL;
static lwe_depth_stencil_target_t* _depth_stencil_target = NULL;
static lwe_blend_state_t* _blend_state = NULL;
static lwe_depth_stencil_state_t* _depth_stencil_state = NULL;
static lwe_rasterizer_state_t* _rasterizer_state = NULL;
static lwe_model_t* _suzanne = NULL;
static lwe_constant_buffer_t* _model_constants = NULL;

static float _rotation = 0.0f;
static float _step = 0.0f;

static void lwe_application_update()
{
  while (!lwe_queue_empty(&_window->input_events)) {
    lwe_input_event_t* event = lwe_queue_queued(&_window->input_events);
    lwe_queue_dequeue(&_window->input_events);

    switch (event->type) {
      case LWE_INPUT_EVENT_MOUSE_BUTTON_PRESSED: {
        if (event->mouse.button == LWE_MOUSE_BUTTON_LEFT) {
          _step += (1.0f / (180.0f / LWE_PI)) * 0.01f;
        } else if (event->mouse.button == LWE_MOUSE_BUTTON_RIGHT) {
          _step -= (1.0f / (180.0f / LWE_PI)) * 0.01f;
        }
      } break;

      case LWE_INPUT_EVENT_MOUSE_BUTTON_RELEASED: {
        if (event->mouse.button == LWE_MOUSE_BUTTON_LEFT) {
          _step -= (1.0f / (180.0f / LWE_PI)) * 0.01f;
        } else if (event->mouse.button == LWE_MOUSE_BUTTON_RIGHT) {
          _step += (1.0f / (180.0f / LWE_PI)) * 0.01f;
        }
      } break;
    }
  }

  lwe_queue_fast_forward(&_window->input_events);
  lwe_queue_fast_forward(&_window->window_events);

  _rotation += _step;
}

static void lwe_application_render()
{
  static lwe_render_stream_t* render_stream =
    lwe_render_stream_create(65535);

  static const float clear_color[4] =
    { 1.0f, 0.0f, 0.6f, 1.0f };

  static const mat4_t model_mat =
    _suzanne->meshes[0].transform;

  static const float radius = 1.5f;

  const mat4_t view_mat =
    mat4_t::look_at(
      vec3_t(cos(_rotation) * radius, sin(_rotation) * radius, radius),
      vec3_t(0.0f, 0.0f, 0.0f),
      vec3_t(0.0f, 1.0f, 0.0f)
    );

  static const float aspect_ratio =
    (float)_manifest->graphics.resolution.width /
    (float)_manifest->graphics.resolution.height;

  static const mat4_t proj_mat =
    mat4_t::perspective(45.0f / (180.0f / LWE_PI), aspect_ratio, 1.0f, 100.0f);

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
    1, &_swap_chain->render_target,
    _depth_stencil_target
  );

  static const lwe_viewport_t viewport = {
    0, 0,
    _manifest->graphics.resolution.width,
    _manifest->graphics.resolution.height
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
    _swap_chain
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

  _manifest =
    lwe_manifest_load("data/manifest");

  _window =
    lwe_window_open(
      _manifest->window.title,
      _manifest->window.width,
      _manifest->window.height
    );

  lwe_array_push(lwe_application_windows(), &_window);

  _swap_chain =
    lwe_swap_chain_create(
      _window, LWE_PIXEL_FORMAT_R8G8B8A8,
      _manifest->graphics.resolution.width,
      _manifest->graphics.resolution.height,
      _manifest->graphics.fullscreen,
      _manifest->graphics.vertical_sync
    );

  lwe_array_push(lwe_application_swap_chains(), &_swap_chain);

  _depth_stencil_target =
    lwe_depth_stencil_target_create(
      LWE_PIXEL_FORMAT_D24_S8,
      _manifest->graphics.resolution.width,
      _manifest->graphics.resolution.height
    );

  lwe_window_show(_window);

  lwe_asset_manager_load("boot.package");

  _blend_state =
    (lwe_blend_state_t*)lwe_asset_manager_find(
      "states/none.blend_state"
    );

  _depth_stencil_state =
    (lwe_depth_stencil_state_t*)lwe_asset_manager_find(
      "states/geometry.depth_stencil_state"
    );

  _rasterizer_state =
    (lwe_rasterizer_state_t*)lwe_asset_manager_find(
      "states/geometry.rasterizer_state"
    );

  _suzanne =
    (lwe_model_t*)lwe_asset_manager_find(
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
    lwe_application_render();
  }
}