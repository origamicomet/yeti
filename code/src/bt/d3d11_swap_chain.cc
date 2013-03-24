// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/d3d11_swap_chain.h>
#include <bt/d3d11_render_device.h>

namespace bt {
  D3D11SwapChain::D3D11SwapChain()
    : SwapChain()
    , _interface(nullptr)
  {
  }
  
  D3D11SwapChain::~D3D11SwapChain()
  {
    if (_interface)
      _interface->Release();
  }

  SwapChain* SwapChain::create(
    Window* window,
    unsigned width,
    unsigned height,
    PixelFormat pixel_format,
    WindowMode mode,
    bool vsync )
  {
    assert(window != nullptr);
    assert(width > 0);
    assert(height > 0);
    assert((pixel_format & BT_PIXEL_FORMAT_COLOR_BIT));

    fail("SwapChain::create is not implemented, yet!\n");
    return nullptr;
  }

  void SwapChain::destroy()
  {
    MAKE_DELETE(D3D11SwapChain, Allocator::heap(), (D3D11SwapChain*)this);
  }

  void SwapChain::set_dimensions(
    unsigned width,
    unsigned height )
  {
    fail("SwapChain::set_dimensions is not implemented, yet!");
  }

  void SwapChain::set_window_mode(
    WindowMode mode )
  {
    fail("SwapChain::set_window_mode is not implemented, yet!");
  }

  void SwapChain::set_vertical_sync(
    bool vsync )
  {
    fail("SwapChain::set_vertical_sync is not implemented, yet!");
  }
} // bt