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

#include <lwe/assets/d3d11_blend_state.h>
#include <lwe/d3d11_render_device.h>

static D3D11_BLEND _blend_factor_to_d3d(
  lwe_blend_factor_t blend_factor )
{
  switch (blend_factor) {
    case LWE_BLEND_FACTOR_ZERO:
      return D3D11_BLEND_ZERO;
    case LWE_BLEND_FACTOR_ONE:
      return D3D11_BLEND_ONE;
    case LWE_BLEND_FACTOR_SRC_COLOR:
      return D3D11_BLEND_SRC_COLOR;
    case LWE_BLEND_FACTOR_INV_SRC_COLOR:
      return D3D11_BLEND_INV_SRC_COLOR;
    case LWE_BLEND_FACTOR_SRC_ALPHA:
      return D3D11_BLEND_SRC_ALPHA;
    case LWE_BLEND_FACTOR_INV_SRC_ALPHA:
      return D3D11_BLEND_INV_SRC_ALPHA;
    case LWE_BLEND_FACTOR_DEST_ALPHA:
      return D3D11_BLEND_DEST_ALPHA;
    case LWE_BLEND_FACTOR_INV_DEST_ALPHA:
      return D3D11_BLEND_INV_DEST_ALPHA;
    case LWE_BLEND_FACTOR_DEST_COLOR:
      return D3D11_BLEND_DEST_COLOR;
    case LWE_BLEND_FACTOR_INV_DEST_COLOR:
      return D3D11_BLEND_INV_DEST_COLOR;
    case LWE_BLEND_FACTOR_SRC_ALPHA_SAT:
      return D3D11_BLEND_SRC_ALPHA_SAT;
  }

  lwe_fail("Invalid blend factor!\n");
}

static D3D11_BLEND_OP _blend_operation_to_d3d(
  lwe_blend_operation_t blend_op )
{
  switch (blend_op) {
    case LWE_BLEND_OP_ADD:
      return D3D11_BLEND_OP_ADD;
    case LWE_BLEND_OP_SUBTRACT:
      return D3D11_BLEND_OP_SUBTRACT;
    case LWE_BLEND_OP_REV_SUBTRACT:
      return D3D11_BLEND_OP_REV_SUBTRACT;
    case LWE_BLEND_OP_MIN:
      return D3D11_BLEND_OP_MIN;
    case LWE_BLEND_OP_MAX:
      return D3D11_BLEND_OP_MAX;
  }

  lwe_fail("Invalid blend operation!\n");
}

static lwe_asset_t* lwe_d3d11_blend_state_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(stream != NULL);
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_BLEND_STATE);

  lwe_blend_state_blob_t* bs_blob =
    (lwe_blend_state_blob_t*)lwe_asset_stream_mrd(stream);

  D3D11_BLEND_DESC bd;
  bd.AlphaToCoverageEnable = FALSE;
  bd.IndependentBlendEnable = FALSE;

  D3D11_RENDER_TARGET_BLEND_DESC* rtbd =
    &bd.RenderTarget[0];

  rtbd->BlendEnable = bs_blob->enabled;
  rtbd->SrcBlend = _blend_factor_to_d3d(bs_blob->color.src);
  rtbd->DestBlend = _blend_factor_to_d3d(bs_blob->color.dest);
  rtbd->BlendOp = _blend_operation_to_d3d(bs_blob->color.op);
  rtbd->SrcBlendAlpha = _blend_factor_to_d3d(bs_blob->color.src);
  rtbd->DestBlendAlpha = _blend_factor_to_d3d(bs_blob->color.dest);
  rtbd->BlendOpAlpha = _blend_operation_to_d3d(bs_blob->color.op);
  rtbd->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  lwe_d3d11_blend_state_t* blend_state =
    (lwe_d3d11_blend_state_t*)lwe_alloc(sizeof(lwe_d3d11_blend_state_t));

  HRESULT hr;

  lwe_fail_if(FAILED(
      hr = _d3d11_device->CreateBlendState(&bd, &blend_state->bs)
    ),

    "ID3D11Device::CreateBlendState failed, hr=%#08X", hr
  );

  return (lwe_asset_t*)blend_state;
}

static void lwe_d3d11_blend_state_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_BLEND_STATE);

  lwe_d3d11_blend_state_t* blend_state = (lwe_d3d11_blend_state_t*)asset;
  blend_state->bs->Release();
  lwe_free((void*)blend_state);
}

// See src/lwe/assets/blend_state.cc
extern bool lwe_blend_state_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd );

void lwe_blend_state_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_BLEND_STATE,
    "blend_state", "blend_state",
    &lwe_d3d11_blend_state_load,
    &lwe_d3d11_blend_state_unload,
    &lwe_blend_state_compile
  );
}