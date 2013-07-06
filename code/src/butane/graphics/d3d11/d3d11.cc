// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/d3d11/d3d11.h>

namespace butane {
  DXGI_FORMAT dxgi_format_from_pixel_format(
    const PixelFormat pixel_format )
  {
    switch ((PixelFormat::Value)pixel_format) {
      case PixelFormat::R8G8B8A8:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
      case PixelFormat::R8G8B8A8_SRGB:
        return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
      case PixelFormat::R16FG16F:
        return DXGI_FORMAT_R16G16_FLOAT;
      case PixelFormat::DXT1:
        return DXGI_FORMAT_BC1_UNORM;
      case PixelFormat::DXT3:
        return DXGI_FORMAT_BC2_UNORM;
      case PixelFormat::DXT5:
        return DXGI_FORMAT_BC3_UNORM;
      case PixelFormat::D24S8:
        return DXGI_FORMAT_D24_UNORM_S8_UINT; }

    __builtin_unreachable();
    return DXGI_FORMAT_UNKNOWN;
  }

  DXGI_FORMAT dxgi_typeless_format_from_pixel_format(
    const PixelFormat pixel_format )
  {
    switch ((PixelFormat::Value)pixel_format) {
      case PixelFormat::R8G8B8A8:
      case PixelFormat::R8G8B8A8_SRGB:
        return DXGI_FORMAT_R8G8B8A8_TYPELESS;
      case PixelFormat::R16FG16F:
        return DXGI_FORMAT_R16G16_TYPELESS;
      case PixelFormat::DXT1:
        return DXGI_FORMAT_BC1_TYPELESS;
      case PixelFormat::DXT3:
        return DXGI_FORMAT_BC2_TYPELESS;
      case PixelFormat::DXT5:
        return DXGI_FORMAT_BC3_TYPELESS;
      case PixelFormat::D24S8:
        return DXGI_FORMAT_R24G8_TYPELESS; }

    __builtin_unreachable();
    return DXGI_FORMAT_UNKNOWN;
  }

  DXGI_FORMAT dxgi_samplable_format_from_pixel_format(
    const PixelFormat pixel_format )
  {
    switch ((PixelFormat::Value)pixel_format) {
      case PixelFormat::R8G8B8A8:
      case PixelFormat::R8G8B8A8_SRGB:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
      case PixelFormat::R16FG16F:
        return DXGI_FORMAT_R16G16_FLOAT;
      case PixelFormat::DXT1:
        return DXGI_FORMAT_BC1_UNORM;
      case PixelFormat::DXT3:
        return DXGI_FORMAT_BC2_UNORM;
      case PixelFormat::DXT5:
        return DXGI_FORMAT_BC3_UNORM;
      case PixelFormat::D24S8:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS; }

    __builtin_unreachable();
    return DXGI_FORMAT_UNKNOWN;
  }

  HRESULT D3DInclude::Open(
    D3D_INCLUDE_TYPE include_type,
    LPCSTR requested_path,
    LPCVOID /* parent_data */,
    LPCVOID* out_buffer,
    UINT* out_buffer_len )
  {
    // What constitutes as a system directory?
    if (include_type == D3D_INCLUDE_SYSTEM)
      return E_NOTIMPL;

    const String path =
      String::format(Allocators::scratch(), "%s/%s", input.root, requested_path);

    FILE* fh = File::open(path.raw(), "rb");
    if (!fh)
      return E_ACCESSDENIED;

    size_t length = 0;
    *out_buffer = File::read(fh, Allocators::heap(), &length);
    *out_buffer_len = length;
    fclose(fh);

    if (!*out_buffer)
      return E_FAIL;

    return S_OK;
  }

  HRESULT D3DInclude::Close(
    LPCVOID buffer )
  {
    assert(buffer != nullptr);
    Allocators::heap().free((void*)buffer);
    return S_OK;
  }
} // butane
