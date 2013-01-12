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

#ifndef _LWE_FOUNDATION_TYPES_H_
#define _LWE_FOUNDATION_TYPES_H_

#define __STDC_FORMAT_MACROS
#define __STDC_LIMIT_MACROS

#if defined(_MSC_VER)
  #include <lwe/foundation/msvc/stdint.h>
  #include <lwe/foundation/msvc/inttypes.h>
#else
  #include <stdint.h>
  #include <inttypes.h>
#endif

typedef uint32_t lwe_hash_t;
typedef uint32_t lwe_hash_seed_t;
typedef uint32_t lwe_type_id_t;
typedef uint32_t lwe_ref_count_t;
typedef uint32_t lwe_size_t;
typedef uint32_t lwe_offset_t;
typedef uint32_t lwe_file_pos_t;

typedef enum lwe_seek_origin_t {
  LWE_SEEK_ORIGIN_INVALID      = 0x00000000u,
  LWE_SEEK_ORIGIN_FORCE_UINT32 = 0xFFFFFFFFu,

  LWE_SEEK_ORIGIN_START   = 0x00000001u,
  LWE_SEEK_ORIGIN_CURRENT = 0x00000002u,
  LWE_SEEK_ORIGIN_END     = 0x00000003u
} lwe_seek_origin_t;

#define LWE_HASH_FORMAT "08x"

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE 1
#endif

#endif // _LWE_FOUNDATION_TYPES_H_