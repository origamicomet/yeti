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
  LWE_KEY_ESC,
  LWE_KEY_F1,
  LWE_KEY_F2,
  LWE_KEY_F3,
  LWE_KEY_F4,
  LWE_KEY_F5,
  LWE_KEY_F6,
  LWE_KEY_F7,
  LWE_KEY_F8,
  LWE_KEY_F9,
  LWE_KEY_F10,
  LWE_KEY_F11,
  LWE_KEY_F12,
  LWE_KEY_UP,
  LWE_KEY_DOWN,
  LWE_KEY_LEFT,
  LWE_KEY_RIGHT,
  LWE_KEY_LSHIFT,
  LWE_KEY_RSHIFT,
  LWE_KEY_LCTRL,
  LWE_KEY_RCTRL,
  LWE_KEY_LALT,
  LWE_KEY_RALT,
  LWE_KEY_TAB,
  LWE_KEY_ENTER,
  LWE_KEY_BACKSPACE,
  LWE_KEY_INSERT,
  LWE_KEY_DEL,
  LWE_KEY_PAGEUP,
  LWE_KEY_PAGEDOWN,
  LWE_KEY_HOME,
  LWE_KEY_END,
  LWE_KEY_NUM_PAD_0,
  LWE_KEY_NUM_PAD_1,
  LWE_KEY_NUM_PAD_2,
  LWE_KEY_NUM_PAD_3,
  LWE_KEY_NUM_PAD_4,
  LWE_KEY_NUM_PAD_5,
  LWE_KEY_NUM_PAD_6,
  LWE_KEY_NUM_PAD_7,
  LWE_KEY_NUM_PAD_8,
  LWE_KEY_NUM_PAD_9,
  LWE_KEY_DIVIDE,
  LWE_KEY_MULTIPLY,
  LWE_KEY_SUBTRACT,
  LWE_KEY_ADD,
  LWE_KEY_DECIMAL,
  LWE_KEY_SPACE,
  LWE_KEY_0,
  LWE_KEY_1,
  LWE_KEY_2,
  LWE_KEY_3,
  LWE_KEY_4,
  LWE_KEY_5,
  LWE_KEY_6,
  LWE_KEY_7,
  LWE_KEY_8,
  LWE_KEY_9,
  LWE_KEY_A,
  LWE_KEY_B,
  LWE_KEY_C,
  LWE_KEY_D,
  LWE_KEY_E,
  LWE_KEY_F,
  LWE_KEY_G,
  LWE_KEY_H,
  LWE_KEY_I,
  LWE_KEY_J,
  LWE_KEY_K,
  LWE_KEY_L,
  LWE_KEY_M,
  LWE_KEY_N,
  LWE_KEY_O,
  LWE_KEY_P,
  LWE_KEY_Q,
  LWE_KEY_R,
  LWE_KEY_S,
  LWE_KEY_T,
  LWE_KEY_U,
  LWE_KEY_V,
  LWE_KEY_W,
  LWE_KEY_X,
  LWE_KEY_Y,
  LWE_KEY_Z
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