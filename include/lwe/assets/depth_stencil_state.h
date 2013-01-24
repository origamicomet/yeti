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

#ifndef _LWE_ASSETS_DEPTH_STENCIL_STATE_H_
#define _LWE_ASSETS_DEPTH_STENCIL_STATE_H_

#include <lwe/asset.h>

typedef enum lwe_comparison_func_t {
  LWE_COMPARISON_FORCE_UINT32  = 0xFFFFFFFFu,
  LWE_COMPARISON_NEVER         = 1,
  LWE_COMPARISON_LESS          = 2,
  LWE_COMPARISON_EQUAL         = 3,
  LWE_COMPARISON_LESS_EQUAL    = 4,
  LWE_COMPARISON_GREATER       = 5,
  LWE_COMPARISON_NOT_EQUAL     = 6,
  LWE_COMPARISON_GREATER_EQUAL = 7,
  LWE_COMPARISON_ALWAYS        = 8
} lwe_comparison_func_t;

typedef enum lwe_stencil_operation_t {
  LWE_STENCIL_OP_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_STENCIL_OP_KEEP         = 1,
  LWE_STENCIL_OP_ZERO         = 2,
  LWE_STENCIL_OP_REPLACE      = 3,
  LWE_STENCIL_OP_INCR_SAT     = 4,
  LWE_STENCIL_OP_DECR_SAT     = 5,
  LWE_STENCIL_OP_INVERT       = 6,
  LWE_STENCIL_OP_INCR         = 7,
  LWE_STENCIL_OP_DECR         = 8
} lwe_stencil_operation_t;

typedef struct lwe_depth_stencil_state_blob_t {
  struct {
    bool tests;
    bool writes;
    lwe_comparison_func_t func;
  } depth;

  struct {
    bool enabled;

    struct {
      lwe_stencil_operation_t stencil_fail_op;
      lwe_stencil_operation_t depth_fail_op;
      lwe_stencil_operation_t pass_op;
      lwe_comparison_func_t func;
    } front_face;

    struct {
      lwe_stencil_operation_t stencil_fail_op;
      lwe_stencil_operation_t depth_fail_op;
      lwe_stencil_operation_t pass_op;
      lwe_comparison_func_t func;
    } back_face;
  } stencil;
} lwe_depth_stencil_state_blob_t;

typedef struct lwe_depth_stencil_state_t
  : public lwe_asset_t
{
} lwe_depth_stencil_state_t;

extern void lwe_depth_stencil_state_register_type();

#endif // _LWE_ASSETS_DEPTH_STENCIL_STATE_H_