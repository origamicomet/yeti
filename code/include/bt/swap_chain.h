// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SWAP_CHAIN_H_
#define _BUTANE_SWAP_CHAIN_H_

#include <bt/foundation.h>
#include <bt/config.h>

#include <bt/window.h>
#include <bt/render_target.h>
#include <bt/pixel_format.h>

namespace bt {
  class SwapChain {
    FOUNDATION_NON_COPYABLE(SwapChain);

    protected:
      SwapChain();
      ~SwapChain();

    public:
      static SwapChain* create(
        Window* window,
        unsigned width,
        unsigned height,
        PixelFormat pixel_format,
        WindowMode mode,
        bool vsync );

      void destroy();

      void resize(
        unsigned width,
        unsigned height );

      void set_window_mode(
        WindowMode mode );

      void set_vertical_sync(
        bool vsync );

    public:
      FOUNDATION_INLINE Window* window() const
      { return _window; }

      FOUNDATION_INLINE RenderTarget* render_target() const
      { return _render_target; }

    protected:
      Winodw* _window;
      RenderTarget* _render_target;
  };
} // bt

#endif // _BUTANE_SWAP_CHAIN_H_