// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/depth_stencil_target.h>

#include <butane/application.h>
#include <butane/graphics/d3d11/render_device.h>
#include <butane/graphics/d3d11/texture.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("depth-stencil targets", Allocators::heap());
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  D3D11DepthStencilTarget::D3D11DepthStencilTarget(
    Texture* texture
  ) : DepthStencilTarget(texture)
  {
  }

  D3D11DepthStencilTarget::~D3D11DepthStencilTarget()
  {
    if (_view)
      _view->Release();
    _view = nullptr;
  }

  DepthStencilTarget* DepthStencilTarget::create(
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height )
  {
    assert(width > 0);
    assert(height > 0);

    if (!(pixel_format & (PixelFormat::DepthBit | PixelFormat::StencilBit)))
      fail("Invalid pixel format, expected a depth and/or stencil format.");

    D3D11Texture* texture = ((D3D11Texture*)Texture::create(
      Texture::TEXTURE_2D, pixel_format, width, height, 1, Texture::DEPTH_STENCIL_TARGETABLE));

    D3D11DepthStencilTarget* depth_stencil_target =
      make_new(D3D11DepthStencilTarget, allocator())(texture);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    dsvd.Format             = dxgi_format_from_pixel_format(pixel_format);
    dsvd.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvd.Flags = 0;
    dsvd.Texture2D.MipSlice = 0;

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* depth_stencil_target->_view = */ {
      const HRESULT hr = render_device->device()->CreateDepthStencilView(
        texture->resource(), &dsvd, &depth_stencil_target->_view);
      if (FAILED(hr))
        fail("ID3D11Device::CreateDepthStencilView failed, hr=%#08x", hr);
    }

    return depth_stencil_target;
  }

  DepthStencilTarget* DepthStencilTarget::create(
    Texture* texture )
  {
    assert(texture != nullptr);

    if (texture->type() != Texture::TEXTURE_2D)
      fail("Invalid texture (invalid type, expected a 2D texture).");

    if (!(texture->pixel_format() & (PixelFormat::DepthBit | PixelFormat::StencilBit)))
      fail("Invalid texture (invalid pixel format, expected a depth and/or stencil format).");

    D3D11DepthStencilTarget* depth_stencil_target =
      make_new(D3D11DepthStencilTarget, allocator())(texture);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    dsvd.Format             = dxgi_format_from_pixel_format(texture->pixel_format());
    dsvd.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvd.Flags              = 0;
    dsvd.Texture2D.MipSlice = 0;

    D3D11Texture* texture_ = (D3D11Texture*)texture;

    D3D11RenderDevice* render_device =
      ((D3D11RenderDevice*)Application::render_device());

    /* depth_stencil_target->_view = */ {
      const HRESULT hr = render_device->device()->CreateDepthStencilView(
        texture_->resource(), &dsvd, &depth_stencil_target->_view);
      if (FAILED(hr))
        fail("ID3D11Device::CreateDepthStencilView failed, hr=%#08x", hr);
    }

    return depth_stencil_target;
  }

  void D3D11DepthStencilTarget::destroy()
  {
    make_delete(D3D11DepthStencilTarget, allocator(), this);
  }
} // butane
