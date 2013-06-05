// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/depth_stencil_target.h>

namespace butane {
  DepthStencilTarget::DepthStencilTarget(
    Texture* texture
  ) : _texture(texture)
  {
  }

  DepthStencilTarget::~DepthStencilTarget()
  {
    if (_texture)
      _texture->destroy();
  }
} // butane
