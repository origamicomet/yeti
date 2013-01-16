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
#include <lwe/d3d_shader_compiler.h>

typedef struct lwe_pixel_shader_blob_t {
  lwe_size_t byte_code_len;
  uint8_t byte_code[1];
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

  lwe_d3d11_pixel_shader_t* pixel_shader =
    (lwe_d3d11_pixel_shader_t*)lwe_alloc(sizeof(lwe_d3d11_pixel_shader_t));

  HRESULT hr;
  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreatePixelShader(
      (const void*)&ps_blob->byte_code[0],
      ps_blob->byte_code_len,
      NULL,
      &pixel_shader->ps
    )),

    "ID3D11Device::CreatePixelShader failed, hr=%#08X",
    hr
  );

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)pixel_shader;
}

static void lwe_pixel_shader_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_PIXEL_SHADER);

  lwe_d3d11_pixel_shader_t* pixel_shader =
	(lwe_d3d11_pixel_shader_t*)asset;

  pixel_shader->ps->Release();

  lwe_free((void*)pixel_shader);
}

static bool lwe_pixel_shader_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_PIXEL_SHADER);
  lwe_assert(acd != NULL);

  const lwe_size_t src_len = lwe_file_size(acd->in);
  void* src = lwe_alloc(src_len);

  if (fread(src, 1, src_len, acd->in) != src_len) {
    lwe_log("  > Unexpected end-of-file!\n");
    lwe_free(src);
    return FALSE;
  }

  const D3D_SHADER_MACRO defines[] = {
    { "PIXEL_SHADER", "1" },
    { "D3D11",        "1" },
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
    "ps_main",
    "ps_4_0",
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

  lwe_pixel_shader_blob_t ps_blob;
  ps_blob.byte_code_len = blob->GetBufferSize();

  static const lwe_size_t ps_blob_size =
    sizeof(lwe_pixel_shader_blob_t) - sizeof(uint8_t);

  if (fwrite((void*)&ps_blob, 1, ps_blob_size, acd->mrd) != ps_blob_size) {
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

void lwe_pixel_shader_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_PIXEL_SHADER,
    "ps",
    &lwe_pixel_shader_load,
    &lwe_pixel_shader_unload,
    &lwe_pixel_shader_compile
  );
}