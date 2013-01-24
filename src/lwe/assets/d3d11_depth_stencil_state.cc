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

#include <lwe/assets/d3d11_depth_stencil_state.h>
#include <lwe/d3d11_render_device.h>

static D3D11_COMPARISON_FUNC _comparison_func_to_d3d(
  lwe_comparison_func_t comparison_func )
{
  switch (comparison_func) {
    case LWE_COMPARISON_NEVER:
      return D3D11_COMPARISON_NEVER;
    case LWE_COMPARISON_LESS:
      return D3D11_COMPARISON_LESS;
    case LWE_COMPARISON_EQUAL:
      return D3D11_COMPARISON_EQUAL;
    case LWE_COMPARISON_LESS_EQUAL:
      return D3D11_COMPARISON_LESS_EQUAL;
    case LWE_COMPARISON_GREATER:
      return D3D11_COMPARISON_GREATER;
    case LWE_COMPARISON_NOT_EQUAL:
      return D3D11_COMPARISON_NOT_EQUAL;
    case LWE_COMPARISON_GREATER_EQUAL:
      return D3D11_COMPARISON_GREATER_EQUAL;
    case LWE_COMPARISON_ALWAYS:
      return D3D11_COMPARISON_ALWAYS;
  }

  lwe_fail("Invalid comparison function!\n");
}

static D3D11_STENCIL_OP _stencil_operation_to_d3d(
  lwe_stencil_operation_t stencil_op )
{
  switch (stencil_op) {
    case LWE_STENCIL_OP_KEEP:
      return D3D11_STENCIL_OP_KEEP;
    case LWE_STENCIL_OP_ZERO:
      return D3D11_STENCIL_OP_ZERO;
    case LWE_STENCIL_OP_REPLACE:
      return D3D11_STENCIL_OP_REPLACE;
    case LWE_STENCIL_OP_INCR_SAT:
      return D3D11_STENCIL_OP_INCR_SAT;
    case LWE_STENCIL_OP_DECR_SAT:
      return D3D11_STENCIL_OP_DECR_SAT;
    case LWE_STENCIL_OP_INVERT:
      return D3D11_STENCIL_OP_INVERT;
    case LWE_STENCIL_OP_INCR:
      return D3D11_STENCIL_OP_INCR;
    case LWE_STENCIL_OP_DECR:
      return D3D11_STENCIL_OP_DECR;
  }

  lwe_fail("Invalid stencil operation!\n");
}

static lwe_asset_t* lwe_d3d11_depth_stencil_state_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(stream != NULL);
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_DEPTH_STENCIL_STATE);

  lwe_depth_stencil_state_blob_t* dss_blob =
    (lwe_depth_stencil_state_blob_t*)lwe_asset_stream_mrd(stream);

  D3D11_DEPTH_STENCIL_DESC dsd;
  dsd.DepthEnable = dss_blob->depth.tests;
  dsd.DepthWriteMask = (dss_blob->depth.writes) ?
    D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  dsd.DepthFunc = _comparison_func_to_d3d(dss_blob->depth.func);
  dsd.StencilEnable = dss_blob->stencil.enabled;
  dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

  dsd.FrontFace.StencilFailOp =
    _stencil_operation_to_d3d(dss_blob->stencil.front_face.stencil_fail_op);
  dsd.FrontFace.StencilDepthFailOp =
    _stencil_operation_to_d3d(dss_blob->stencil.front_face.depth_fail_op);
  dsd.FrontFace.StencilPassOp =
    _stencil_operation_to_d3d(dss_blob->stencil.front_face.pass_op);
  dsd.FrontFace.StencilFunc =
    _comparison_func_to_d3d(dss_blob->stencil.front_face.func);

  dsd.BackFace.StencilFailOp =
    _stencil_operation_to_d3d(dss_blob->stencil.back_face.stencil_fail_op);
  dsd.BackFace.StencilDepthFailOp =
    _stencil_operation_to_d3d(dss_blob->stencil.back_face.depth_fail_op);
  dsd.BackFace.StencilPassOp =
    _stencil_operation_to_d3d(dss_blob->stencil.back_face.pass_op);
  dsd.BackFace.StencilFunc =
    _comparison_func_to_d3d(dss_blob->stencil.back_face.func);

  lwe_d3d11_depth_stencil_state_t* depth_stencil_state =
    (lwe_d3d11_depth_stencil_state_t*)lwe_alloc(
      sizeof(lwe_d3d11_depth_stencil_state_t)
    );

  HRESULT hr;

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateDepthStencilState(
      &dsd, &depth_stencil_state->dss
    )),

    "ID3D11Device::CreateDepthStencilState failed, hr=%#08X", hr
  );

  return (lwe_asset_t*)depth_stencil_state;
}

static void lwe_d3d11_depth_stencil_state_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_DEPTH_STENCIL_STATE);

  lwe_d3d11_depth_stencil_state_t* depth_stencil_state =
    (lwe_d3d11_depth_stencil_state_t*)asset;

  depth_stencil_state->dss->Release();

  lwe_free((void*)depth_stencil_state);
}

// See src/lwe/assets/depth_stencil_state.cc
extern bool lwe_depth_stencil_state_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd );

void lwe_depth_stencil_state_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_DEPTH_STENCIL_STATE,
    "depth_stencil_state", "depth_stencil_state",
    &lwe_d3d11_depth_stencil_state_load,
    &lwe_d3d11_depth_stencil_state_unload,
    &lwe_depth_stencil_state_compile
  );
}