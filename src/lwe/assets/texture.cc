// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#include <lwe/assets/texture.h>

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

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

typedef struct {
  uint32_t size;
  uint32_t flags;
  uint32_t fourcc;
  uint32_t rgb_bit_count;
  uint32_t r_bitmask;
  uint32_t g_bitmask;
  uint32_t b_bitmask;
  uint32_t a_bitmask;
} DDS_PIXELFORMAT;

typedef struct {
  uint32_t size;
  uint32_t header_flags;
  uint32_t height;
  uint32_t width;
  uint32_t pitch_or_linear_size;
  uint32_t depth; // Only if DDS_HEADER_FLAGS_VOLUME is set in header_flags.
  uint32_t mipmap_count;
  uint32_t reserved1[11];
  DDS_PIXELFORMAT pixel_format;
  uint32_t surface_flags;
  uint32_t cubemap_flags;
  uint32_t reserved2[3];
} DDS_HEADER;

static const DDS_PIXELFORMAT DDSPF_DXT1 = {
  sizeof(DDS_PIXELFORMAT), DDS_FOURCC, 0x31545844,
  0, 0, 0, 0, 0
};

static const DDS_PIXELFORMAT DDSPF_DXT3 = {
  sizeof(DDS_PIXELFORMAT), DDS_FOURCC, 0x33545844,
  0, 0, 0, 0, 0
};

static const DDS_PIXELFORMAT DDSPF_DXT5 = {
  sizeof(DDS_PIXELFORMAT), DDS_FOURCC, 0x35545844,
  0, 0, 0, 0, 0
};

static const DDS_PIXELFORMAT DDSPF_A8R8G8B8 = { 
  sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32,
  0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000
};

typedef struct {
  const DDS_PIXELFORMAT* ddspf;
  lwe_pixel_format_t lwepf;
} DDSPF_TO_LWEPF;

static const DDSPF_TO_LWEPF* _ddspf_to_lwepf(
  const DDS_HEADER* header )
{
  static const DDSPF_TO_LWEPF _lut[] = {
    { &DDSPF_DXT1, LWE_PIXEL_FORMAT_DXT1 },
    { &DDSPF_DXT3, LWE_PIXEL_FORMAT_DXT3 },
    { &DDSPF_DXT5, LWE_PIXEL_FORMAT_DXT5 },
    { &DDSPF_A8R8G8B8, LWE_PIXEL_FORMAT_R8G8B8A8 }
  };

  static const lwe_size_t _num =
    sizeof(_lut) / sizeof(DDSPF_TO_LWEPF);

  for (lwe_size_t pf_idx = 0; pf_idx < _num; ++pf_idx ) {
    if (memcmp((void*)_lut[pf_idx].ddspf, (void*)&header->pixel_format, sizeof(DDS_PIXELFORMAT)) != 0)
      continue;

    return &_lut[pf_idx];
  }

  return NULL;
}

static void _swizzle_argb(
  const DDS_HEADER* header,
  const void* data,
  lwe_size_t num_faces )
{
  for (lwe_size_t face = 0; face < num_faces; ++face) {
    lwe_size_t mw = header->width;
    lwe_size_t mh = header->height;

    for (lwe_size_t i = 0; i < mw * mh * 4; i += 4) {
      uint32_t* uint_data = ((uint32_t*)data);
      const uint32_t px = uint_data[i / 4];
      uint_data[i / 4] = ((px & 0x00ffffff) << 8) | ((px & 0xff000000) >> 24);
    }

    data = (void*)(((uint8_t*)data) + mw * mh * 4);

    for (lwe_size_t level = 0; level < header->mipmap_count; ++level) {
      mw >>= 1;
      mh >>= 1;

      for (lwe_size_t i = 0; i < mw * mh * 4; i += 4) {
        uint32_t* uint_data = ((uint32_t*)data);
        const uint32_t px = uint_data[i / 4];
        uint_data[i / 4] = ((px & 0x00ffffff) << 8) | ((px & 0xff000000) >> 24);
      }

      data = (void*)(((uint8_t*)data) + mw * mh * 4);
    }
  }
}

static lwe_size_t _determine_num_faces(
  const DDS_HEADER* header )
{
  if( header->surface_flags & DDS_SURFACE_FLAGS_TEXTURE ) {
    return (header->header_flags & DDS_HEADER_FLAGS_DEPTH) ? header->depth : 1;
  } else if( header->surface_flags & DDS_SURFACE_FLAGS_CUBEMAP ) {
    if( (header->cubemap_flags & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES )
      return 0;
    return 6;
  }

  return 0;
}

static lwe_texture_type_t _determine_texture_type(
  const DDS_HEADER* header )
{
  if( header->surface_flags & DDS_SURFACE_FLAGS_TEXTURE ) {
    if (header->header_flags & DDS_HEADER_FLAGS_DEPTH)
      return (header->depth > 1) ? LWE_TEXTURE_TYPE_2D_ARRAY : LWE_TEXTURE_TYPE_2D;

    return LWE_TEXTURE_TYPE_2D;
  } else if( header->surface_flags & DDS_SURFACE_FLAGS_CUBEMAP ) {
    if( (header->cubemap_flags & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES )
      return LWE_TEXTURE_TYPE_INVALID;

    return LWE_TEXTURE_TYPE_CUBE_MAP;
  }

  return LWE_TEXTURE_TYPE_INVALID;
}

bool lwe_texture_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd )
{
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_TEXTURE);
  lwe_assert(acd != NULL);

  char magic[4];

  if (fread((void*)&magic[0], sizeof(char), 4, acd->in) != 4) {
    lwe_log("  > Not a valid DirectDrawSurface file!\n");
    return FALSE;
  }

  if (strncmp(magic, "DDS ", 4) != 0) {
    lwe_log("  > Not a valid DirectDrawSurface file!\n");
    return FALSE;
  }

  DDS_HEADER dds_header;

  if (fread((void*)&dds_header, sizeof(DDS_HEADER), 1, acd->in) != 1) {
    lwe_log("  > Not a valid DirectDrawSurface file!\n");
    return FALSE;
  }

  const lwe_texture_type_t texture_type = _determine_texture_type(&dds_header);

  if (texture_type == LWE_TEXTURE_TYPE_INVALID) {
    lwe_log("  > Invalid DirectDrawSurface texture type!\n");
    return FALSE;
  }

  const DDSPF_TO_LWEPF* pixel_format = _ddspf_to_lwepf(&dds_header);

  if (!pixel_format) {
    lwe_log("  > Not a supported pixel format!\n");
    return FALSE;
  }

  lwe_log(
    "  > pixel_format=%s is_compressed=%s\n",
    lwe_pixel_format_to_string(pixel_format->lwepf),
    lwe_pixel_format_is_compressed(pixel_format->lwepf) ? "true" : "false"
  );

  lwe_size_t data_len = 0; {
    const lwe_offset_t orig = ftell(acd->in);
    fseek(acd->in, 0, SEEK_END);
    data_len = ftell(acd->in) - orig;
    fseek(acd->in, orig, SEEK_SET);
  }

  void* data = lwe_alloc(data_len);

  if (fread(data, 1, data_len, acd->in) != data_len) {
    lwe_log("  > Malformed DirectDrawSurface!\n");
    lwe_free(data);
    return FALSE;
  }

  const lwe_size_t num_faces = _determine_num_faces(&dds_header);

  if (pixel_format->ddspf == &DDSPF_A8R8G8B8)
    _swizzle_argb(&dds_header, data, num_faces);

  lwe_texture_blob_t tex_blob;
  tex_blob.type = texture_type;
  tex_blob.pixel_format = pixel_format->lwepf;
  tex_blob.width = dds_header.width;
  tex_blob.height = dds_header.height;
  tex_blob.depth = num_faces;
  tex_blob.flags = 0;

  if ((dds_header.header_flags & DDS_HEADER_FLAGS_MIPMAP))
    tex_blob.flags |= LWE_TEXTURE_FLAGS_PRECOMPUTED_MIPS;
  else
    tex_blob.flags |= LWE_TEXTURE_FLAGS_NO_MIPS;

  if (fwrite((void*)&tex_blob, sizeof(lwe_texture_blob_t), 1, acd->mrd) != 1) {
    lwe_log("  > Unable to write memory-resident data!\n");
    lwe_free(data);
    return FALSE;
  }

  if (fwrite(data, 1, data_len, acd->mrd) != data_len) {
    lwe_log("  > Unable to write memory-resident data!\n");
    lwe_free(data);
    return FALSE;
  }

  lwe_free(data);
  return TRUE;
}