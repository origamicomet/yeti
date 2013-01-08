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

#ifndef _LWE_ASSETS_TEXTURE_H_
#define _LWE_ASSETS_TEXTURE_H_

#include <lwe/asset.h>
#include <lwe/pixel_format.h>

typedef enum lwe_texture_type_t {
  LWE_TEXTURE_TYPE_INVALID      = 0x00000000u,
  LWE_TEXTURE_TYPE_FORCE_UINT32 = 0xFFFFFFFFu,

  LWE_TEXTURE_TYPE_1D           = 0x00000001u,
  LWE_TEXTURE_TYPE_1D_ARRAY     = 0x00000002u,
  LWE_TEXTURE_TYPE_2D           = 0x00000003u,
  LWE_TEXTURE_TYPE_2D_ARRAY     = 0x00000004u,
  LWE_TEXTURE_TYPE_3D           = 0x00000005u,
  LWE_TEXTURE_TYPE_CUBE_MAP     = 0x00000006u,
} lwe_texture_type_t;

typedef enum lwe_texture_flags_t {
  LWE_TEXTURE_FLAGS_DEFAULT          = 0x00000000u,
  LWE_TEXTURE_FLAGS_FORCE_UINT32     = 0xFFFFFFFFu,

  LWE_TEXTURE_FLAGS_NO_MAPS          = (1u << 31u),
  LWE_TEXTURE_FLAGS_PRECOMPUTED_MIPS = (1u << 30u),
} lwe_texture_flags_t;

typedef struct lwe_texture_blob_t {
  lwe_texture_type_t type;
  lwe_pixel_format_t pixel_format;
  uint32_t width, height, depth;
  uint32_t flags;
} lwe_texture_blob_t;

typedef struct lwe_texture_t
  : public lwe_asset_t, lwe_texture_blob_t
{
} lwe_texture_t;

extern void lwe_texture_register_type();

#endif // _LWE_ASSETS_TEXTURE_H_