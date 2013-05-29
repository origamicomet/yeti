// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RENDER_TARGET_H_
#define _BUTANE_RENDER_TARGET_H_

#include <butane/butane.h>
#include <butane/graphics/texture.h>

namespace butane {
  class BUTANE_EXPORT RenderTarget abstract {
    __foundation_trait(RenderTarget, non_copyable);
  };
} // butane

#endif // _BUTANE_RENDER_TARGET_H_
