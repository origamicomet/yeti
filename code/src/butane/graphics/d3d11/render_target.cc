// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/render_target.h>

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
  }

  RenderTarget* RenderTarget::create(
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height )
  {
    assert(width > 0);
    assert(height > 0);

    // D3D11Texture* texture =
    //   make_new(D3D11Texture, allocator())();

    // D3D11RenderTarget* render_target =
    //   make_new(D3D11RenderTarget, allocator())();

    return nullptr;
  }

  void D3D11RenderTarget::destroy()
  {
    make_delete(D3D11RenderTarget, allocator(), this);
  }
} // butane
