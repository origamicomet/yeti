// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_DEPTH_STENCIL_STATE_H_
#define _BUTANE_D3D11_DEPTH_STENCIL_STATE_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/depth_stencil_state.h>

namespace butane {
  class BUTANE_EXPORT D3D11DepthStencilState final
    : public DepthStencilState
  {
    __foundation_trait(D3D11DepthStencilState, non_copyable);

    private:
      friend class DepthStencilState;

    protected:
      D3D11DepthStencilState();
      ~D3D11DepthStencilState();

    public:
      void destory() override;

    public:
      FOUNDATION_INLINE ID3D11DepthStencilState* interface()
      { return _interface; }

    private:
      ID3D11DepthStencilState* _interface;
  };
} // butane

#endif // _BUTANE_D3D11_DEPTH_STENCIL_STATE_H_
