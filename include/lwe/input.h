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

#ifndef _LWE_INPUT_H_
#define _LWE_INPUT_H_

#include <lwe/foundation.h>

typedef enum lwe_key_code_t {
  LWE_KEY_CODE_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_KEY_CODE_INVALID      = 0x00000000u,
} lwe_key_code_t;

typedef enum lwe_mouse_button_t {
  LWE_MOUSE_BUTTON_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_MOUSE_BUTTON_INVALID      = 0x00000000u,
  LWE_MOUSE_BUTTON_LEFT         = 0x00000001u,
  LWE_MOUSE_BUTTON_RIGHT        = 0x00000002u,
  LWE_MOUSE_BUTTON_MIDDLE       = 0x00000003u
} lwe_mouse_button_t;

typedef enum lwe_input_event_type_t {
  LWE_INPUT_EVENT_FORCE_UINT32          = 0xFFFFFFFFu,
  LWE_INPUT_EVENT_INVALID               = 0x00000000u,
  LWE_INPUT_EVENT_KEY_PRESSED           = 0x00000001u,
  LWE_INPUT_EVENT_KEY_RELEASED          = 0x00000002u,
  LWE_INPUT_EVENT_MOUSE_MOVED           = 0x00000003u,
  LWE_INPUT_EVENT_MOUSE_BUTTON_PRESSED  = 0x00000004u,
  LWE_INPUT_EVENT_MOUSE_BUTTON_RELEASED = 0x00000005u
} lwe_input_event_type_t;

typedef struct lwe_input_event_t {
  lwe_input_event_type_t type;

  union {
    struct {
      lwe_key_code_t code;
    } key;

    struct {
      union {
        struct {
          int32_t x;
          int32_t y;
        } pos;

        lwe_mouse_button_t button;
      };
    } mouse;
  };
} lwe_input_event_t;

typedef lwe_queue_t<lwe_input_event_t> lwe_input_queue_t;

#endif // _LWE_INPUT_H_