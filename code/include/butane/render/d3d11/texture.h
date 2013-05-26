// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_TEXTURE_H_
#define _BUTANE_D3D11_TEXTURE_H_

#include <butane/render/texture.h>

#include <Windows.h>
#include <DXGI.h>
#include <D3D11.h>

namespace butane {
  class BUTANE_EXPORT D3D11Texture final
    : public butane::Texture
  {
    __foundation_trait(D3D11Texture, non_copyable);

    private:
      friend class Texture;

    private:
      D3D11Texture(
        const Type type,
        const PixelFormat pixel_format,
        const uint32_t width,
        const uint32_t height,
        const uint32_t depth );

      ~D3D11Texture();

    public:
      void destroy();

    public:
      FOUNDATION_INLINE ID3D11Resource* resource() const
      { return _resource; }

    private:
      ID3D11Resource* _resource;
  };
} // butane

#endif // _BUTANE_D3D11_TEXTURE_H_
