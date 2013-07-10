// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TEXTURE_H_
#define _BUTANE_TEXTURE_H_

#include <butane/butane.h>
#include <butane/graphics/pixel_format.h>

namespace butane {
  class BUTANE_EXPORT Texture abstract {
    __foundation_trait(Texture, non_copyable);

    public:
      enum Type {
        TEXTURE_1D = 1,
        TEXTURE_2D = 2,
        TEXTURE_3D = 3,
        CUBE_MAP   = 4
      };

      enum Flags {
        /*! The specified data includes mip maps. */
        HAS_MIP_MAPS             = (1u << 0u),
        /*! Allows a texture to be rendered through a butane::RenderTarget. */
        RENDER_TARGETABLE        = (1u << 1u),
        /*! Allows a texture to be rendered through a butane::DepthStencilTarget. */
        DEPTH_STENCIL_TARGETABLE = (1u << 2u)
      };

    protected:
      Texture(
        const Type type,
        const PixelFormat pixel_format,
        const uint32_t width,
        const uint32_t height,
        const uint32_t depth );

      virtual ~Texture();

    public:
      static Texture* create(
        const Type type,
        const PixelFormat pixel_format,
        const uint32_t width,
        const uint32_t height,
        const uint32_t depth,
        const uint32_t flags,
        const void* data = nullptr );

      virtual Texture* alias(
        const uint32_t width,
        const uint32_t height,
        const uint32_t depth,
        const uint32_t flags ) = 0;

      virtual void destroy() = 0;

    public:
      FOUNDATION_INLINE Type type() const
      { return _type; }

      FOUNDATION_INLINE PixelFormat pixel_format() const
      { return _pixel_format; }

      FOUNDATION_INLINE uint32_t width() const
      { return _width; }

      FOUNDATION_INLINE uint32_t height() const
      { return _height; }

      FOUNDATION_INLINE uint32_t depth() const
      { return _depth; }

    private:
      Type _type;
      PixelFormat _pixel_format;
      uint32_t _width;
      uint32_t _height;
      uint32_t _depth;
  };
} // butane

#endif // _BUTANE_TEXTURE_H_
