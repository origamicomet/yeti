// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/texture.h>

namespace butane {
  Texture::Texture(
    const Type type,
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height,
    const uint32_t depth
  ) : _type(type)
    , _pixel_format(pixel_format)
    , _width(width)
    , _height(height)
    , _depth(depth)
  {
  }

  Texture::~Texture()
  {
  }
} // butane
