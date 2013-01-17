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
#include <lwe/assets/d3d11_texture.h>
#include <lwe/assets/d3d11_vertex_shader.h>
#include <lwe/assets/d3d11_pixel_shader.h>
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
  const lwe_render_cmd_sort_key_t sort_key,
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

LWE_INLINE static void _set_viewports(
  const lwe_render_cmd_sort_key_t sort_key,
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
  const lwe_render_cmd_sort_key_t sort_key,
  const lwe_clear_cmd_t* cmd )
{
  ID3D11RenderTargetView* rtvs[8];
  ID3D11DepthStencilView* dsv;

  _d3d11_context->OMGetRenderTargets(8, &rtvs[0], &dsv);

  if (cmd->clear_render_targets) {
    for (lwe_size_t rt = 0; rt < 8; ++rt) {
      if (!rtvs[rt])
        continue;

      _d3d11_context->ClearRenderTargetView(rtvs[rt], &cmd->rgba[0]);
    }
  }

  UINT clear_flags = 0;

  if (cmd->clear_depth_target)
    clear_flags |= D3D11_CLEAR_DEPTH;

  if (cmd->clear_stencil_target)
    clear_flags |= D3D11_CLEAR_STENCIL;

  if (dsv) {
    _d3d11_context->ClearDepthStencilView(
      dsv, clear_flags, cmd->depth, cmd->stencil
    );
  }
}

LWE_INLINE static void _generate_mips(
  const lwe_render_cmd_sort_key_t sort_key,
  const lwe_generate_mips_cmd_t* cmd )
{
  lwe_d3d11_render_target_t* render_target =
    (lwe_d3d11_render_target_t*)cmd->target;

  _d3d11_context->GenerateMips(render_target->srv);
}

LWE_INLINE static void _draw(
  lwe_size_t constant_buffer_offset,
  const lwe_render_cmd_sort_key_t sort_key,
  const lwe_draw_cmd_t* cmd )
{
#if defined(LWE_DEBUG_BUILD) || defined(LWE_DEVELOPMENT_BUILD)
  lwe_fail_if(
    cmd->vertex_decl.components !=
    cmd->material->vertex_shader->vertex_decl.components,
    "Mesh and vertex shader vertex declarations don't match!"
  );
#endif

  const lwe_d3d11_constant_buffer_t* constant_buffer =
    (const lwe_d3d11_constant_buffer_t*)cmd->material->constants;

  _d3d11_context->VSSetConstantBuffers(
    constant_buffer_offset, 1,
    constant_buffer ? &constant_buffer->buffer : NULL
  );

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
    _d3d11_context->VSSetShaderResources(tex, 1, &textures[tex]->srv);
    _d3d11_context->PSSetShaderResources(tex, 1, &textures[tex]->srv);
  }

  _d3d11_context->IASetInputLayout(
    vertex_shader->input_layout
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
    lwe_vertex_declaration_to_size(cmd->vertex_decl);

  lwe_d3d11_vertex_buffer_t* vertices =
    (lwe_d3d11_vertex_buffer_t*)cmd->vertices;

  _d3d11_context->IASetVertexBuffers(
    0, 1, &vertices->buffer, &vertex_stride, 0
  );

  _d3d11_context->DrawIndexed(
    cmd->num_indicies, 0, 0
  );
}

LWE_INLINE static void _present(
  lwe_swap_chain_t* swap_chain,
  const lwe_render_cmd_sort_key_t sort_key,
  const lwe_present_cmd_t* cmd )
{
  ((lwe_d3d11_swap_chain_t*)swap_chain)->swap_chain->Present(0, 0);
}

LWE_INLINE static void _dispatch_render_command(
  lwe_size_t constant_buffer_offset,
  lwe_swap_chain_t* swap_chain,
  const lwe_render_cmd_sort_key_t sort_key,
  const lwe_render_cmd_t* cmd )
{
  switch (cmd->type) {
    case LWE_RENDER_COMMAND_TYPE_SET_RENDER_TARGETS:
      _set_render_targets(sort_key, (const lwe_set_render_targets_cmd_t*)cmd);
    break;

    case LWE_RENDER_COMMAND_TYPE_SET_VIEWPORTS:
      _set_viewports(sort_key, (const lwe_set_viewports_cmd_t*)cmd);
    break;

    case LWE_RENDER_COMMAND_TYPE_CLEAR:
      _clear(sort_key, (const lwe_clear_cmd_t*)cmd);
    break;

    case LWE_RENDER_COMMAND_TYPE_GENERATE_MIPS:
      _generate_mips(sort_key, (const lwe_generate_mips_cmd_t*)cmd);
    break;

    case LWE_RENDER_COMMAND_TYPE_DRAW:
      _draw(constant_buffer_offset, sort_key, (const lwe_draw_cmd_t*)cmd);
    break;

    case LWE_RENDER_COMMAND_TYPE_PRESENT:
      _present(swap_chain, sort_key, (const lwe_present_cmd_t*)cmd);
    break;
  }
}

void lwe_render_device_dispatch(
  lwe_size_t num_constant_buffers,
  lwe_constant_buffer_t** constant_buffers_,
  lwe_swap_chain_t* swap_chain,
  lwe_size_t num_streams,
  const lwe_render_cmd_stream_t** streams )
{
  lwe_assert(num_streams > 0);
  lwe_assert(streams != NULL);

  lwe_d3d11_constant_buffer_t** constant_buffers =
    (lwe_d3d11_constant_buffer_t**)constant_buffers_;

  for (lwe_size_t cb = 0; cb < num_constant_buffers; ++cb) {
    _d3d11_context->VSSetConstantBuffers(cb, 1, &constant_buffers[cb]->buffer);
    _d3d11_context->PSSetConstantBuffers(cb, 1, &constant_buffers[cb]->buffer);
  }

  for (lwe_size_t stream = 0; stream < num_streams; ++stream) {
    for (lwe_size_t key = 0; key < streams[stream]->next_key; ++key) {
      const lwe_render_cmd_t* cmd =
        (const lwe_render_cmd_t*)(
          streams[stream]->cmd_buffer +
          streams[stream]->keys[key].offset
        );

      _dispatch_render_command(
        num_constant_buffers, swap_chain, streams[stream]->keys[key], cmd
      );
    }
  }
}