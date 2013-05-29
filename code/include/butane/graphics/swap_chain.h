// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_SWAP_CHAIN_H_
#define _BUTANE_SWAP_CHAIN_H_

#include <butane/butane.h>

#include <butane/window.h>
#include <butane/graphics/pixel_format.h>

namespace butane {
  class BUTANE_EXPORT SwapChain abstract {
    __foundation_trait(SwapChain, non_copyable);

    public:
      typedef void (*OnResizedHandler)(
        void* closure,
        const SwapChain* swap_chain );

    protected:
      SwapChain(
        Window* window,
        const PixelFormat pixel_format,
        const uint32_t width,
        const uint32_t height,
        bool fullscreen,
        bool vertical_sync );

      virtual ~SwapChain();

    public:
      static SwapChain* create(
        Window* window,
        const PixelFormat pixel_format,
        const uint32_t width,
        const uint32_t height,
        bool fullscreen,
        bool vertical_sync );

      virtual void destroy() = 0;

    public:
      FOUNDATION_INLINE Window* window() const
      { return _window; }

      // virtual void set_window(
      //   Window* window );

      FOUNDATION_INLINE PixelFormat pixel_format() const
      { return _pixel_format; }

      // virtual void set_pixel_format(
      //   const PixelFormat pf );

      FOUNDATION_INLINE uint32_t width() const
      { return _width; }

      FOUNDATION_INLINE uint32_t height() const
      { return _height; }

      virtual void set_dimensions(
        const uint32_t width,
        const uint32_t height );

      FOUNDATION_INLINE bool fullscreen() const
      { return _fullscreen; }

      virtual void set_fullscreen(
        bool fullscreen );

      FOUNDATION_INLINE bool vertical_sync() const
      { return _vertical_sync; }

      virtual void set_vertical_sync(
        bool vertical_sync );

    public:
      void set_on_resized_handler(
        OnResizedHandler handler,
        void* closure = nullptr );

    private:
      Window* _window;
      PixelFormat _pixel_format;
      uint32_t _width;
      uint32_t _height;
      bool _fullscreen;
      bool _vertical_sync;

    protected:
      struct {
        OnResizedHandler handler;
        void* closure;
      } _on_resized;
  };
} // butane

#endif // _BUTANE_SWAP_CHAIN_H_
