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

#include <lwe/assets/d3d11_vertex_shader.h>
#include <lwe/d3d11_render_device.h>

typedef struct lwe_vsbp_t /* vertex_shader_blob_permutation */ {
  uint32_t flags;
  uint32_t vertex_decl;
  lwe_size_t byte_code_len;
  uint8_t byte_code[1];
} lwe_vsbp_t;

typedef struct lwe_vertex_shader_blob_t {
  lwe_size_t num_permutations;
} lwe_vertex_shader_blob_t;

static lwe_asset_t* lwe_vertex_shader_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_VERTEX_SHADER);
  lwe_assert(stream != NULL);

  static const D3D11_INPUT_ELEMENT_DESC iedt[] = {
    { "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR0",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD0",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD1",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TANGENT",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BINORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDWEIGHT",  0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  uint8_t* blob = (uint8_t*)lwe_asset_stream_mrd(stream);

  lwe_vertex_shader_blob_t* vs_blob =
    (lwe_vertex_shader_blob_t*)(blob + 0);

  lwe_vertex_shader_t* vertex_shader =
    (lwe_vertex_shader_t*)lwe_alloc(
      sizeof(lwe_vertex_shader_t) +
      (vs_blob->num_permutations - 1) * sizeof(lwe_shader_permutation_t)
    );

  vertex_shader->num_permutations = vs_blob->num_permutations;

  lwe_vsbp_t* permutation =
    (lwe_vsbp_t*)(blob + sizeof(lwe_vertex_shader_blob_t));

  for (lwe_size_t i = 0; i < vertex_shader->num_permutations; ++i) {
    lwe_d3d11_vsp_t* vsp = (lwe_d3d11_vsp_t*)&vertex_shader->permutations[i];
    vsp->flags = permutation->flags;

    HRESULT hr;
    lwe_fail_if(FAILED(
      hr = _d3d11_device->CreateVertexShader(
        (const void*)&permutation->byte_code[0],
        permutation->byte_code_len,
        NULL,
        &vsp->vs
      )),

      "ID3D11Device::CreateVertexShader failed, hr=%#08X",
      hr
    );

    // Create the appropriate input-layout:
    D3D11_INPUT_ELEMENT_DESC ieds[9];
    lwe_size_t num_ieds = 0;

    uint32_t compontents = permutation->vertex_decl;
    while (compontents) {
      memcpy(
        (void*)&ieds[num_ieds++],
        (const void*)&iedt[lwe_count_trailing_zeros(compontents)],
        sizeof(D3D11_INPUT_ELEMENT_DESC)
      );

      compontents = compontents & (compontents - 1);
    }

    lwe_fail_if(FAILED(
      hr = _d3d11_device->CreateInputLayout(
        &ieds[0],
        num_ieds,
        (const void*)&permutation->byte_code[0],
        permutation->byte_code_len,
        &vsp->input_layout
      )),

      "ID3D11Device::CreateInputLayout failed, hr=%#08X",
      hr
    );

    permutation =
      (lwe_vsbp_t*)(
        ((uint8_t*)permutation) +
        sizeof(lwe_vsbp_t)
        + permutation->byte_code_len - 1
      );
  }

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)vertex_shader;
}

static void lwe_vertex_shader_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_VERTEX_SHADER);

  lwe_vertex_shader_t* vertex_shader = (lwe_vertex_shader_t*)asset;

  for (lwe_size_t i = 0; i < vertex_shader->num_permutations; ++i) {
    lwe_d3d11_vsp_t* vsp = (lwe_d3d11_vsp_t*)&vertex_shader->permutations[i];
    vsp->input_layout->Release();
    vsp->vs->Release();
  }

  lwe_free((void*)vertex_shader);
}

void lwe_vertex_shader_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_VERTEX_SHADER,
    "vertex_shader",
    &lwe_vertex_shader_load,
    &lwe_vertex_shader_unload
  );
}