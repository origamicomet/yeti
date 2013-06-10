// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_SAMPLER_H_
#define _BUTANE_D3D11_SAMPLER_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/sampler.h>

namespace butane {
  class BUTANE_EXPORT D3D11Sampler final
    : public Sampler
  {
    __foundation_trait(D3D11Sampler, non_copyable);

    private:
      friend class Sampler;

    protected:
      D3D11Sampler();
      ~D3D11Sampler();

    public:
      void destory() override;

    public:
      FOUNDATION_INLINE ID3D11SamplerState* interface()
      { return _interface; }

    private:
      ID3D11SamplerState* _interface;
  };
} // butane

#endif // _BUTANE_D3D11_SAMPLER_H_
