// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/pixel_format.h>

namespace butane {
  PixelFormat PixelFormat::from_str(
    const char* str )
  {
    assert(str != nullptr);

    static const Pair<const char*, PixelFormat> _str_to_pf[] = {
      Pair<const char*, PixelFormat>("R8G8B8A8", PixelFormat::R8G8B8A8),
      Pair<const char*, PixelFormat>("R8G8B8A8_SRGB", PixelFormat::R8G8B8A8_SRGB),
      Pair<const char*, PixelFormat>("R16FG16F", PixelFormat::R16FG16F),
      Pair<const char*, PixelFormat>("DXT1", PixelFormat::DXT1),
      Pair<const char*, PixelFormat>("DXT3", PixelFormat::DXT3),
      Pair<const char*, PixelFormat>("DXT5", PixelFormat::DXT5),
      Pair<const char*, PixelFormat>("D24S8", PixelFormat::D24S8)
    };

    for (size_t i = 0; i < (sizeof(_str_to_pf) / sizeof(_str_to_pf[0])); ++i) {
      if (strcmp(_str_to_pf[i].key, str) != 0)
        continue;
      return _str_to_pf[i].value;
    }

    return PixelFormat(PixelFormat::INVALID);
  }

  size_t PixelFormat::row_stride(
    const uint32_t width ) const
  {
    switch (_value) {
      case R8G8B8A8:
      case R8G8B8A8_SRGB:
        return width * 4;
      case R16FG16F:
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
    switch (_value) {
      case R8G8B8A8:
      case R8G8B8A8_SRGB:
        return width * height * 4;
      case R16FG16F:
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

