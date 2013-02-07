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

#include <lwe/window.h>
#include <lwe/foundation/platforms/windows.h>
#include "win32.rc.h"

typedef struct lwe_win32_window_t
  : public lwe_window_t
{
  int32_t mouse_x;
  int32_t mouse_y;
} lwe_win32_window_t;

static LRESULT WINAPI _lwe_window_proc(
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam )
{
  lwe_win32_window_t* window =
    (lwe_win32_window_t*)GetPropA(hWnd, "lwe_window");

  switch( uMsg ) {
    case WM_CLOSE: {
      lwe_window_event_t event;
      event.type = LWE_WINDOW_EVENT_CLOSED;
      lwe_queue_enqueue(&window->window_events, &event);
      return 0;
    } break;

    case WM_SIZE: {
      RECT client_rect;
      GetClientRect(hWnd, &client_rect);;
      return 0;
    } break;

    case WM_INPUT: {
      uint8_t buffer[40 /* keyboard=32, mouse=40 */];
      UINT buffer_size = sizeof(buffer);

      lwe_fail_if(
        (GetRawInputData(
          (HRAWINPUT)lParam, RID_INPUT,
          (void*)&buffer[0], &buffer_size,
          sizeof(RAWINPUTHEADER)
        ) == -1),
        "GetRawInputData failed"
      );

      RAWINPUT* raw = (RAWINPUT*)&buffer[0];

      switch (raw->header.dwType) {
        case RIM_TYPEMOUSE: {
          const bool relative =
            (raw->data.mouse.usFlags & MOUSE_MOVE_RELATIVE) != 0;

          const int32_t lmx = raw->data.mouse.lLastX;
          const int32_t lmy = raw->data.mouse.lLastY;

          if (relative) {
            if ((lmx == 0) && (lmy == 0))
              goto handle_mouse_buttons;

            window->mouse_x += lmx;
            window->mouse_y += lmy;
          } else {
            if ((window->mouse_x == lmx) && (window->mouse_y == lmy))
              goto handle_mouse_buttons;

            window->mouse_x = lmx;
            window->mouse_y = lmy;
          }

          lwe_input_event_t event;
          event.type = LWE_INPUT_EVENT_MOUSE_MOVED;
          event.mouse.pos.x = window->mouse_x;
          event.mouse.pos.y = window->mouse_y;
          lwe_queue_enqueue(&window->input_events, &event);

        handle_mouse_buttons:
          const uint16_t button_flags = raw->data.mouse.usButtonFlags;
          
          // Left:
          if (button_flags & RI_MOUSE_LEFT_BUTTON_DOWN) {
            lwe_input_event_t event;
            event.type = LWE_INPUT_EVENT_MOUSE_BUTTON_PRESSED;
            event.mouse.button = LWE_MOUSE_BUTTON_LEFT;
            lwe_queue_enqueue(&window->input_events, &event);
          } else if (button_flags & RI_MOUSE_LEFT_BUTTON_UP) {
            lwe_input_event_t event;
            event.type = LWE_INPUT_EVENT_MOUSE_BUTTON_RELEASED;
            event.mouse.button = LWE_MOUSE_BUTTON_LEFT;
            lwe_queue_enqueue(&window->input_events, &event);
          }

          // Right:
          if (button_flags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
            lwe_input_event_t event;
            event.type = LWE_INPUT_EVENT_MOUSE_BUTTON_PRESSED;
            event.mouse.button = LWE_MOUSE_BUTTON_RIGHT;
            lwe_queue_enqueue(&window->input_events, &event);
          } else if (button_flags & RI_MOUSE_RIGHT_BUTTON_UP) {
            lwe_input_event_t event;
            event.type = LWE_INPUT_EVENT_MOUSE_BUTTON_RELEASED;
            event.mouse.button = LWE_MOUSE_BUTTON_RIGHT;
            lwe_queue_enqueue(&window->input_events, &event);
          }

          // Middle:
          if (button_flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
            lwe_input_event_t event;
            event.type = LWE_INPUT_EVENT_MOUSE_BUTTON_PRESSED;
            event.mouse.button = LWE_MOUSE_BUTTON_MIDDLE;
            lwe_queue_enqueue(&window->input_events, &event);
          } else if (button_flags & RI_MOUSE_MIDDLE_BUTTON_UP) {
            lwe_input_event_t event;
            event.type = LWE_INPUT_EVENT_MOUSE_BUTTON_RELEASED;
            event.mouse.button = LWE_MOUSE_BUTTON_MIDDLE;
            lwe_queue_enqueue(&window->input_events, &event);
          }
        } break;

        case RIM_TYPEKEYBOARD: {
          const USHORT key_code = raw->data.keyboard.VKey;
        } break;
      }
    } break;
  }

  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

static void _lwe_window_initialize()
{
  static bool have_initialized = false;

  if (have_initialized)
    return;

  WNDCLASSEXW wcx;
  memset(&wcx, 0, sizeof(WNDCLASSEX));
  wcx.cbSize        = sizeof(WNDCLASSEX);
  wcx.style         = CS_VREDRAW | CS_HREDRAW;
  wcx.lpfnWndProc   = &_lwe_window_proc;
  wcx.hInstance     = GetModuleHandle(NULL);
  wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wcx.hIcon         = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_ENGINE_ICON));
  wcx.hIconSm       = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_ENGINE_ICON));
  wcx.lpszClassName = L"lwe_window";

  lwe_fail_if(
    !RegisterClassExW(&wcx),
    "Unable to register window class `lwe_window`"
  );

  // Register for keyboard and mouse raw-input events:
  // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
  RAWINPUTDEVICE raw_input_devices[2];

  // Keyboard:
  raw_input_devices[0].usUsagePage = 0x01;
  raw_input_devices[0].usUsage = 0x06;
  raw_input_devices[0].hwndTarget = NULL;
  raw_input_devices[0].dwFlags = 0;

  // Mouse:
  raw_input_devices[1].usUsagePage = 0x01;
  raw_input_devices[1].usUsage = 0x02;
  raw_input_devices[1].hwndTarget = NULL;
  raw_input_devices[1].dwFlags = 0;

  lwe_fail_if(
    !RegisterRawInputDevices(
      &raw_input_devices[0],
      2,
      sizeof(RAWINPUTDEVICE)
    ),

    "Unable to register for raw input devices"
  );

  have_initialized = true;
}

lwe_window_t* lwe_window_open(
  lwe_const_str_t title,
  uint32_t width,
  uint32_t height )
{
  lwe_assert(title != NULL);
  lwe_assert(width > 0);
  lwe_assert(height > 0);

  lwe_win32_window_t* window =
    (lwe_win32_window_t*)lwe_alloc(sizeof(lwe_win32_window_t));

  window->width = width;
  window->height = height;
  window->fullscreen = false;

  lwe_native_str_t native_title = NULL; {
    const lwe_size_t num_bytes = lwe_to_native_str(title, -1, NULL);
    native_title = (lwe_native_str_t)lwe_stack_alloc(num_bytes);
    lwe_to_native_str(title, -1, native_title);
  }

  _lwe_window_initialize();

  // TODO: Set appropriate styles when fullscreen == true.
  const DWORD dwStyles = WS_OVERLAPPEDWINDOW;
  const DWORD dwExStyles = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

  RECT client_area = { 0, 0, width, height };
  AdjustWindowRectEx(&client_area, dwStyles, FALSE, dwExStyles);

  HWND sys_handle = CreateWindowExW(
    dwExStyles,
    L"lwe_window",
    native_title,
    dwStyles,
    0, 0,
    client_area.right - client_area.left + 1,
    client_area.bottom - client_area.top + 1,
    NULL, NULL, GetModuleHandle(NULL), NULL
  );

  SetPropA(sys_handle, "lwe_window", (HANDLE)window);
  window->sys_handle = (uintptr_t)sys_handle;
  lwe_queue_resize(&window->input_events, 256);
  lwe_queue_resize(&window->window_events, 256);
  window->mouse_x = window->mouse_y = 0;

  return window;
}

void lwe_window_set_title(
  lwe_window_t* window,
  lwe_const_str_t new_title )
{
  lwe_assert(window != NULL);
  lwe_assert(new_title != NULL);

  lwe_native_str_t native_title = NULL; {
    const lwe_size_t num_bytes = lwe_to_native_str(new_title, -1, NULL);
    native_title = (lwe_native_str_t)lwe_stack_alloc(num_bytes);
    lwe_to_native_str(new_title, -1, native_title);
  }

  SetWindowTextW((HWND)window->sys_handle, native_title);
}

void lwe_window_resize(
  lwe_window_t* window,
  uint32_t new_width,
  uint32_t new_height )
{
  lwe_assert(window != NULL);
  lwe_assert(new_width > 0);
  lwe_assert(new_height > 0);

  HWND sys_handle = (HWND)window->sys_handle;
  const DWORD dwStyles = (DWORD)GetWindowLongPtr(sys_handle, GWL_STYLE);
  const DWORD dwExStyles = (DWORD)GetWindowLongPtr(sys_handle, GWL_EXSTYLE);

  RECT client_area = { 0, 0, new_width, new_height };
  AdjustWindowRectEx(&client_area, dwStyles, FALSE, dwExStyles);

  SetWindowPos(
    sys_handle, 0, 0, 0,
    client_area.right - client_area.left + 1,
    client_area.bottom - client_area.top + 1,
    SWP_NOMOVE | SWP_NOZORDER
  );
}

void lwe_window_toggle_fullscreen(
  lwe_window_t* window,
  bool fullscreen )
{
  lwe_assert(window != NULL);
  lwe_fail("Currently not implemented.");
}

void lwe_window_show(
  lwe_window_t* window )
{
  lwe_assert(window != NULL);
  ShowWindow((HWND)window->sys_handle, SW_SHOW);
}

void lwe_window_minimize(
  lwe_window_t* window )
{
  lwe_assert(window != NULL);
  ShowWindow((HWND)window->sys_handle, SW_MINIMIZE);
}

void lwe_window_maximize(
  lwe_window_t* window )
{
  lwe_assert(window != NULL);
  ShowWindow((HWND)window->sys_handle, SW_MAXIMIZE);
}

void lwe_window_close(
  lwe_window_t* window )
{
  lwe_assert(window != NULL);

  DestroyWindow((HWND)window->sys_handle);
  lwe_queue_resize(&window->input_events, 0);
  lwe_queue_resize(&window->window_events, 0);
  lwe_free((void*)window);
}

void lwe_message_pump()
{
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}