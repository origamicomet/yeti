// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_SWAP_CHAIN_H_
#define _BUTANE_D3D11_SWAP_CHAIN_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/swap_chain.h>

namespace butane {
  class BUTANE_EXPORT D3D11SwapChain final
    : public SwapChain
  {
    __foundation_trait(D3D11SwapChain, non_copyable);

    private:
      friend class SwapChain;

    private:
      D3D11SwapChain(
        Window* window,
        const PixelFormat pixel_format,
        const uint32_t width,
        const uint32_t height,
        bool fullscreen,
        bool vertical_sync );

      ~D3D11SwapChain();

    public:
      void destroy() override;

    public:
      // void set_window(
      //   Window* window ) override;

      // void set_pixel_format(
      //   const PixelFormat pf ) override;

      void set_dimensions(
        const uint32_t width,
        const uint32_t height ) override;

      void set_fullscreen(
        bool fullscreen ) override;

      void set_vertical_sync(
        bool vertical_sync ) override;

    private:
      static void on_window_resized(
        D3D11SwapChain* swap_chain,
        Window* window );

      static void on_window_windowed(
        D3D11SwapChain* swap_chain,
        Window* window );

      static void on_window_fullscreen(
        D3D11SwapChain* swap_chain,
        Window* window );

    public:
      FOUNDATION_INLINE bool interface() const
      { return _interface; }

    private:
      IDXGISwapChain* _interface;
  };
} // butane

#endif // _BUTANE_D3D11_SWAP_CHAIN_H_
