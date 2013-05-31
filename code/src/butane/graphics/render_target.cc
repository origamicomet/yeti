// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/render_target.h>

namespace butane {
  RenderTarget::RenderTarget(
    Texture* texture
  ) : _texture(texture)
  {
  }

  RenderTarget::~RenderTarget()
  {
    if (_texture)
      _texture->destroy();
  }
} // butane
