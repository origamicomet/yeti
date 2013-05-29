// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_PIXEL_SHADER_H_
#define _BUTANE_D3D11_PIXEL_SHADER_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/pixel_shader.h>

namespace butane {
  class BUTANE_EXPORT D3D11PixelShader final
    : public PixelShader
  {
    __foundation_trait(D3D11PixelShader, non_copyable);

    private:
      friend class PixelShader;

    private:
      D3D11PixelShader(
        const Resource::Id id );

      ~D3D11PixelShader();

    public:
      FOUNDATION_INLINE ID3D11PixelShader* resource() const
      { return _resource; }

    private:
      ID3D11PixelShader* _resource;
  };
} // butane

#endif // _BUTANE_D3D11_PIXEL_SHADER_H_
