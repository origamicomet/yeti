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

#include <lwe/d3d11_render_device.h>
#include <lwe/d3d11_swap_chain.h>
#include <lwe/d3d11_render_target.h>
#include <lwe/d3d11_depth_stencil_target.h>
#include <lwe/d3d11_index_buffer.h>
#include <lwe/d3d11_vertex_buffer.h>
#include <lwe/d3d11_constant_buffer.h>
#include <lwe/d3d11_input_layout.h>
#include <lwe/assets/d3d11_texture.h>
#include <lwe/assets/d3d11_vertex_shader.h>
#include <lwe/assets/d3d11_pixel_shader.h>
#include <lwe/assets/d3d11_blend_state.h>
#include <lwe/assets/d3d11_depth_stencil_state.h>
#include <lwe/assets/d3d11_rasterizer_state.h>
#include <lwe/assets/material.h>

D3D_FEATURE_LEVEL    _d3d_feature_level = D3D_FEATURE_LEVEL_11_0;
IDXGIFactory*        _dxgi_factory      = NULL;
IDXGIAdapter*        _dxgi_adapter      = NULL;
ID3D11Device*        _d3d11_device      = NULL;
ID3D11DeviceContext* _d3d11_context     = NULL;

void lwe_render_device_create(
  uint32_t adapter_id )
{
  HRESULT hr;

  lwe_fail_if(FAILED(
    hr = CreateDXGIFactory(
      __uuidof(IDXGIFactory),
      (void**)&_dxgi_factory
    )),

    "CreateDXGIFactory failed, hr=%#08X", hr
  );

  lwe_fail_if(FAILED(
    hr = _dxgi_factory->EnumAdapters(
      adapter_id,
      &_dxgi_adapter
    )),

    "IDXGIFactory::EnumAdapters failed, hr=%#08X", hr
  );

  static const D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

#if defined (LWE_DEBUG_BUILD)
  static const UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
  static const UINT flags = 0;
#endif

  lwe_fail_if(FAILED(
    hr = D3D11CreateDevice(
      _dxgi_adapter,
      D3D_DRIVER_TYPE_UNKNOWN,
      NULL, flags,
      &feature_level, 1, /* D3D11 only */
      D3D11_SDK_VERSION,
      &_d3d11_device,
      &_d3d_feature_level,
      &_d3d11_context
    )),

    "D3D11CreateDevice failed, hr=%#08X", hr
  );

  lwe_log("Created render device (D3D11), adapter_id=%u\n", adapter_id);
}

LWE_INLINE static void _set_render_targets(
  const lwe_set_render_targets_cmd_t* cmd )
{
  ID3D11RenderTargetView* rtvs[8];
  memset((void*)&rtvs[0], 0, 8 * sizeof(ID3D11RenderTargetView*));

  lwe_d3d11_render_target_t** render_targets =
    (lwe_d3d11_render_target_t**)cmd->targets;

  for (lwe_size_t rt = 0; rt < cmd->num_targets; ++rt)
    rtvs[rt] = (render_targets[rt]) ? render_targets[rt]->rtv : NULL;

  lwe_d3d11_depth_stencil_target_t* depth_stencil_target =
    (lwe_d3d11_depth_stencil_target_t*)cmd->dst;

  _d3d11_context->OMSetRenderTargets(
    cmd->num_targets, &rtvs[0],
    depth_stencil_target ? depth_stencil_target->dstv : NULL
  );
}

LWE_INLINE static void _set_blend_state(
  const lwe_set_blend_state_cmd_t* cmd )
{
  lwe_d3d11_blend_state_t* blend_state =
    (lwe_d3d11_blend_state_t*)cmd->blend_state;

  _d3d11_context->OMSetBlendState(
    blend_state->bs, NULL, 0xffffffffu
  );
}

LWE_INLINE static void _set_depth_stencil_state(
  const lwe_set_depth_stencil_state_cmd_t* cmd )
{
  lwe_d3d11_depth_stencil_state_t* depth_stencil_state =
    (lwe_d3d11_depth_stencil_state_t*)cmd->depth_stencil_state;

  _d3d11_context->OMSetDepthStencilState(
    depth_stencil_state->dss, cmd->stencil_ref
  );
}

LWE_INLINE static void _set_rasterizer_state(
  const lwe_set_rasterizer_state_cmd_t* cmd )
{
  lwe_d3d11_rasterizer_state_t* rasterizer_state =
    (lwe_d3d11_rasterizer_state_t*)cmd->rasterizer_state;

  _d3d11_context->RSSetState(
    rasterizer_state->rs
  );
}

LWE_INLINE static void _set_viewports(
  const lwe_set_viewports_cmd_t* cmd )
{
  D3D11_VIEWPORT viewports[8];

  for (lwe_size_t vp = 0; vp < cmd->num_viewports; ++vp) {
    viewports[vp].TopLeftX = cmd->viewports[vp].min_x;
    viewports[vp].TopLeftY = cmd->viewports[vp].min_y;
    viewports[vp].Width = cmd->viewports[vp].max_x - viewports[vp].TopLeftX;
    viewports[vp].Height = cmd->viewports[vp].max_y - viewports[vp].TopLeftY;
    viewports[vp].MinDepth = 0.0f;
    viewports[vp].MaxDepth = 1.0f;
  }

  _d3d11_context->RSSetViewports(cmd->num_viewports, &viewports[0]);
}

LWE_INLINE static void _clear(
  const lwe_clear_cmd_t* cmd )
{
  ID3D11RenderTargetView* rtvs[8];
  ID3D11DepthStencilView* dsv;

  _d3d11_context->OMGetRenderTargets(8, &rtvs[0], &dsv);

  if (cmd->flags & LWE_CLEAR_COLOR) {
    for (lwe_size_t rt = 0; rt < 8; ++rt) {
      if (!rtvs[rt])
        continue;

      _d3d11_context->ClearRenderTargetView(rtvs[rt], &cmd->rgba[0]);
    }
  }

  UINT flags = 0;

  if (cmd->flags & LWE_CLEAR_DEPTH)
    flags |= D3D11_CLEAR_DEPTH;

  if (cmd->flags & LWE_CLEAR_STENCIL)
    flags |= D3D11_CLEAR_STENCIL;

  if (dsv) {
    _d3d11_context->ClearDepthStencilView(
      dsv, flags, cmd->depth, cmd->stencil
    );
  }
}

LWE_INLINE static void _generate_mips(
  const lwe_generate_mips_cmd_t* cmd )
{
  lwe_d3d11_render_target_t* render_target =
    (lwe_d3d11_render_target_t*)cmd->target;

  _d3d11_context->GenerateMips(render_target->srv);
}

LWE_INLINE static void _draw(
  const lwe_draw_cmd_t* cmd )
{
  const lwe_d3d11_constant_buffer_t** constant_buffers =
    (const lwe_d3d11_constant_buffer_t**)&cmd->constant_buffers[0];

  for (lwe_size_t i = 0; i < cmd->num_constant_buffers; ++i) {
    _d3d11_context->VSSetConstantBuffers(
      i, 1, constant_buffers[i] ? &constant_buffers[i]->buffer : NULL
    );
  }

  const lwe_d3d11_constant_buffer_t* material_constants =
    (const lwe_d3d11_constant_buffer_t*)cmd->material->constants;

  if (material_constants) {
    _d3d11_context->VSSetConstantBuffers(
      cmd->num_constant_buffers, 1, &material_constants->buffer
    );
  }

  lwe_d3d11_vertex_shader_t* vertex_shader =
    (lwe_d3d11_vertex_shader_t*)cmd->material->vertex_shader;

  lwe_d3d11_pixel_shader_t* pixel_shader =
    (lwe_d3d11_pixel_shader_t*)cmd->material->pixel_shader;

  _d3d11_context->VSSetShader(
    vertex_shader->vs, NULL, 0
  );
  
  _d3d11_context->PSSetShader(
    pixel_shader->ps, NULL, 0
  );

  lwe_d3d11_texture_t** textures =
    (lwe_d3d11_texture_t**)&cmd->material->textures[0];

  for (lwe_size_t tex = 0; tex < cmd->material->num_textures; ++tex) {
    // _d3d11_context->VSSetShaderResources(tex, 1, &textures[tex]->srv);
    _d3d11_context->PSSetShaderResources(tex, 1, &textures[tex]->srv);
    _d3d11_context->PSSetSamplers(tex, 1, &textures[tex]->ss);
  }

  lwe_d3d11_input_layout_t* input_layout =
    (lwe_d3d11_input_layout_t*)cmd->input_layout;

  _d3d11_context->IASetInputLayout(
    input_layout->input_layout
  );
  
  _d3d11_context->IASetPrimitiveTopology(
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
  );

  lwe_d3d11_index_buffer_t* indicies =
    (lwe_d3d11_index_buffer_t*)cmd->indicies;
  
  _d3d11_context->IASetIndexBuffer(
    indicies->buffer, DXGI_FORMAT_R32_UINT, 0
  );

  const UINT vertex_stride =
    input_layout->stride;

  lwe_d3d11_vertex_buffer_t* vertices =
    (lwe_d3d11_vertex_buffer_t*)cmd->vertices;

  const UINT offset = 0;

  _d3d11_context->IASetVertexBuffers(
    0, 1, &vertices->buffer, &vertex_stride, &offset
  );

  _d3d11_context->DrawIndexed(
    cmd->num_indicies, 0, 0
  );
}

LWE_INLINE static void _present(
  const lwe_present_cmd_t* cmd )
{
  ((lwe_d3d11_swap_chain_t*)cmd->swap_chain)->swap_chain->Present(0, 0);
}

LWE_INLINE static const lwe_render_cmd_t* _dispatch_render_command(
  const lwe_render_cmd_t* cmd_ )
{
  switch (cmd_->type) {
    case LWE_RENDER_COMMAND_TYPE_SET_RENDER_TARGETS: {
      const lwe_set_render_targets_cmd_t* cmd =
        (const lwe_set_render_targets_cmd_t*)cmd_;

      _set_render_targets(cmd);

      return (const lwe_render_cmd_t*)(
        ((uint8_t*)cmd_) +
        sizeof(lwe_set_render_targets_cmd_t) +
        (cmd->num_targets - 1) * sizeof(lwe_render_target_t*)
      );
    } break;

    case LWE_RENDER_COMMAND_TYPE_SET_BLEND_STATE: {
      const lwe_set_blend_state_cmd_t* cmd =
        (const lwe_set_blend_state_cmd_t*)cmd_;

        _set_blend_state(cmd);

        return (const lwe_render_cmd_t*)(
          ((uint8_t*)cmd_) + sizeof(lwe_set_blend_state_cmd_t)
        );
    } break;

    case LWE_RENDER_COMMAND_TYPE_SET_DEPTH_STENCIL_STATE: {
      const lwe_set_depth_stencil_state_cmd_t* cmd =
        (const lwe_set_depth_stencil_state_cmd_t*)cmd_;

        _set_depth_stencil_state(cmd);

        return (const lwe_render_cmd_t*)(
          ((uint8_t*)cmd_) + sizeof(lwe_set_depth_stencil_state_cmd_t)
        );
    } break;

    case LWE_RENDER_COMMAND_TYPE_SET_RASTERIZER_STATE: {
      const lwe_set_rasterizer_state_cmd_t* cmd =
        (const lwe_set_rasterizer_state_cmd_t*)cmd_;

        _set_rasterizer_state(cmd);

        return (const lwe_render_cmd_t*)(
          ((uint8_t*)cmd_) + sizeof(lwe_set_rasterizer_state_cmd_t)
        );
    } break;

    case LWE_RENDER_COMMAND_TYPE_SET_VIEWPORTS: {
      const lwe_set_viewports_cmd_t* cmd =
        (const lwe_set_viewports_cmd_t*)cmd_;

      _set_viewports(cmd);

      return (const lwe_render_cmd_t*)(
        ((uint8_t*)cmd_) +
        sizeof(lwe_set_viewports_cmd_t) +
        (cmd->num_viewports - 1) * sizeof(lwe_viewport_t)
      );
    } break;

    case LWE_RENDER_COMMAND_TYPE_CLEAR: {
      _clear((const lwe_clear_cmd_t*)cmd_);

      return (const lwe_render_cmd_t*)(
        ((uint8_t*)cmd_) +
        sizeof(lwe_clear_cmd_t)
      );
    } break;

    case LWE_RENDER_COMMAND_TYPE_GENERATE_MIPS: {
      _generate_mips((const lwe_generate_mips_cmd_t*)cmd_);

      return (const lwe_render_cmd_t*)(
        ((uint8_t*)cmd_) +
        sizeof(lwe_generate_mips_cmd_t)
      );
    } break;

    case LWE_RENDER_COMMAND_TYPE_DRAW: {
      const lwe_draw_cmd_t* cmd =
        (const lwe_draw_cmd_t*)cmd_;

      _draw(cmd);

      return (const lwe_render_cmd_t*)(
        ((uint8_t*)cmd_) +
        sizeof(lwe_draw_cmd_t) +
        cmd->num_constant_buffers * sizeof(lwe_constant_buffer_t*) -
        sizeof(lwe_constant_buffer_t*)
      );
    } break;

    case LWE_RENDER_COMMAND_TYPE_PRESENT: {
      _present((const lwe_present_cmd_t*)cmd_);

      return (const lwe_render_cmd_t*)(
        ((uint8_t*)cmd_) +
        sizeof(lwe_present_cmd_t)
      );
    } break;
  }

  return NULL;
}

void lwe_render_device_dispatch(
  lwe_size_t num_streams,
  const lwe_render_stream_t** streams )
{
  lwe_assert(num_streams > 0);
  lwe_assert(streams != NULL);

  for (lwe_size_t stream_idx = 0; stream_idx < num_streams; ++stream_idx) {
    const uint8_t* stream = &streams[stream_idx]->buffer[0];

    while (stream && (stream < (&streams[stream_idx]->buffer[streams[stream_idx]->next_command]))) {
      const lwe_render_cmd_t* cmd = (const lwe_render_cmd_t*)stream;
      stream = (uint8_t*)_dispatch_render_command(cmd);
    }
  }
}