// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_TEXTURE_H_
#define _BUTANE_TEXTURE_H_

#include <bt/foundation.h>
#include <bt/config.h>

#include <bt/resource.h>

namespace bt {
  enum TextureType {
    BT_TEXTURE_1D       = 1,
    BT_TEXTURE_1D_ARRAY = 2,
    BT_TEXTURE_2D       = 3,
    BT_TEXTURE_2D_ARRAY = 4,
    BT_TEXTURE_3D       = 5,
    BT_TEXTURE_CUBE_MAP = 6
  };

  class Texture
    : public Resource
  {
    protected:
      Texture( Resource::Type& type, Resource::Id id );
      virtual Texture();

    public:
      static Texture* create(
        TextureType type,
        PixelFormat format,
        unsigned width,
        unsigned height,
        unsigned depth,
        unsigned flags,
        void* data = 0 );

      void destroy();

    public:
      static const Type& register();

    private:
      static Resource* load( Resource::Stream* stream );
      static Resource* reload( Resource* resource );
      static void unload( Resource* resource );
      static bool compile( Resource::CompileData& compile_data );

    public:
      FOUNDATION_INLINE PixelFormat format() const
      { return _format; }

      FOUNDATION_INLINE void dimensions(
        unsigned& width,
        unsigned& height,
        unsigned& depth ) const
      {
        width = _width;
        height = _height;
        depth = _depth;
      }

    protected:
      PixelFormat _format;
      unsigned width;
      unsigned height;
      unsigned depth;
  };
} // bt

#endif // _BUTANE_TEXTURE_H_