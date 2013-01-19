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

#include <lwe/assets/d3d11_texture.h>
#include <lwe/d3d11_pixel_format.h>
#include <lwe/d3d11_render_device.h>

static lwe_asset_t* lwe_d3d11_texture_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(stream != NULL);

  uint8_t* blob = (uint8_t*)lwe_asset_stream_mrd(stream);
  lwe_texture_blob_t* texture_blob = (lwe_texture_blob_t*)(blob + 0);
  void* data = (void*)(blob + sizeof(lwe_texture_blob_t));

  lwe_d3d11_texture_t* texture =
    (lwe_d3d11_texture_t*)lwe_alloc(sizeof(lwe_d3d11_texture_t));

  texture->type = texture_blob->type;
  texture->pixel_format = texture_blob->pixel_format;
  texture->width = texture_blob->width;
  texture->height = texture_blob->height;
  texture->depth = texture_blob->depth;
  texture->flags = texture_blob->flags;

  HRESULT hr;

  const D3D11_SUBRESOURCE_DATA sbrd = {
    data,

    lwe_pixel_format_row_stride(
      texture->pixel_format, texture->width),

    lwe_pixel_format_layer_stride(
      texture->pixel_format, texture->width, texture->height)
  };

  D3D11_SHADER_RESOURCE_VIEW_DESC srvd;

  switch (texture->type) {
    case LWE_TEXTURE_TYPE_1D:
    case LWE_TEXTURE_TYPE_1D_ARRAY: {
      lwe_fail_if(
        texture->height != 1,
        "Unexpected texture height, height=%u",
        texture->height
      );

      D3D11_TEXTURE1D_DESC td;
      td.Width          = texture->width;
      td.MipLevels      = 0;
      td.ArraySize      = texture->depth;
      td.Format         = lwe_pixel_format_to_dxgi(texture->pixel_format);
      td.Usage          = D3D11_USAGE_IMMUTABLE;
      td.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
      td.CPUAccessFlags = 0;
      td.MiscFlags      = 0;

      lwe_fail_if(FAILED(
        hr = _d3d11_device->CreateTexture1D(
          &td,
          &sbrd,
          (ID3D11Texture1D**)(&texture->resource)
        )),

        "ID3D11Device::CreateTexture1D failed, hr=%#08X", hr
      );

      srvd.Format = td.Format;

      srvd.ViewDimension = (texture->type == LWE_TEXTURE_TYPE_1D) ?
        D3D11_SRV_DIMENSION_TEXTURE1D : D3D11_SRV_DIMENSION_TEXTURE1DARRAY;

      srvd.Texture1DArray.MostDetailedMip = 0;
      srvd.Texture1DArray.MipLevels = -1;
      srvd.Texture1DArray.FirstArraySlice = 0;
      srvd.Texture1DArray.ArraySize = texture->depth;
    } break;

    case LWE_TEXTURE_TYPE_2D:
    case LWE_TEXTURE_TYPE_2D_ARRAY: {
      D3D11_TEXTURE2D_DESC td;
      td.Width          = texture->width;
      td.Height         = texture->height;
      td.MipLevels      = 0;
      td.ArraySize      = texture->depth;
      td.Format         = lwe_pixel_format_to_dxgi(texture->pixel_format);
      td.Usage          = D3D11_USAGE_IMMUTABLE;
      td.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
      td.CPUAccessFlags = 0;
      td.MiscFlags      = 0;

      lwe_fail_if(FAILED(
        hr = _d3d11_device->CreateTexture2D(
          &td,
          &sbrd,
          (ID3D11Texture2D**)(&texture->resource)
        )),

        "ID3D11Device::CreateTexture2D failed, hr=%#08X", hr
      );

      srvd.Format = td.Format;

      srvd.ViewDimension = (texture->type == LWE_TEXTURE_TYPE_2D) ?
        D3D11_SRV_DIMENSION_TEXTURE2D : D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

      srvd.Texture2DArray.MostDetailedMip = 0;
      srvd.Texture2DArray.MipLevels = -1;
      srvd.Texture2DArray.FirstArraySlice = 0;
    } break;

    case LWE_TEXTURE_TYPE_3D: {
      D3D11_TEXTURE3D_DESC td;
      td.Width          = texture->width;
      td.Height         = texture->height;
      td.Depth          = texture->depth;
      td.MipLevels      = 0;
      td.Format         = lwe_pixel_format_to_dxgi(texture->pixel_format);
      td.Usage          = D3D11_USAGE_IMMUTABLE;
      td.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
      td.CPUAccessFlags = 0;
      td.MiscFlags      = 0;

      lwe_fail_if(FAILED(
        hr = _d3d11_device->CreateTexture3D(
          &td,
          &sbrd,
          (ID3D11Texture3D**)(&texture->resource)
        )),

        "ID3D11Device::CreateTexture3D failed, hr=%#08X", hr
      );

      srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
      srvd.Texture3D.MostDetailedMip = 0;
      srvd.Texture3D.MipLevels = -1;
    } break;

    case LWE_TEXTURE_TYPE_CUBE_MAP: {
      lwe_fail_if(
        texture->depth != 6,
        "Unexpected texture depth, depth=%u",
        texture->depth
      );

      D3D11_TEXTURE2D_DESC td;
      td.Width          = texture->width;
      td.Height         = texture->height;
      td.MipLevels      = 0;
      td.ArraySize      = texture->depth;
      td.Format         = lwe_pixel_format_to_dxgi(texture->pixel_format);
      td.Usage          = D3D11_USAGE_IMMUTABLE;
      td.BindFlags      = D3D11_BIND_SHADER_RESOURCE;
      td.CPUAccessFlags = 0;
      td.MiscFlags      = 0;

      lwe_fail_if(FAILED(
        hr = _d3d11_device->CreateTexture2D(
          &td,
          &sbrd,
          (ID3D11Texture2D**)(&texture->resource)
        )),

        "ID3D11Device::CreateTexture2D failed, hr=%#08X", hr
      );

      srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
      srvd.TextureCube.MostDetailedMip = 0;
      srvd.TextureCube.MipLevels = -1;
    } break;
  }

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateShaderResourceView(
      texture->resource,
      &srvd,
      &texture->srv
    )),

    "ID3D11Device::CreateShaderResourceView failed, hr=%#08X", hr
  );

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)texture;
}

static void lwe_d3d11_texture_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_TEXTURE);

  lwe_d3d11_texture_t* texture = (lwe_d3d11_texture_t*)asset;
  texture->resource->Release();
  texture->srv->Release();
  lwe_free((void*)texture);
}

// See src/lwe/assets/texture.cc
extern bool lwe_texture_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd );

void lwe_texture_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_TEXTURE,
    "texture", "dds",
    &lwe_d3d11_texture_load,
    &lwe_d3d11_texture_unload,
    &lwe_texture_compile
  );
}