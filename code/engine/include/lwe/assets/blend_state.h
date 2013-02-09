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

#ifndef _LWE_ASSETS_BLEND_STATE_H_
#define _LWE_ASSETS_BLEND_STATE_H_

#include <lwe/asset.h>

typedef enum lwe_blend_factor_t {
  LWE_BLEND_FACTOR_FORCE_UINT32   = 0xFFFFFFFFu,
  LWE_BLEND_FACTOR_ZERO           = 1,
  LWE_BLEND_FACTOR_ONE            = 2,
  LWE_BLEND_FACTOR_SRC_COLOR      = 3,
  LWE_BLEND_FACTOR_INV_SRC_COLOR  = 4,
  LWE_BLEND_FACTOR_SRC_ALPHA      = 5,
  LWE_BLEND_FACTOR_INV_SRC_ALPHA  = 6,
  LWE_BLEND_FACTOR_DEST_ALPHA     = 7,
  LWE_BLEND_FACTOR_INV_DEST_ALPHA = 8,
  LWE_BLEND_FACTOR_DEST_COLOR     = 9,
  LWE_BLEND_FACTOR_INV_DEST_COLOR = 10,
  LWE_BLEND_FACTOR_SRC_ALPHA_SAT  = 11
} lwe_blend_factor_t;

typedef enum lwe_blend_operation_t {
  LWE_BLEND_OP_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_BLEND_OP_ADD          = 1,
  LWE_BLEND_OP_SUBTRACT     = 2,
  LWE_BLEND_OP_REV_SUBTRACT = 3,
  LWE_BLEND_OP_MIN          = 4,
  LWE_BLEND_OP_MAX          = 5
} lwe_blend_operation_t;

typedef struct lwe_blend_state_blob_t {
  bool enabled;

  struct {
    lwe_blend_factor_t src;
    lwe_blend_factor_t dest;
    lwe_blend_operation_t op;
  } color;

  struct {
    lwe_blend_factor_t src;
    lwe_blend_factor_t dest;
    lwe_blend_operation_t op;
  } alpha;
} lwe_blend_state_blob_t;

typedef struct lwe_blend_state_t
  : public lwe_asset_t
{
} lwe_blend_state_t;

extern void lwe_blend_state_register_type();

#endif // _LWE_ASSETS_BLEND_STATE_H_