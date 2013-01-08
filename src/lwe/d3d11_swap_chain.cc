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

#include <lwe/d3d11_swap_chain.h>
#include <lwe/d3d11_pixel_format.h>
#include <lwe/d3d11_render_device.h>

lwe_swap_chain_t* lwe_swap_chain_create(
  lwe_window_t* window,
  uint32_t width,
  uint32_t height,
  lwe_pixel_format_t pixel_format,
  bool fullscreen,
  bool vsync )
{
  lwe_assert(window != NULL);
  lwe_assert(lwe_pixel_format_is_color(pixel_format) != FALSE);

  lwe_d3d11_swap_chain_t* swap_chain =
    (lwe_d3d11_swap_chain_t*)lwe_alloc(sizeof(lwe_d3d11_swap_chain_t));

  swap_chain->window = window;
  swap_chain->width = width;
  swap_chain->height = height;
  swap_chain->pixel_format = pixel_format;
  swap_chain->fullscreen = fullscreen;
  swap_chain->vsync = vsync;

  DXGI_SWAP_CHAIN_DESC scd;
  scd.BufferDesc.Width = width;
  scd.BufferDesc.Height = height;
  scd.BufferDesc.RefreshRate.Numerator = vsync ? 1 : 0;
  scd.BufferDesc.RefreshRate.Denominator = 1;
  scd.BufferDesc.Format = lwe_pixel_format_to_dxgi(pixel_format);
  scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  scd.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
  scd.SampleDesc.Count = 1;
  scd.SampleDesc.Quality = 0;
  scd.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.BufferCount = 2;
  scd.OutputWindow = ((HWND)window->sys_handle);
  scd.Windowed = TRUE;
  scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

  HRESULT hr;

  lwe_fail_if(FAILED(
    hr = _dxgi_factory->CreateSwapChain(
      _d3d11_device,
      &scd,
      &swap_chain->swap_chain
    )),

    "IDXGIFactory::CreateSwapChain failed, hr=%#08X", hr
  );

  ID3D11Texture2D* color_buffer = NULL;

  lwe_fail_if(FAILED(
    hr = swap_chain->swap_chain->GetBuffer(
      0,
      __uuidof(ID3D11Texture2D),
      (void**)&color_buffer
    )),

    "IDXGISwapChain::GetBuffer failed, hr=%#08X", hr
  );

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateRenderTargetView(
      color_buffer,
      NULL,
      &swap_chain->rtv
    )),

    "ID3D11Device::CreateRenderTargetView failed, hr=%#08X", hr
  );

  color_buffer->Release();

  // TODO: Switch to fullscreen if fullscreen == true.

  return (lwe_swap_chain_t*)swap_chain;
}

void lwe_swap_chain_resize(
  lwe_swap_chain_t* swap_chain_,
  uint32_t new_width,
  uint32_t new_height )
{
  lwe_assert(swap_chain_ != NULL);
  lwe_fail("Currently not implemented.");
}

void lwe_swap_chain_toggle_fullscreen(
  lwe_swap_chain_t* swap_chain_,
  bool fullscreen )
{
  lwe_assert(swap_chain_ != NULL);
  lwe_fail("Currently not implemented.");
}

void lwe_swap_chain_toggle_vsync(
  lwe_swap_chain_t* swap_chain_,
  bool vsync )
{
  lwe_assert(swap_chain_ != NULL);
  lwe_fail("Currently not implemented.");
}

void lwe_swap_chain_present(
  lwe_swap_chain_t* swap_chain_ )
{
  lwe_assert(swap_chain_ != NULL);

  lwe_d3d11_swap_chain_t* swap_chain = (lwe_d3d11_swap_chain_t*)swap_chain_;

  swap_chain->swap_chain->Present(0, 0);
}

void lwe_swap_chain_destroy(
  lwe_swap_chain_t* swap_chain_ )
{
  lwe_assert(swap_chain_ != NULL);

  lwe_d3d11_swap_chain_t* swap_chain = (lwe_d3d11_swap_chain_t*)swap_chain_;

  swap_chain->rtv->Release();
  swap_chain->swap_chain->Release();

  lwe_free((void*)swap_chain);
}