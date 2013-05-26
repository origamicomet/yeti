// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TEXTURE_H_
#define _BUTANE_TEXTURE_H_

#include <butane/foundation.h>
#include <butane/config.h>
#include <butane/pixel_format.h>

namespace butane {
  class BUTANE_EXPORT Texture abstract {
    __foundation_trait(Texture, non_copyable);

    public:
      enum Type {
        TEXTURE_1D       = 1,
        TEXTURE_1D_ARRAY = 2,
        TEXTURE_2D       = 3,
        TEXTURE_2D_ARRAY = 4,
        TEXTURE_3D       = 5,
        CUBE_MAP         = 6
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
        bool streamable = false );

      virtual void destroy() = 0;

    protected:
      const Type _type;
      const PixelFormat _pixel_format;
      const uint32_t _width;
      const uint32_t _height;
      const uint32_t _depth;
  };
} // butane

#endif // _BUTANE_TEXTURE_H_
