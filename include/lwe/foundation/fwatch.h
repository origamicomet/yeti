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

#ifndef _LWE_FOUNDATION_FWATCH_H_
#define _LWE_FOUNDATION_FWATCH_H_

#include <lwe/foundation/string.h>

typedef struct lwe_fwatch_t lwe_fwatch_t;

typedef enum lwe_fwatch_event_t {
  LWE_FWATCH_EVENT_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_FWATCH_EVENT_INVALID      = 0x00000000u,
  LWE_FWATCH_EVENT_CREATED      = 0x00000001u,
  LWE_FWATCH_EVENT_DESTROYED    = 0x00000002u,
  LWE_FWATCH_EVENT_MODIFIED     = 0x00000003u 
} lwe_fwatch_event_t;

typedef void (*lwe_fwatch_callback_t)(
  void* user_ptr,
  lwe_fwatch_event_t event,
  lwe_const_str_t path );

extern lwe_fwatch_t* lwe_fwatch_start(
  lwe_const_str_t path,
  void* user_ptr,
  lwe_fwatch_callback_t on_modified );

extern void lwe_fwatch_poll(
  lwe_fwatch_t* fwatch );

extern void lwe_fwatch_stop(
  lwe_fwatch_t* fwatch );

#endif // _LWE_FOUNDATION_FWATCH_H_