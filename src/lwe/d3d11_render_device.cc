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