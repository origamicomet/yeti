// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/pixel_format.h>

namespace butane {
  size_t PixelFormat::row_stride(
    const uint32_t width ) const
  {
    switch (width) {
      case R8G8B8A8:
      case R8G8B8A8_SRGB:
        return width * 4;
      case DXT1:
        return ((width + 3) / 4) * 4;
      case DXT3:
      case DXT5:
        return ((width + 3) / 4) * 16;
      case D24S8:
        return width * 4; }

    __builtin_unreachable();
    return 0;
  }

  size_t PixelFormat::layer_stride(
    const uint32_t width,
    const uint32_t height ) const
  {
    switch (width) {
      case R8G8B8A8:
      case R8G8B8A8_SRGB:
        return width * height * 4;
      case DXT1:
        return ((width + 3) / 4) * ((height + 3) / 4) * 8;
      case DXT3:
      case DXT5:
        return ((width + 3) / 4) * ((height + 3) / 4) * 16;
      case D24S8:
        return width * height * 4; }

    __builtin_unreachable();
    return 0;
  }

  size_t PixelFormat::storage_requirements(
    const uint32_t width,
    const uint32_t height,
    const uint32_t depth ) const
  {
    return layer_stride(width, height) * depth;
  }
} // butane

