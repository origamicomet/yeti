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

#ifndef _LWE_ASSETS_RASTERIZER_STATE_H_
#define _LWE_ASSETS_RASTERIZER_STATE_H_

#include <lwe/asset.h>

typedef enum lwe_fill_mode_t {
  LWE_FILL_MODE_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_FILL_WIREFRAME         = 1,
  LWE_FILL_SOLID             = 2
} lwe_fill_mode_t;

typedef enum lwe_cull_mode_t {
  LWE_CULL_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_CULL_NONE         = 1,
  LWE_CULL_FRONT        = 2,
  LWE_CULL_BACK         = 3
} lwe_cull_mode_t;

typedef enum lwe_front_face_t {
  LWE_FRONT_FACE_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_FRONT_FACE_CW           = 1,
  LWE_FRONT_FACE_CCW          = 2
} lwe_front_face_t;

typedef struct lwe_rasterizer_state_blob_t {
  lwe_fill_mode_t fill_mode;
  lwe_cull_mode_t cull_mode;
  lwe_front_face_t front_face;
  bool scissor_enabled;
} lwe_rasterizer_state_blob_t;

typedef struct lwe_rasterizer_state_t
  : public lwe_asset_t
{
} lwe_rasterizer_state_t;

extern void lwe_rasterizer_state_register_type();

#endif // _LWE_ASSETS_RASTERIZER_STATE_H_