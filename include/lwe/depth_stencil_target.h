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

#ifndef _LWE_DEPTH_STENCIL_TARGET_H_
#define _LWE_DEPTH_STENCIL_TARGET_H_

#include <lwe/foundation.h>
#include <lwe/pixel_format.h>

typedef struct lwe_depth_stencil_target_t {
  lwe_pixel_format_t pixel_format;
  uint32_t width;
  uint32_t height;
} lwe_depth_stencil_target_t;

extern lwe_depth_stencil_target_t* lwe_depth_stencil_target_create(
  lwe_pixel_format_t pixel_format,
  uint32_t width,
  uint32_t height );

extern void lwe_depth_stencil_target_destroy(
  lwe_depth_stencil_target_t* depth_stencil_target );

#endif // _LWE_DEPTH_STENCIL_TARGET_H_