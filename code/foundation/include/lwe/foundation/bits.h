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

#ifndef _LWE_FOUNDATION_BITS_H_
#define _LWE_FOUNDATION_BITS_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>

#if defined(_MSC_VER)
  #include <intrin.h>

  #pragma intrinsic(_BitScanForward)
  LWE_INLINE unsigned long lwe_count_trailing_zeros( unsigned long x ) {
    unsigned long r = 0;
    _BitScanForward(&r, x);
    return r;
  }

  #pragma intrinsic(_BitScanReverse)
  LWE_INLINE unsigned long lwe_count_leading_zeros( unsigned long x ) {
    unsigned long r = 0;
    _BitScanReverse(&r, x);
    return r;
  }
#elif defined(__GNUC__)
  #define lwe_count_trailing_zeros( x ) __builtin_ctz(x)
  #define lwe_count_leading_zeros( x ) __builtin_clz(x)
#else
  #error Compiler not implemented.
#endif

#endif // _LWE_FOUNDATION_BITS_H_