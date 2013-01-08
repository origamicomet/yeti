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

#include <lwe/assets/d3d11_pixel_shader.h>
#include <lwe/d3d11_render_device.h>

typedef struct lwe_psbp_t /* pixel_shader_blob_permutation */ {
  uint32_t flags;
  lwe_size_t byte_code_len;
  uint8_t byte_code[1];
} lwe_psbp_t;

typedef struct lwe_pixel_shader_blob_t {
  lwe_size_t num_permutations;
} lwe_pixel_shader_blob_t;

static lwe_asset_t* lwe_pixel_shader_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_PIXEL_SHADER);
  lwe_assert(stream != NULL);

  uint8_t* blob = (uint8_t*)lwe_asset_stream_mrd(stream);

  lwe_pixel_shader_blob_t* ps_blob =
    (lwe_pixel_shader_blob_t*)(blob + 0);

  lwe_pixel_shader_t* pixel_shader =
    (lwe_pixel_shader_t*)lwe_alloc(
      sizeof(lwe_pixel_shader_t) +
      (ps_blob->num_permutations - 1) * sizeof(lwe_shader_permutation_t)
    );

  pixel_shader->num_permutations = ps_blob->num_permutations;

  lwe_psbp_t* permutation =
    (lwe_psbp_t*)(blob + sizeof(lwe_pixel_shader_blob_t));

  for (lwe_size_t i = 0; i < pixel_shader->num_permutations; ++i) {
    lwe_d3d11_psp_t* psp = (lwe_d3d11_psp_t*)&pixel_shader->permutations[i];
    psp->flags = permutation->flags;

    HRESULT hr;
    lwe_fail_if(FAILED(
      hr = _d3d11_device->CreatePixelShader(
      (const void*)&permutation->byte_code[0],
        permutation->byte_code_len,
        NULL,
        &psp->ps
      )),

      "ID3D11Device::CreatePixelShader failed, hr=%#08X",
      hr
    );

    permutation =
      (lwe_psbp_t*)(
        ((uint8_t*)permutation) +
        sizeof(lwe_psbp_t)
        + permutation->byte_code_len - 1
      );
  }

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)pixel_shader;
}

static void lwe_pixel_shader_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_PIXEL_SHADER);

  lwe_pixel_shader_t* pixel_shader = (lwe_pixel_shader_t*)asset;

  for (lwe_size_t i = 0; i < pixel_shader->num_permutations; ++i) {
    lwe_d3d11_psp_t* psp = (lwe_d3d11_psp_t*)&pixel_shader->permutations[i];
    psp->ps->Release();
  }

  lwe_free((void*)pixel_shader);
}

void lwe_pixel_shader_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_PIXEL_SHADER,
    "pixel_shader",
    &lwe_pixel_shader_load,
    &lwe_pixel_shader_unload
  );
}