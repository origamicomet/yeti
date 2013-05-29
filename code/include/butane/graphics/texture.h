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

    public:
      virtual void destroy() = 0;
  };
} // butane

#endif // _BUTANE_TEXTURE_H_
