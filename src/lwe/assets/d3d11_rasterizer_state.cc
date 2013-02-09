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

#include <lwe/assets/d3d11_rasterizer_state.h>
#include <lwe/d3d11_render_device.h>

static D3D11_FILL_MODE _fill_mode_to_d3d(
  lwe_fill_mode_t fill_mode )
{
  switch (fill_mode) {
    case LWE_FILL_WIREFRAME:
      return D3D11_FILL_WIREFRAME;
    case LWE_FILL_SOLID:
      return D3D11_FILL_SOLID;
  }

  lwe_fail("Invalid fill mode!\n");
}

static D3D11_CULL_MODE _cull_mode_to_d3d(
  lwe_cull_mode_t cull_mode )
{
  switch (cull_mode) {
    case LWE_CULL_NONE:
      return D3D11_CULL_NONE;
    case LWE_CULL_FRONT:
      return D3D11_CULL_FRONT;
    case LWE_CULL_BACK:
      return D3D11_CULL_BACK;
  }

  lwe_fail("Invalid cull mode!\n");
}

static lwe_asset_t* lwe_d3d11_rasterizer_state_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(stream != NULL);
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_RASTERIZER_STATE);

  lwe_rasterizer_state_blob_t* rs_blob =
    (lwe_rasterizer_state_blob_t*)lwe_asset_stream_mrd(stream);

  D3D11_RASTERIZER_DESC rd;
  rd.FillMode = _fill_mode_to_d3d(rs_blob->fill_mode);
  rd.CullMode = _cull_mode_to_d3d(rs_blob->cull_mode);
  rd.FrontCounterClockwise = (rs_blob->front_face == LWE_FRONT_FACE_CCW);
  rd.DepthBias = 0;
  rd.SlopeScaledDepthBias = 0.0f;
  rd.DepthBiasClamp = 0.0f;
  rd.DepthClipEnable = TRUE;
  rd.ScissorEnable = rs_blob->scissor_enabled;
  rd.MultisampleEnable = FALSE;
  rd.AntialiasedLineEnable = FALSE;

  lwe_d3d11_rasterizer_state_t* rasterizer_state =
    (lwe_d3d11_rasterizer_state_t*)lwe_alloc(sizeof(lwe_d3d11_rasterizer_state_t));

  HRESULT hr;

  lwe_fail_if(FAILED(
      hr = _d3d11_device->CreateRasterizerState(&rd, &rasterizer_state->rs)
    ),

    "ID3D11Device::CreateRasterizerState failed, hr=%#08X", hr
  );

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)rasterizer_state;
}

static void lwe_d3d11_rasterizer_state_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_RASTERIZER_STATE);

  lwe_d3d11_rasterizer_state_t* rasterizer_state =
    (lwe_d3d11_rasterizer_state_t*)asset;

  rasterizer_state->rs->Release();

  lwe_free((void*)rasterizer_state);
}

// See src/lwe/assets/rasterizer_state.cc
extern bool lwe_rasterizer_state_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd );

void lwe_rasterizer_state_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_RASTERIZER_STATE,
    "rasterizer_state", "rasterizer_state",
    &lwe_d3d11_rasterizer_state_load,
    &lwe_d3d11_rasterizer_state_unload,
  #if defined(LWE_DEBUG_BUILD) || defined(LWE_DEVELOPMENT_BUILD)
    &lwe_rasterizer_state_compile
  #else
    NULL
  #endif
  );
}