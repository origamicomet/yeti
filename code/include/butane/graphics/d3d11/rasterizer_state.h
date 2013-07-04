// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_RASTERIZER_STATE_H_
#define _BUTANE_D3D11_RASTERIZER_STATE_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/rasterizer_state.h>

namespace butane {
  class BUTANE_EXPORT D3D11RasterizerState final
    : public RasterizerState
  {
    __foundation_trait(D3D11RasterizerState, non_copyable);

    private:
      friend class RasterizerState;

    protected:
      D3D11RasterizerState();
      ~D3D11RasterizerState();

    public:
      void destroy() override;

    public:
      FOUNDATION_INLINE ID3D11RasterizerState* interface()
      { return _interface; }

    private:
      ID3D11RasterizerState* _interface;
  };
} // butane

#endif // _BUTANE_D3D11_RASTERIZER_STATE_H_
