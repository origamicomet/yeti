// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/window.h>

#include <Windows.h>

namespace bt {
  Window::Window()
    : _sys_handle(nullptr)
  {
  }

  Window::~Window()
  {
    if (_sys_handle)
      DestroyWindow((HWND)_sys_handle);
  }

  static LRESULT WINAPI win32_window_proc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam )
  {
    Window* window = (Window*)GetPropA(hWnd, "bt::Window");
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }

  static void styles( WindowMode mode, DWORD& dwStyles, DWORD& dwExStyles ) {
    switch (mode) {
      case BT_WINDOWED: {
        dwStyles = WS_OVERLAPPEDWINDOW;
        dwExStyles = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      } break;

      case BT_FULLSCREEN: {
        dwStyles = WS_OVERLAPPEDWINDOW;
        dwExStyles = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      } break;

      case BT_FULLSCREEN_WINDOWED: {
        dwStyles = WS_OVERLAPPEDWINDOW;
        dwExStyles = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      } break;
    }
  }

  Window* Window::open(
    const char* title,
    unsigned width,
    unsigned height )
  {
    assert(title != nullptr);
    assert(width > 0);
    assert(height > 0);

    const wchar_t* native_title; {
      const size_t len = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
      native_title = (wchar_t*)alloca(len * sizeof(wchar_t));
      MultiByteToWideChar(CP_UTF8, 0, title, -1, native_title, len);
    }

    DWORD dwStyles, dwExStyles; {
      styles(BT_WINDOWED, dwStyles, dwExStyles);
    }

    RECT client_area = { 0, 0, width, height };
    AdjustWindowRectEx(&client_area, dwStyles, FALSE, dwExStyles);

    HWND sys_handle = CreateWindowExW(
      dwExStyles,
      L"Butane",
      native_title,
      dwStyles,
      0, 0,
      client_area.right - client_area.left + 1,
      client_area.bottom - client_area.top + 1,
      NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!sys_handle)
      return nullptr;

    SetPropA(sys_handle, "bt::Window", (HANDLE)this);

    Window* window = MAKE_NEW(Window, Allocator::heap());
    window->_title = title;
    window->_width = width;
    window->_height = height;
    window->_mode = BT_WINDOWED;
    window->_sys_handle = (uintptr_t)sys_handle;

    return window;
  }

  void Window::close()
  {
    MAKE_DELETE(Window, Allocator::heap(), this);
  }

  void Window::set_title(
    const char* title )
  {
    assert(title != nullptr);

    const wchar_t* native_title; {
      const size_t len = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
      native_title = (wchar_t*)alloca(len * sizeof(wchar_t));
      MultiByteToWideChar(CP_UTF8, 0, title, -1, native_title, len);
    }

    _title = title;
    SetWindowTextW((HWND)_sys_handle, native_title);
  }

  void Window::set_dimensions(
    unsigned width,
    unsigned height )
  {
    assert(width > 0);
    assert(height > 0);

    const DWORD dwStyles = (DWORD)GetWindowLongPtr((HWND)_sys_handle, GWL_STYLE);
    const DWORD dwExStyles = (DWORD)GetWindowLongPtr((HWND)_sys_handle, GWL_EXSTYLE);

    RECT client_area = { 0, 0, width, height };
    AdjustWindowRectEx(&client_area, dwStyles, FALSE, dwExStyles);

    SetWindowPos(
      (HWND)_sys_handle, 0, 0, 0,
      client_area.right - client_area.left + 1,
      client_area.bottom - client_area.top + 1,
      SWP_NOMOVE | SWP_NOZORDER);
  }

  void Window::set_window_mode(
    WindowMode mode )
  {
    fail("Window::set_window_mode is not implemented, yet!\n");
  }

  void Window::show()
  {
    ShowWindow((HWND)_sys_handle, SW_SHOW);
  }

  void Window::hide()
  {
    ShowWindow((HWND)_sys_handle, SW_HIDE);
  }

  void Window::minimize()
  {
    ShowWindow((HWND)_sys_handle, SW_MINIMIZE);
  }

  void Window::maximize()
  {
    ShowWindow((HWND)_sys_handle, SW_MAXIMIZE);
  }

  namespace Windows {
    void initialize()
    {
      WNDCLASSEXW wcx;
      zero((void*)&wcx, sizeof(WNDCLASSEX));
      wcx.cbSize        = sizeof(WNDCLASSEX);
      wcx.style         = CS_VREDRAW | CS_HREDRAW;
      wcx.lpfnWndProc   = &win32_window_proc;
      wcx.hInstance     = GetModuleHandle(NULL);
      wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
      wcx.hIcon         = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_ENGINE_ICON));
      wcx.hIconSm       = LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_ENGINE_ICON));
      wcx.lpszClassName = L"Butane";

      if (!RegisterClassExW(&wcx))
        fail("Unable to register WndClassEx `Butane`!\n");

      // Register for keyboard and mouse raw-input events:
      // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
      RAWINPUTDEVICE rids[2];

      // Keyboard:
      rids[0].usUsagePage = 0x01;
      rids[0].usUsage = 0x06;
      rids[0].hwndTarget = NULL;
      rids[0].dwFlags = 0;

      // Mouse:
      rids[1].usUsagePage = 0x01;
      rids[1].usUsage = 0x02;
      rids[1].hwndTarget = NULL;
      rids[1].dwFlags = 0;

      if (!RegisterRawInputDevices(&rids[0], 2, sizeof(RAWINPUTDEVICE)))
        fail("Unable to register RawInput devices!\n");
    }

    void update()
    {
      MSG msg;
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  } // Windows
} // bt