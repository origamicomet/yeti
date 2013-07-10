// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_D3D11_TEXTURE_H_
#define _BUTANE_D3D11_TEXTURE_H_

#include <butane/graphics/d3d11/d3d11.h>
#include <butane/graphics/texture.h>

namespace butane {
  class BUTANE_EXPORT D3D11Texture final
    : public Texture
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
      Texture* alias(
        const uint32_t width,
        const uint32_t height,
        const uint32_t depth,
        const uint32_t flags ) override;

      void destroy() override;

    public:
      FOUNDATION_INLINE ID3D11Resource* resource() const
      { return _resource; }

      FOUNDATION_INLINE ID3D11ShaderResourceView* srv() const
      { return _srv; }

    private:
      ID3D11Resource* _resource;
      ID3D11ShaderResourceView* _srv;
  };
} // butane

#endif // _BUTANE_D3D11_TEXTURE_H_
