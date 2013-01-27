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

#include <lwe/d3d11_pixel_format.h>

DXGI_FORMAT lwe_pixel_format_to_dxgi(
  lwe_pixel_format_t pixel_format )
{
  switch (pixel_format) {
    case LWE_PIXEL_FORMAT_R8G8B8A8:
      return DXGI_FORMAT_R8G8B8A8_UNORM;

    case LWE_PIXEL_FORMAT_R8G8B8A8_SRGB:
      return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

    case LWE_PIXEL_FORMAT_DXT1:
      return DXGI_FORMAT_BC1_UNORM;

    case LWE_PIXEL_FORMAT_DXT3:
      return DXGI_FORMAT_BC2_UNORM;

    case LWE_PIXEL_FORMAT_DXT5:
      return DXGI_FORMAT_BC3_UNORM;

    case LWE_PIXEL_FORMAT_D24_S8:
      return DXGI_FORMAT_D24_UNORM_S8_UINT;
  }

  return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT lwe_pixel_format_to_typeless_dxgi(
  lwe_pixel_format_t pixel_format )
{
  switch (pixel_format) {
    case LWE_PIXEL_FORMAT_D24_S8:
      return DXGI_FORMAT_R24G8_TYPELESS;
  }

  return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT lwe_pixel_format_to_masked_typeless_dxgi(
  lwe_pixel_format_t pixel_format )
{
  switch (pixel_format) {
    case LWE_PIXEL_FORMAT_D24_S8:
      return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
  }

  return DXGI_FORMAT_UNKNOWN;
}