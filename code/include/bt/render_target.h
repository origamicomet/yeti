// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RENDER_TARGET_H_
#define _BUTANE_RENDER_TARGET_H_

#include <bt/foundation.h>
#include <bt/config.h>

#include <bt/texture.h>

namespace bt {
  class RenderTarget {
    protected:
      RenderTarget();
      ~RenderTarget();

    public:
      static RenderTarget* create(
        PixelFormat format,
        unsigned width,
        unsigned height );

      void destroy();

      void resize(
        unsigned width,
        unsigned height );

    public:
      FOUNDATION_INLINE PixelFormat format() const
      { return _texture->format(); }

      FOUNDATION_INLINE void dimensions( 
        unsigned& width,
        unsigned& height ) const
      {
        _texture->dimensions(width, height);
      }

    protected:
      Texture* _texture;
  };
} // bt

#endif // _BUTANE_RENDER_TARGET_H_