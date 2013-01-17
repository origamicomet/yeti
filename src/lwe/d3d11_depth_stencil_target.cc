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

#include <lwe/d3d11_depth_stencil_target.h>
#include <lwe/d3d11_pixel_format.h>
#include <lwe/d3d11_render_device.h>

lwe_depth_stencil_target_t* lwe_depth_stencil_target_create(
  lwe_pixel_format_t pixel_format,
  uint32_t width,
  uint32_t height )
{
  lwe_assert(
    lwe_pixel_format_is_depth(pixel_format) ||
    lwe_pixel_format_is_stencil(pixel_format)
  );

  lwe_assert(width > 0);
  lwe_assert(height > 0);

  lwe_d3d11_depth_stencil_target_t* depth_stencil_target =
    (lwe_d3d11_depth_stencil_target_t*)lwe_alloc(sizeof(lwe_d3d11_depth_stencil_target_t));

  depth_stencil_target->pixel_format = pixel_format;
  depth_stencil_target->width = width;
  depth_stencil_target->height = height;

  D3D11_TEXTURE2D_DESC td;
  td.Width          = width;
  td.Height         = height;
  td.MipLevels      = 0;
  td.ArraySize      = 1;
  td.Format         = lwe_pixel_format_to_dxgi(pixel_format);
  td.Usage          = D3D11_USAGE_DEFAULT;
  td.BindFlags      = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  td.MiscFlags      = 0;

  HRESULT hr;

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateTexture2D(
      &td, NULL, &depth_stencil_target->texture
    )),

    "ID3D11Device::CreateTexture2D failed, hr=%#08X", hr
  );

  D3D11_DEPTH_STENCIL_VIEW_DESC dstvd;
  dstvd.Format = td.Format;
  dstvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dstvd.Flags = 0;
  dstvd.Texture2D.MipSlice = 0;

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateDepthStencilView(
      depth_stencil_target->texture, &dstvd, &depth_stencil_target->dstv
    )),

    "ID3D11Device::CreateDepthStencilView, hr=%#08X", hr
  );

  D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
  srvd.Format = td.Format;
  srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvd.Texture2D.MostDetailedMip = 0;
  srvd.Texture2D.MipLevels = -1;

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateShaderResourceView(
      depth_stencil_target->texture, &srvd, &depth_stencil_target->srv
    )),

    "ID3D11Device::CreateShaderResourceView, hr=%#08X", hr
  );

  return (lwe_depth_stencil_target_t*)depth_stencil_target;
}

void lwe_depth_stencil_target_destroy(
  lwe_depth_stencil_target_t* depth_stencil_target_ )
{
  lwe_assert(depth_stencil_target_ != NULL);

  lwe_d3d11_depth_stencil_target_t* depth_stencil_target =
    (lwe_d3d11_depth_stencil_target_t*)depth_stencil_target_;

  depth_stencil_target->texture->Release();
  depth_stencil_target->dstv->Release();
  depth_stencil_target->srv->Release();

  lwe_free((void*)depth_stencil_target);
}
