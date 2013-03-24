// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/render_target.h>

namespace bt {
  RenderTarget::RenderTarget()
    : _texture(nullptr)
  {
  }

  RenderTarget::~RenderTarget()
  {
    if (_texture)
      _texture->destroy();
  }
} // bt