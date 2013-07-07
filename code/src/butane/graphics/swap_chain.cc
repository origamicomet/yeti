// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/swap_chain.h>

#include <butane/application.h>
#include <butane/graphics/render_device.h>

namespace butane {
  SwapChain::SwapChain(
    Window* window,
    RenderTarget* render_target,
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height,
    bool fullscreen,
    bool vertical_sync
  ) : _window(window)
    , _render_target(nullptr)
    , _pixel_format(pixel_format)
    , _width(width)
    , _height(height)
    , _fullscreen(fullscreen)
    , _vertical_sync(vertical_sync)
  {
    _on_resized.handler = nullptr;
    _on_resized.closure = nullptr;

    Application::render_device()->on_swap_chain_created(this);
  }

  SwapChain::~SwapChain()
  {
    if (_render_target)
      _render_target->destroy();

    Application::render_device()->on_swap_chain_destroyed(this);
  }

  // void SwapChain::set_window(
  //   Window* window )
  // {}

  // void SwapChain::set_pixel_format(
  //   const PixelFormat pf )
  // {}

  void SwapChain::set_dimensions(
    const uint32_t width,
    const uint32_t height )
  {
    _width = width;
    _height = height;

    if (_on_resized.handler)
      _on_resized.handler(_on_resized.closure, this);

    Application::render_device()->on_swap_chain_resized(this);
  }

  void SwapChain::set_fullscreen(
    bool fullscreen )
  { _fullscreen = fullscreen; }

  void SwapChain::set_vertical_sync(
    bool vertical_sync )
  { _vertical_sync = vertical_sync; }
} // butane
