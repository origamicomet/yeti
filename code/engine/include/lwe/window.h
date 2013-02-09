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

#ifndef _LWE_WINDOW_H_
#define _LWE_WINDOW_H_

#include <lwe/foundation.h>
#include <lwe/input.h>

typedef enum lwe_window_event_type_t {
  LWE_WINDOW_EVENT_FORCE_UINT32 = 0xFFFFFFFFu,
  LWE_WINDOW_EVENT_INVALID      = 0x00000000u,
  LWE_WINDOW_EVENT_CLOSED       = 0x00000001u
} lwe_window_event_type_t;

typedef struct lwe_window_event_t {
  lwe_window_event_type_t type;
} lwe_window_event_t;

typedef lwe_queue_t<lwe_window_event_t> lwe_window_event_queue_t;

typedef struct lwe_window_t {
  uint32_t width;
  uint32_t height;
  bool fullscreen;
  uintptr_t sys_handle;
  lwe_input_queue_t input_events;
  lwe_window_event_queue_t window_events;
} lwe_window_t;

extern lwe_window_t* lwe_window_open(
  lwe_const_str_t title,
  uint32_t width,
  uint32_t height );

extern void lwe_window_set_title(
  lwe_window_t* window,
  lwe_const_str_t new_title );

extern void lwe_window_resize(
  lwe_window_t* window,
  uint32_t new_width,
  uint32_t new_height );

extern void lwe_window_toggle_fullscreen(
  lwe_window_t* window,
  bool fullscreen );

extern void lwe_window_show(
  lwe_window_t* window );

extern void lwe_window_minimize(
  lwe_window_t* window );

extern void lwe_window_maximize(
  lwe_window_t* window );

extern void lwe_window_close(
  lwe_window_t* window );

extern void lwe_message_pump();

#endif // _LWE_WINDOW_H_