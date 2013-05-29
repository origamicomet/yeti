// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/butane.h>
#include <butane/graphics/texture.h>

#define DDS_MAGIC  0x20534444
#define DDS_FOURCC 0x00000004
#define DDS_RGB    0x00000040
#define DDS_RGBA   0x00000041
#define DDS_DX10   0x30315844

#define DDS_HEADER_FLAGS_TEXTURE    0x00001007
#define DDS_HEADER_FLAGS_MIPMAP     0x00020000
#define DDS_HEADER_FLAGS_VOLUME     0x00800000
#define DDS_HEADER_FLAGS_PITCH      0x00000008
#define DDS_HEADER_FLAGS_LINEARSIZE 0x00080000
#define DDS_HEADER_FLAGS_DEPTH      0x00800000

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008

#define DDS_CUBEMAP_POSITIVEX 0x00000600
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00
#define DDS_CUBEMAP_POSITIVEY 0x00001200
#define DDS_CUBEMAP_NEGATIVEY 0x00002200
#define DDS_CUBEMAP_POSITIVEZ 0x00004200
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200

#define DDS_CUBEMAP_ALLFACES (DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX | \
                              DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY | \
                              DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ)

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

typedef struct {
  uint32_t size;
  uint32_t flags;
  uint32_t fourcc;
  uint32_t bits;
  uint32_t r;
  uint32_t g;
  uint32_t b;
  uint32_t a;
} DDS_PIXELFORMAT;

typedef struct {
  uint32_t size;
  uint32_t header_flags;
  uint32_t height;
  uint32_t width;
  uint32_t pitch_or_linear_size;
  uint32_t depth;
  uint32_t mipmap_count;
  uint32_t reserved1[11];
  DDS_PIXELFORMAT pixel_format;
  uint32_t surface_flags;
  uint32_t cubemap_flags;
  uint32_t reserved2[3];
} DDS_HEADER;

static const DDS_PIXELFORMAT DDSPF_DXT1 = {
  sizeof(DDS_PIXELFORMAT), DDS_FOURCC, 0x31545844, 0, 0, 0, 0, 0 };

static const DDS_PIXELFORMAT DDSPF_DXT3 = {
  sizeof(DDS_PIXELFORMAT), DDS_FOURCC, 0x33545844, 0, 0, 0, 0, 0 };

static const DDS_PIXELFORMAT DDSPF_DXT5 = {
  sizeof(DDS_PIXELFORMAT), DDS_FOURCC, 0x35545844, 0, 0, 0, 0, 0 };

static const DDS_PIXELFORMAT DDSPF_A8R8G8B8 = {
  sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32,
  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };

namespace butane {
  extern BUTANE_EXPORT bool type_from_dds(
    const DDS_HEADER& dds,
    Texture::Type& type );

  extern BUTANE_EXPORT bool pixel_format_from_dds(
    const DDS_HEADER& dds,
    PixelFormat& pixel_format );

  extern BUTANE_EXPORT uint32_t num_of_faces_from_dds(
    const DDS_HEADER& dds,
    uint32_t& num_of_faces );
} // butane
