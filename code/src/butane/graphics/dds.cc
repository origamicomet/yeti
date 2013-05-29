// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/graphics/dds.h>

namespace butane {
  bool type_from_dds(
    const DDS_HEADER& dds,
    Texture::Type& type )
  {
    if (dds.surface_flags & DDS_SURFACE_FLAGS_TEXTURE) {
      if (dds.header_flags & DDS_HEADER_FLAGS_DEPTH) {
        type = Texture::TEXTURE_2D;
        return true; }
      type = Texture::TEXTURE_2D;
      return true;
    } else if (dds.surface_flags & DDS_SURFACE_FLAGS_CUBEMAP) {
      if ((dds.cubemap_flags & DDS_CUBEMAP_ALLFACES) == DDS_CUBEMAP_ALLFACES) {
        type = Texture::CUBE_MAP;
        return true; }
      return false;
    }

    return false;
  }

  bool pixel_format_from_dds(
    const DDS_HEADER& dds,
    PixelFormat& pixel_format )
  {
    typedef Pair<const DDS_PIXELFORMAT*, const PixelFormat> Pair;
    static const Pair fmts[] = {
      Pair(&DDSPF_DXT1, PixelFormat::DXT1),
      Pair(&DDSPF_DXT3, PixelFormat::DXT3),
      Pair(&DDSPF_DXT5, PixelFormat::DXT5),
      Pair(&DDSPF_A8R8G8B8, PixelFormat::R8G8B8A8)
    };

    for (size_t fmt = 0; fmt < (sizeof(fmts) / sizeof(fmts[0])); ++fmt) {
      if (compare((const void*)fmts[fmt].key, (const void*)&dds.pixel_format, sizeof(DDS_PIXELFORMAT))) {
        pixel_format = fmts[fmt].value; return true; } }

    return false;
  }

  uint32_t num_of_faces_from_dds(
    const DDS_HEADER& dds,
    uint32_t& num_of_faces )
  {
    if (dds.surface_flags & DDS_SURFACE_FLAGS_TEXTURE) {
      num_of_faces = ((dds.header_flags & DDS_HEADER_FLAGS_DEPTH) ? dds.depth : 1);
      return true;
    } else if (dds.surface_flags & DDS_SURFACE_FLAGS_CUBEMAP) {
      num_of_faces = (((dds.cubemap_flags & DDS_CUBEMAP_ALLFACES) == DDS_CUBEMAP_ALLFACES) ? 6 : 0);
      return true; }

    return false;
  }
} // butane
