// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/window.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace bt {
  Window::Window()
    : _mode(Window::Mode::WINDOWED)
    , _sys_handle(nullptr)
  {
  }

  Window::~Window()
  {
    if (_sys_handle)
      DestroyWindow((HWND)_sys_handle);
  }

  static void styles( Window::Mode mode, DWORD& dwStyles, DWORD& dwExStyles ) {
    switch (mode) {
      case Window::Mode::WINDOWED: {
        dwStyles = WS_OVERLAPPEDWINDOW;
        dwExStyles = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
      } break;

      case Window::Mode::FULLSCREEN:
      case Window::Mode::FULLSCREEN_WINDOWED: {
        fail("Fullscreen and Fullscreen Windowed are not implemented, yet!\n");
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
      native_title = (const wchar_t*)alloca(len * sizeof(wchar_t));
      MultiByteToWideChar(CP_UTF8, 0, title, -1, (wchar_t*)native_title, len);
    }

    DWORD dwStyles, dwExStyles; {
      styles(Window::Mode::WINDOWED, dwStyles, dwExStyles);
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

    Window* window = make_new(Window, Allocator::heap());
    window->_title = String(Allocator::heap(), title);
    window->_width = width;
    window->_height = height;
    window->_mode = Window::Mode::WINDOWED;
    window->_sys_handle = (uintptr_t)sys_handle;
    SetPropA(sys_handle, "bt::Window", (HANDLE)window);

    return window;
  }

  void Window::close()
  {
    make_delete(Window, Allocator::heap(), this);
  }

  void Window::set_title(
    const char* title )
  {
    assert(title != nullptr);

    const wchar_t* native_title; {
      const size_t len = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
      native_title = (const wchar_t*)alloca(len * sizeof(wchar_t));
      MultiByteToWideChar(CP_UTF8, 0, title, -1, (wchar_t*)native_title, len);
    }

    _title = String(Allocator::heap(), title);
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
    Window::Mode mode )
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
} // bt