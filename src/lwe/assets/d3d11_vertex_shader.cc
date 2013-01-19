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
#include <lwe/d3d_shader_compiler.h>

typedef struct lwe_vertex_shader_blob_t {
  lwe_vertex_declaration_t vertex_decl;
  lwe_size_t byte_code_len;
  uint8_t byte_code[1];
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

  lwe_d3d11_vertex_shader_t* vertex_shader =
    (lwe_d3d11_vertex_shader_t*)lwe_alloc(sizeof(lwe_d3d11_vertex_shader_t));

  vertex_shader->vertex_decl = vs_blob->vertex_decl;

  HRESULT hr;
  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateVertexShader(
      (const void*)&vs_blob->byte_code[0],
      vs_blob->byte_code_len,
      NULL,
      &vertex_shader->vs
    )),

    "ID3D11Device::CreateVertexShader failed, hr=%#08X", hr
  );

  lwe_size_t num_ieds = 0;
  D3D11_INPUT_ELEMENT_DESC ieds[9];
  uint32_t components = vs_blob->vertex_decl.components;

  while (components) {
    memcpy(
      (void*)&ieds[num_ieds++],
      (const void*)&iedt[lwe_count_trailing_zeros(components)],
      sizeof(D3D11_INPUT_ELEMENT_DESC)
    );

    components = components & (components - 1);
  }

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateInputLayout(
      &ieds[0],
      num_ieds,
      (const void*)&vs_blob->byte_code[0],
      vs_blob->byte_code_len,
      &vertex_shader->input_layout
    )),

    "ID3D11Device::CreateInputLayout failed, hr=%#08X", hr
  );

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)vertex_shader;
}

static void lwe_vertex_shader_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_VERTEX_SHADER);

  lwe_d3d11_vertex_shader_t* vertex_shader =
	(lwe_d3d11_vertex_shader_t*)asset;

  vertex_shader->input_layout->Release();
  vertex_shader->vs->Release();

  lwe_free((void*)vertex_shader);
}

static bool _determine_vertex_decl(
  ID3D11ShaderReflection* shader_ref,
  lwe_vertex_declaration_t* vertex_decl )
{
  lwe_assert(shader_ref != NULL);
  lwe_assert(vertex_decl != NULL);

  HRESULT hr;
  D3D11_SHADER_DESC desc;

  lwe_fail_if(
    FAILED(hr = shader_ref->GetDesc(&desc)),
    "ID3D11ShaderReflection::GetDesc failed, hr=%#08X", hr
  );

  vertex_decl->components = 0;

  for (lwe_size_t i = 0; i < desc.InputParameters; ++i) {
    D3D11_SIGNATURE_PARAMETER_DESC param_desc;

    lwe_fail_if(
      FAILED(hr = shader_ref->GetInputParameterDesc(i, &param_desc)),
      "ID3D11ShaderReflection::GetInputParameterDesc failed, hr=%#08X", hr
    );

    uint32_t component = 0;

    if (strcmp("POSITION", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_POSITION;
    else if (strcmp("COLOR0", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_COLOR0;
    else if (strcmp("TEXCOORD", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_TEXCOORD0;
    else if (strcmp("TEXCOORD1", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_TEXCOORD1;
    else if (strcmp("NORMAL", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_NORMAL;
    else if (strcmp("TANGENT", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_TANGENT;
    else if (strcmp("BINORMAL", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_BINORMAL;
    else if (strcmp("BONEINDICES", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_BONEINDICES;
    else if (strcmp("BONEWEIGHTS", param_desc.SemanticName) == 0)
      component = LWE_VERTEX_COMPONENTS_BONEWEIGHTS;
    else {
      lwe_log("  > Unknown input semantic `%s`\n", param_desc.SemanticName);
      return FALSE;
    }

    // Check that the order is correct (lower bits specifed already):
    if ((vertex_decl->components & ~(component - 1)) != 0) {
      lwe_log("  > Vertex components out of order!\n");
      lwe_log("  > When trying to set %s\n", param_desc.SemanticName);
      lwe_log("  >   POSITION: [%c]\n", vertex_decl->position ? 'X' : ' ');
      lwe_log("  >   COLOR0: [%c]\n", vertex_decl->color0 ? 'X' : ' ');
      lwe_log("  >   TEXCOORD0: [%c]\n", vertex_decl->texcoord0 ? 'X' : ' ');
      lwe_log("  >   TEXCOORD1: [%c]\n", vertex_decl->texcoord1 ? 'X' : ' ');
      lwe_log("  >   NORMAL: [%c]\n", vertex_decl->normal ? 'X' : ' ');
      lwe_log("  >   TANGENT: [%c]\n", vertex_decl->tangent ? 'X' : ' ');
      lwe_log("  >   BINORMAL: [%c]\n", vertex_decl->binormal ? 'X' : ' ');
      lwe_log("  >   BONEINDICES: [%c]\n", vertex_decl->boneindices ? 'X' : ' ');
      lwe_log("  >   BONEWEIGHTS: [%c]\n", vertex_decl->boneweights ? 'X' : ' ');
      return FALSE;
    }

    vertex_decl->components |= component;
  }

  return TRUE;
}

static bool lwe_vertex_shader_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_VERTEX_SHADER);
  lwe_assert(acd != NULL);

  const lwe_size_t src_len = lwe_file_size(acd->in);
  void* src = lwe_alloc(src_len);

  if (fread(src, 1, src_len, acd->in) != src_len) {
    lwe_log("  > Unexpected end-of-file!\n");
    lwe_free(src);
    return FALSE;
  }

  const D3D_SHADER_MACRO defines[] = {
    { "VERTEX_SHADER", "1" },
    { "D3D11",         "1" },
	  { NULL, NULL },
  };

  D3DInclude include;
  include.acd = acd;

#if defined(LWE_DEBUG_BUILD) || defined(LWE_DEVELOPMENT_BUILD)
  static const UINT flags =
    D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY |
    D3DCOMPILE_OPTIMIZATION_LEVEL0 |
    D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#else
  static const UINT flags =
    D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY |
    D3DCOMPILE_OPTIMIZATION_LEVEL3 |
    D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

  ID3DBlob* blob = NULL;
  ID3DBlob* error_msg_blob = NULL;

  HRESULT hr = D3DCompile(
    src, src_len,
    acd->path,
    &defines[0],
    &include,
    "vs_main",
    "vs_4_0",
    flags, 0,
    &blob,
    &error_msg_blob
  );

  lwe_free(src);

  if (FAILED(hr)) {
    lwe_log(
      "  > Compiler Error (hr=%#08X):\n\n    %s\n",
      hr, error_msg_blob->GetBufferPointer()
    );

    error_msg_blob->Release();
    return FALSE;
  }

  lwe_vertex_shader_blob_t vs_blob;

  ID3D11ShaderReflection* shader_ref = NULL;

  hr = D3DReflect(
    blob->GetBufferPointer(), blob->GetBufferSize(),
    IID_ID3D11ShaderReflection, (void**)&shader_ref
  );

  if (FAILED(hr)) {
    lwe_log("  > Unable to D3DReflect, hr=%#08X\n", hr);
    blob->Release();
    return FALSE;
  }

  if (!_determine_vertex_decl(shader_ref, &vs_blob.vertex_decl)) {
    lwe_log("  > Unable to determine vertex declaration!\n");
    shader_ref->Release();
    blob->Release();
    return FALSE;
  }

  shader_ref->Release();

  vs_blob.byte_code_len = blob->GetBufferSize();

  static const lwe_size_t vs_blob_size =
    sizeof(lwe_vertex_shader_blob_t) - sizeof(uint8_t);

  if (fwrite((void*)&vs_blob, 1, vs_blob_size, acd->mrd) != vs_blob_size) {
    lwe_log("  > Unable to write memory-resident data!\n");
    blob->Release();
    return FALSE;
  }

  if (fwrite((void*)blob->GetBufferPointer(), 1, blob->GetBufferSize(), acd->mrd) != blob->GetBufferSize()) {
    lwe_log("  > Unable to write memory-resident data!\n");
    blob->Release();
    return FALSE;
  }

  blob->Release();
  return TRUE;
}

void lwe_vertex_shader_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_VERTEX_SHADER,
    "vs", "vs",
    &lwe_vertex_shader_load,
    &lwe_vertex_shader_unload,
    &lwe_vertex_shader_compile
  );
}