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

#ifndef _LWE_TIME_STEP_POLICY_H_
#define _LWE_TIME_STEP_POLICY_H_

#include <lwe/foundation.h>

typedef enum lwe_time_step_policy_t {
  LWE_TIME_STEP_POLICY_FORCE_UINT32          = 0xFFFFFFFFu,
  LWE_TIME_STEP_POLICY_INVALID               = 0x00000000u,
  LWE_TIME_STEP_POLICY_VARIABLE              = 0x00000001u,
  LWE_TIME_STEP_POLICY_FIXED                 = 0x00000002u,
  LWE_TIME_STEP_POLICY_SMOOTHED              = 0x00000003u,
  LWE_TIME_STEP_POLICY_SMOOTHED_WITH_PAYBACK = 0x00000004u 
} lwe_time_step_policy_t;

typedef struct lwe_time_step_policy_params_t {
  struct {
  } variable;

  struct {
    float target_frame_rate;
  } fixed;

  struct {
  } smoothed;

  struct {
  } smoothed_with_payback;
} lwe_time_step_policy_params_t;

extern lwe_time_step_policy_t lwe_string_to_time_step_policy(
  lwe_const_str_t str );

#endif // _LWE_TIME_STEP_POLICY_H_