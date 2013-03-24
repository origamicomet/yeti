// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/d3d11.h>
#include <bt/render_target.h>

namespace bt {
  D3D11RenderTarget::D3D11RenderTarget()
    : RenderTarget()
    , _rtv(nullptr)
  {
  }

  D3D11RenderTarget::~D3D11RenderTarget()
  {
    if (_rtv)
      _rtv->Release();
  }

  RenderTarget* RenderTarget::create(
    PixelFormat format,
    unsigned width,
    unsigned height )
  {
    assert((format & BT_PIXEL_FORMAT_COLOR_BIT));
    assert(width > 0);
    assert(height > 0);

    fail("RenderTarget::create is not implemented, yet!\n");
    return nullptr;
  }

  void RenderTarget::destroy()
  {
    MAKE_DELETE(D3D11RenderTarget, Allocator::heap(), (D3D11RenderTarget*)this);
  }

  void RenderTarget::resize(
    unsigned width,
    unsigned height )
  {
    assert(width > 0);
    assert(height > 0);

    fail("RenderTarget::resize is not implemented, yet!\n");
  }
} // bt