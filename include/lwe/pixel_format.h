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

#ifndef _LWE_PIXEL_FORMAT_H_
#define _LWE_PIXEL_FORMAT_H_

#include <lwe/foundation.h>

#define LWE_PIXEL_FORMAT_COLOR_BIT      (1u << 31u)
#define LWE_PIXEL_FORMAT_DEPTH_BIT      (1u << 30u)
#define LWE_PIXEL_FORMAT_STENCIL_BIT    (1u << 29u)
#define LWE_PIXEL_FORMAT_COMPRESSED_BIT (1u << 28u)

#define LWE_PIXEL_FORMAT_FLAGS_MASK 0xF0000000u
#define LWE_PIXEL_FORMAT_TYPE_MASK  0x0FFFFFFFu

typedef enum lwe_pixel_format_t {
  LWE_PIXEL_FORMAT_INVALID      = 0x00000000u,
  LWE_PIXEL_FORMAT_FORCE_UINT32 = 0xFFFFFFFFu,

  LWE_PIXEL_FORMAT_R8G8B8A8 =
    0x00000001u | LWE_PIXEL_FORMAT_COLOR_BIT,

  LWE_PIXEL_FORMAT_R8G8B8A8_SRGB =
    0x00000002u | LWE_PIXEL_FORMAT_COLOR_BIT,

  LWE_PIXEL_FORMAT_DXT1 =
    0x00000003u | LWE_PIXEL_FORMAT_COLOR_BIT | LWE_PIXEL_FORMAT_COMPRESSED_BIT,

  LWE_PIXEL_FORMAT_DXT3 =
    0x00000004u | LWE_PIXEL_FORMAT_COLOR_BIT | LWE_PIXEL_FORMAT_COMPRESSED_BIT,

  LWE_PIXEL_FORMAT_DXT5 =
    0x00000005u | LWE_PIXEL_FORMAT_COLOR_BIT | LWE_PIXEL_FORMAT_COMPRESSED_BIT,

  LWE_PIXEL_FORMAT_D24_S8 =
    0x00000006u | LWE_PIXEL_FORMAT_DEPTH_BIT,
} lwe_pixel_format_t;

LWE_INLINE bool lwe_pixel_format_is_color(
  lwe_pixel_format_t pixel_format )
{
  return (pixel_format & LWE_PIXEL_FORMAT_COLOR_BIT) != 0;
}

LWE_INLINE bool lwe_pixel_format_is_depth(
  lwe_pixel_format_t pixel_format )
{
  return (pixel_format & LWE_PIXEL_FORMAT_DEPTH_BIT) != 0;
}

LWE_INLINE bool lwe_pixel_format_is_stencil(
  lwe_pixel_format_t pixel_format )
{
  return (pixel_format & LWE_PIXEL_FORMAT_STENCIL_BIT) != 0;
}

LWE_INLINE bool lwe_pixel_format_is_compressed(
  lwe_pixel_format_t pixel_format )
{
  return (pixel_format & LWE_PIXEL_FORMAT_COMPRESSED_BIT) != 0;
}

extern lwe_size_t lwe_pixel_format_row_stride(
  lwe_pixel_format_t pixel_format,
  uint32_t width );

extern lwe_size_t lwe_pixel_format_layer_stride(
  lwe_pixel_format_t pixel_format,
  uint32_t width,
  uint32_t height );

extern lwe_size_t lwe_pixel_format_storage_requirements(
  lwe_pixel_format_t pixel_format,
  uint32_t width,
  uint32_t height,
  uint32_t depth );

#endif // _LWE_PIXEL_FORMAT_H_