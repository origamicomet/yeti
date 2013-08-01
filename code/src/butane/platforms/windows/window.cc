// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/platforms/windows/window.h>

namespace butane {
namespace windows {
  static void state_to_styles(
    const bool fullscreen,
    DWORD& styles,
    DWORD& ex_styles )
  {
    if (fullscreen) {
      styles = WS_OVERLAPPED;
      ex_styles = WS_EX_APPWINDOW;
      return; }

    styles = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    ex_styles = WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;
  }

  Window::Window(
    const char* title,
    uint32_t width,
    uint32_t height,
    bool fullscreen
  ) : butane::Window(title, width, height, fullscreen)
    , _hwnd(nullptr)
  {
    Window::_initialize();
  }

  Window::~Window()
  {
    if (_hwnd)
      DestroyWindow(_hwnd);
  }

  void Window::close()
  {
    make_delete(Window, butane::Window::allocator(), this);
  }

  void Window::show()
  {
    ShowWindow(_hwnd, SW_SHOW);
  }

  void Window::hide()
  {
    ShowWindow(_hwnd, SW_HIDE);
  }

  void Window::minimize()
  {
    ShowWindow(_hwnd, SW_MINIMIZE);
  }

  void Window::maximize()
  {
    ShowWindow(_hwnd, SW_MAXIMIZE);
  }

  void Window::update()
  {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  void Window::set_title(
    const char* title )
  {
    butane::Window::set_title(title);

    assert(title != nullptr);
    wchar_t* native_title; {
      const size_t len = MultiByteToWideChar(
        CP_UTF8, 0, title, -1, nullptr, 0);
      native_title = (wchar_t*)alloca(len * sizeof(wchar_t));
      MultiByteToWideChar(
        CP_UTF8, 0, title, -1, native_title, len);
    }

    SetWindowTextW(_hwnd, native_title);
  }

  void Window::set_dimensions(
    const uint32_t width,
    const uint32_t height )
  {
    assert(width > 0);
    assert(height > 0);

    DWORD styles, ex_styles;
    state_to_styles(fullscreen(), styles, ex_styles);

    RECT client_area = { 0, 0, width, height };
    AdjustWindowRectEx(&client_area, styles, FALSE, ex_styles);

    SetWindowPos(
      _hwnd, NULL, 0, 0,
      client_area.right - client_area.left + 1,
      client_area.bottom - client_area.top + 1,
      SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
  }

  void Window::set_fullscreen(
    bool fullscreen )
  {
    butane::Window::set_fullscreen(fullscreen);

    // This isn't required?
    // DWORD styles, ex_styles;
    // state_to_styles(fullscreen, styles, ex_styles);
    // SetWindowLongPtr(_hwnd, GWL_STYLE, (LONG_PTR)styles);
    // SetWindowLongPtr(_hwnd, GWL_EXSTYLE, (LONG_PTR)ex_styles);
  }

  void Window::_initialize()
  {
    const LogScope log_scope("Window::initialize");

    static bool has_initialized = false;
    if (has_initialized)
      return;

    WNDCLASSEXW wcx;
    memset(&wcx, 0, sizeof(WNDCLASSEX));
    wcx.cbSize        = sizeof(WNDCLASSEX);
    wcx.style         = CS_VREDRAW | CS_HREDRAW;
    wcx.lpfnWndProc   = &Window::_window_proc;
    wcx.hInstance     = GetModuleHandle(NULL);
    wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcx.hIcon         = LoadIcon(wcx.hInstance, MAKEINTRESOURCE(0x3E8));
    wcx.hIconSm       = LoadIcon(wcx.hInstance, MAKEINTRESOURCE(0x3E8));
    wcx.lpszClassName = L"butane::Window";

    if (!RegisterClassExW(&wcx))
      fail("Window class registration failed! err=%#08x", GetLastError());

    // Register for keyboard and mouse raw input events:
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

    if (!RegisterRawInputDevices(&raw_input_devices[0], 2, sizeof(RAWINPUTDEVICE)))
      fail("Raw input registration failed! err=%#08x", GetLastError());

    has_initialized = true;
  }

  LRESULT WINAPI Window::_window_proc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam )
  {
    Window* window = (Window*)GetPropA(hWnd, "butane::Window");

    switch( uMsg ) {
      case WM_CLOSE: {
        if (window->_on_closed.handler)
          window->_on_closed.handler(window->_on_closed.closure, window);
        return 0;
      } break;

      case WM_SIZE: {
        if ((wParam == SIZE_MAXHIDE) || (wParam == SIZE_MAXSHOW))
          return 0;
        window->butane::Window::set_dimensions(LOWORD(lParam), HIWORD(lParam));
        return 0;
      } break;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }
} // windows
} // butane

namespace butane {
  Window* Window::open(
    const char* title,
    uint32_t width,
    uint32_t height,
    bool fullscreen )
  {
    const LogScope log_scope("Window::open");

    assert(title != nullptr);
    assert(width > 0);
    assert(height > 0);

    if (fullscreen)
      fail("Fullscreen is not supported, yet.");

    windows::Window* window =
      make_new(windows::Window, butane::Window::allocator())(title, width, height, fullscreen);

    wchar_t* native_title; {
      const size_t len = MultiByteToWideChar(
        CP_UTF8, 0, title, -1, nullptr, 0);
      native_title = (wchar_t*)alloca(len * sizeof(wchar_t));
      MultiByteToWideChar(
        CP_UTF8, 0, title, -1, native_title, len);
    }

    DWORD styles, ex_styles;
    windows::state_to_styles(fullscreen, styles, ex_styles);

    RECT client_area = { 0, 0, width, height };
    AdjustWindowRectEx(&client_area, styles, FALSE, ex_styles);

    HWND hwnd = CreateWindowExW(
      ex_styles, L"butane::Window", native_title, styles, 0, 0,
      client_area.right - client_area.left,
      client_area.bottom - client_area.top,
      NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd)
      fail("CreateWindowExW failed, err=%d", GetLastError());

    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL);
    if (!monitor)
      fail("MonitorFromWindow failed, err=%d", GetLastError());

    MONITORINFO mi;
    zero((void*)&mi, sizeof(MONITORINFO));
    mi.cbSize = sizeof(MONITORINFO);
    if (!GetMonitorInfo(monitor, &mi))
      fail("GetMonitorInfo failed, err=%d", GetLastError());

    RECT encompassing_area;
    if (!GetWindowRect(hwnd, &encompassing_area))
      fail("GetWindowRect failed, err=%d", GetLastError());

    SetWindowPos(
      hwnd,
      HWND_TOP,
      (mi.rcMonitor.right - mi.rcMonitor.left - (encompassing_area.right - encompassing_area.left - 1)) / 2 + mi.rcMonitor.left,
      (mi.rcMonitor.bottom - mi.rcMonitor.top - (encompassing_area.bottom - encompassing_area.top - 1)) / 2 + mi.rcMonitor.top ,
      0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE);

    SetPropA(hwnd, "butane::Window", (HANDLE)window);
    window->_hwnd = hwnd;

    return window;
  }
} // butane
