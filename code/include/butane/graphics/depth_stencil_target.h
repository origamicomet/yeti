// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_DEPTH_STENCIL_TARGET_H_
#define _BUTANE_DEPTH_STENCIL_TARGET_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT DepthStencilTarget abstract {
    __foundation_trait(DepthStencilTarget, non_copyable);
  };
} // butane

#endif // _BUTANE_DEPTH_STENCIL_TARGET_H_
