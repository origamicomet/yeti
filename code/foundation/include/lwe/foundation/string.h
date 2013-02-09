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

#ifndef _LWE_FOUNDATION_STRING_H_
#define _LWE_FOUNDATION_STRING_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>

#include <stdio.h>
#include <string.h>

typedef char* lwe_str_t;
typedef const lwe_str_t lwe_const_str_t;

#if defined(LWE_WINDOWS_BUILD)
  typedef wchar_t lwe_native_char_t;
  typedef lwe_native_char_t* lwe_native_str_t;
  typedef const lwe_native_str_t lwe_const_native_str_t;
#elif defined(LWE_MACOSX_BUILD) || defined(LWE_LINUX_BUILD)
  #error Platform is not implemented.
#else
  #error Platform is not supported.
#endif

#if defined(LWE_MSVC_BUILD)
  #define snprintf _snprintf
  #define vsnprintf _vsnprintf
#endif

extern lwe_size_t lwe_to_native_str(
  lwe_const_str_t str,
  int str_len,
  lwe_native_str_t native_str );

extern lwe_size_t lwe_from_native_str(
  lwe_const_native_str_t native_str,
  int native_str_len,
  lwe_str_t str );

#endif // _LWE_FOUNDATION_STRING_H_