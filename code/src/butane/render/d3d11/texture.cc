// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/render/d3d11/texture.h>

#include <butane/application.h>
#include <butane/render/device.h>
#include <butane/render/d3d11/device.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("textures", Allocators::heap());
    return allocator;
  }

  D3D11Texture::D3D11Texture(
    const Type type,
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height,
    const uint32_t depth
  ) : butane::Texture(type, pixel_format, width, height, depth)
  {
  }

  D3D11Texture::~D3D11Texture()
  {
    if (_resource)
      _resource->Release();
  }

  void D3D11Texture::destroy()
  {
    make_delete(D3D11Texture, allocator(), this);
  }

  Texture* Texture::create(
    const Type type,
    const PixelFormat pixel_format,
    const uint32_t width,
    const uint32_t height,
    const uint32_t depth,
    bool streamable )
  {
    return nullptr;
  }
} // butane
