// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_RENDER_TARGET_H_
#define _BUTANE_D3D11_RENDER_TARGET_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/render_target.h>

namespace butane {
  class SwapChain;
  class BUTANE_EXPORT D3D11RenderTarget final
    : public RenderTarget
  {
    __foundation_trait(D3D11RenderTarget, non_copyable);

    private:
      friend class RenderTarget;
      friend class SwapChain;
      friend class D3D11SwapChain;

    private:
      D3D11RenderTarget(
        Texture* texture );

      ~D3D11RenderTarget();

    public:
      void destroy() override;

    public:
      FOUNDATION_INLINE ID3D11RenderTargetView* view() const
      { return _view; }

    private:
      ID3D11RenderTargetView* _view;
  };
} // butane

#endif // _BUTANE_D3D11_RENDER_TARGET_H_
