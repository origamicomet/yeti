// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_DEPTH_STENCIL_TARGET_H_
#define _BUTANE_DEPTH_STENCIL_TARGET_H_

#include <butane/butane.h>
#include <butane/graphics/texture.h>

namespace butane {
  class BUTANE_EXPORT DepthStencilTarget abstract {
    __foundation_trait(DepthStencilTarget, non_copyable);

    protected:
      DepthStencilTarget(
        Texture* texture );

      virtual ~DepthStencilTarget();

    public:
      static DepthStencilTarget* create(
        const PixelFormat pixel_format,
        const uint32_t width,
        const uint32_t height );

      virtual void destroy() = 0;

    public:
      FOUNDATION_INLINE Texture* texture() const
      { return _texture; }

    private:
      Texture* _texture;
  };
} // butane

#endif // _BUTANE_DEPTH_STENCIL_TARGET_H_
