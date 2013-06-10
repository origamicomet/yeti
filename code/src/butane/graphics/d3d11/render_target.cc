// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/render_target.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>
#include <butane/graphics/d3d11/texture.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("render targets", Allocators::heap());
    return allocator;
  }

  D3D11RenderTarget::D3D11RenderTarget(
    Texture* texture
  ) : RenderTarget(texture)
  {
  }

  D3D11RenderTarget::~D3D11RenderTarget()
  {
    if (_view)
      _view->Release();
    _view = nullptr;
  }

  RenderTarget* RenderTarget::create(
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height )
  {
    assert(width > 0);
    assert(height > 0);

    if (!(pixel_format & PixelFormat::ColorBit))
      fail("Invalid pixel format, expected a color format.");

    D3D11Texture* texture = ((D3D11Texture*)Texture::create(
      Texture::TEXTURE_2D, pixel_format, width, height, 1, Texture::RENDER_TARGETABLE));

    D3D11RenderTarget* render_target =
      make_new(D3D11RenderTarget, allocator())(texture);

    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    rtvd.Format             = dxgi_format_from_pixel_format(pixel_format);
    rtvd.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvd.Texture2D.MipSlice = 0;

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* render_target->_view = */ {
      const HRESULT hr = render_device->device()->CreateRenderTargetView(
        texture->resource(), &rtvd, &render_target->_view);
      if (FAILED(hr))
        fail("ID3D11Device::CreateRenderTargetView failed, hr=%#08x", hr);
    }

    return render_target;
  }

  void D3D11RenderTarget::destroy()
  {
    make_delete(D3D11RenderTarget, allocator(), this);
  }
} // butane
