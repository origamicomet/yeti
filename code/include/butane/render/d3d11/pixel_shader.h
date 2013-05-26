// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_PIXEL_SHADER_H_
#define _BUTANE_D3D11_PIXEL_SHADER_H_

#include <butane/resource/pixel_shader.h>

#include <Windows.h>
#include <DXGI.h>
#include <D3D11.h>

namespace butane {
  class BUTANE_EXPORT D3D11PixelShader final
    : public butane::PixelShader
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
