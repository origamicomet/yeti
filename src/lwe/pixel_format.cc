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

#include <lwe/pixel_format.h>

lwe_size_t lwe_pixel_format_row_stride(
  lwe_pixel_format_t pixel_format,
  uint32_t width )
{
  switch (pixel_format) {
    case LWE_PIXEL_FORMAT_R8G8B8A8:
    case LWE_PIXEL_FORMAT_R8G8B8A8_SRGB:
      return width * 4;

    case LWE_PIXEL_FORMAT_D24_S8:
      return width * 4;
  }

  return 0;
}

lwe_size_t lwe_pixel_format_layer_stride(
  lwe_pixel_format_t pixel_format,
  uint32_t width,
  uint32_t height )
{
  switch (pixel_format) {
    case LWE_PIXEL_FORMAT_R8G8B8A8:
    case LWE_PIXEL_FORMAT_R8G8B8A8_SRGB:
      return width * 4;

    case LWE_PIXEL_FORMAT_DXT1:
      return ((width + 3) / 4) * ((height + 3) / 4) * 8;

    case LWE_PIXEL_FORMAT_DXT3:
    case LWE_PIXEL_FORMAT_DXT5:
      return ((width + 3) / 4) * ((height + 3) / 4) * 16;

    case LWE_PIXEL_FORMAT_D24_S8:
      return width * 4;
  }

  return 0;
}

lwe_size_t lwe_pixel_format_storage_requirements(
  lwe_pixel_format_t pixel_format,
  uint32_t width,
  uint32_t height,
  uint32_t depth )
{
  return lwe_pixel_format_layer_stride(pixel_format, width, height) * depth;
}