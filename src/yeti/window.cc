//===-- yeti/window.cc ----------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/window.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

namespace yeti {

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  static LRESULT WINAPI _WindowProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

Window::Window() {
  this->native_hndl_ = NULL;
  this->is_closable_ = true;
  this->is_resizeable_ = true;
}

Window::~Window() {
}

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  static void compensate_for_chrome(const DWORD styles,
                                    const DWORD ex_styles,
                                    u32 *width,
                                    u32 *height)
  {
    RECT area = { 0, 0, *width, *height };

    ::AdjustWindowRectEx(&area, styles, FALSE, ex_styles);

    *width  = area.right - area.left;
    *height = area.bottom - area.top;
  }
#endif

Window *Window::open(const Window::Description &desc) {
  yeti_assert_development(desc.title != NULL);

  yeti_assert_development(desc.dimensions.width > 0);
  yeti_assert_development(desc.dimensions.width <= 65535);

  yeti_assert_development(desc.dimensions.height > 0);
  yeti_assert_development(desc.dimensions.height <= 65535);

  Window *window = YETI_NEW(Window, core::global_heap_allocator());

  window->is_resizeable_ = desc.resizeable;
  window->is_closable_ = desc.closable;

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // TODO(mtwilliams): Use our own icon, IDI_ENGINE_ICON?
  WNDCLASSEXW wcx;
  memset(&wcx, 0, sizeof(wcx));
  wcx.cbSize        = sizeof(WNDCLASSEX);
  wcx.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
  wcx.lpfnWndProc   = &_WindowProcW;
  wcx.hInstance     = ::GetModuleHandle(NULL);
  wcx.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
  wcx.hIcon         = (HICON)::LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_APPLICATION));
  wcx.hIconSm       = (HICON)::LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_APPLICATION));
  wcx.lpszClassName = L"875d3758-bff3-11e5-93a6-4441a4143a20";

  // NOTE(mtwilliams): This is not thread-safe. Not that any of this stuff is.
  static const bool registered_class_succesfully = (::RegisterClassExW(&wcx) != 0);
  yeti_assert(registered_class_succesfully);

  DWORD styles = WS_CAPTION
               | WS_BORDER
               | WS_CLIPCHILDREN
               | WS_CLIPSIBLINGS;

  if (desc.resizeable)
    styles |= (WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX);
  else
    styles |= (WS_SYSMENU);

  DWORD ex_styles = WS_EX_APPWINDOW;

  WCHAR title_w[256];
  ::MultiByteToWideChar(CP_UTF8, 0, desc.title, -1, title_w, 256);

  u32 width = desc.dimensions.width;
  u32 height = desc.dimensions.height;

  compensate_for_chrome(styles, ex_styles, &width, &height);

  HWND hndl = ::CreateWindowExW(ex_styles,
                                wcx.lpszClassName,
                                &title_w[0],
                                styles,
                                0, 0, width, height,
                                NULL,
                                NULL,
                                ::GetModuleHandle(NULL),
                                (LPVOID)window);

  const bool created_windowed_succesfully = (hndl != NULL);
  yeti_assert(created_windowed_succesfully);

  // HACK(mtwilliams): Center the window on the encompassing monitor.
  HMONITOR encompassing_monitor = ::MonitorFromWindow(hndl, MONITOR_DEFAULTTONULL);
  MONITORINFO encompassing_monitor_info;
  ::memset((void *)&encompassing_monitor_info, 0, sizeof(MONITORINFO));
  encompassing_monitor_info.cbSize = sizeof(MONITORINFO);
  ::GetMonitorInfo(encompassing_monitor, &encompassing_monitor_info);
  RECT encompassing_area;
  ::GetWindowRect(hndl, &encompassing_area);
  const DWORD monitor_width = encompassing_monitor_info.rcMonitor.right - encompassing_monitor_info.rcMonitor.left;
  const DWORD monitor_height = encompassing_monitor_info.rcMonitor.bottom - encompassing_monitor_info.rcMonitor.top;
  const DWORD encompassing_width = encompassing_area.right - encompassing_area.left - 1;
  const DWORD encompassing_height = encompassing_area.bottom - encompassing_area.top - 1;
  const DWORD centered_pos_x = (monitor_width - encompassing_width) / 2 + encompassing_monitor_info.rcMonitor.left;
  const DWORD centered_pos_y = (monitor_height - encompassing_height) / 2 + encompassing_monitor_info.rcMonitor.top;
  ::SetWindowPos(hndl, HWND_TOP, centered_pos_x, centered_pos_y, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE);

  window->native_hndl_ = (uintptr_t)hndl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
  return window;
}

void Window::update(EventHandler event_handler,
                    void *event_handler_ctx) {
  yeti_assert_debug(event_handler != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::SetPropA((HWND)native_hndl_, "event_handler", (HANDLE)event_handler);
  ::SetPropA((HWND)native_hndl_, "event_handler_ctx", (HANDLE)event_handler_ctx);

  MSG msg;
  while (::PeekMessage(&msg, (HWND)native_hndl_, 0, 0, PM_REMOVE)) {
    // TODO(mtwilliams): Translate and pass messages to |event_handler|.
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::close() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // We destroy the window in WM_CLOSE (and free it in WM_NCDESTROY) so that we
  // don't have to handle programmatic window closes differently. This
  // introduces some complexity, notably with multi-threaded rendering, but
  // reduces the complexity of window management for users, specifically
  // when embedding engine windows inside tools.
  ::CloseWindow((HWND)native_hndl_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::show() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_SHOW);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::hide() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_HIDE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::minimize() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_MINIMIZE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::maximize() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_MAXIMIZE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::restore() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_RESTORE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

bool Window::clip() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  RECT clipping_area = { 0, };
  ::GetClientRect((HWND)native_hndl_, &clipping_area);

  // We want to clip inside the system chrome, which in Microsoft terminology
  // means the "client area." Of course they fail to provide an API that's the
  // opposite of AdjustWindowRect(Ex). So we have to convert our client area
  // into screen coordinates by manually adjusting GetClientRect output.
  POINT offset = {0, 0};
  ::ClientToScreen((HWND)native_hndl_, &offset);
  clipping_area.left += offset.x;
  clipping_area.right += offset.x;
  clipping_area.top += offset.y;
  clipping_area.bottom += offset.y;

  return (::ClipCursor(&clipping_area) != 0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  // TODO(mtwilliams): Use XGrabPointer.
  // Refer to http://stackoverflow.com/questions/4642867.
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return false;
#endif
}

bool Window::unclip() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ClipCursor(NULL);
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return false;
#endif
}

void Window::title(char title[256]) const {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  WCHAR title_w[256];
  ::GetWindowTextW((HWND)native_hndl_, (LPWSTR)title_w, 256);
  ::WideCharToMultiByte(CP_UTF8, 0, title_w, -1, (LPSTR)title, 256, NULL, NULL);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::set_title(const char *new_title) {
  yeti_assert(strlen(new_title) <= 255);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  WCHAR new_title_w[256];
  ::MultiByteToWideChar(CP_UTF8, 0, new_title, -1, new_title_w, 256);
  ::SetWindowTextW((HWND)native_hndl_, new_title_w);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::dimensions(u32 *width, u32 *height) const {
  yeti_assert_debug(width != NULL);
  yeti_assert_debug(height != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  RECT client_area;
  ::GetClientRect((HWND)native_hndl_, &client_area);
  *width = client_area.right - client_area.left;
  *height = client_area.bottom - client_area.top;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::resize(u32 new_width, u32 new_height) {
  yeti_assert_development(new_width > 0);
  yeti_assert_development(new_width <= 65535);

  yeti_assert_development(new_height > 0);
  yeti_assert_development(new_height <= 65535);

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD styles = ::GetWindowLong((HWND)native_hndl_, GWL_STYLE);
  const DWORD ex_styles = ::GetWindowLong((HWND)native_hndl_, GWL_EXSTYLE);

  u32 width = new_width;
  u32 height = new_height;

  compensate_for_chrome(styles, ex_styles, &width, &height);

  ::SetWindowPos((HWND)native_hndl_,
                 NULL,
                 0, 0, width, height,
                 SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

bool Window::closable() const {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return is_closable_;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return false;
#endif
}

void Window::set_closable(bool closable) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const HMENU menu = ::GetSystemMenu((HWND)native_hndl_, FALSE);

  if (is_closable_ = closable) {
    ::EnableMenuItem(menu, SC_CLOSE, MF_BYCOMMAND);
  } else {
    ::EnableMenuItem(menu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

bool Window::keyboard_focus() const {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (::GetFocus() == (HWND)native_hndl_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

bool Window::set_keyboard_focus(bool new_keyboard_focus) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (::SetFocus(new_keyboard_focus ? (HWND)native_hndl_ : NULL) != NULL);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return false;
#endif
}

bool Window::mouse_focus() const {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // HACK(mtwilliams): Assume we have mouse focus.
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

bool Window::set_mouse_focus(bool new_mouse_focus) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // HACK(mtwilliams): Assume we have mouse focus.
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return false;
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return false;
#endif
}

uintptr_t Window::to_native_hndl() const {
  return this->native_hndl_;
}

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
static LRESULT WINAPI _WindowProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  Window *window = (Window *)::GetPropA(hWnd, "inst");

  Window::EventHandler event_handler = (Window::EventHandler)::GetPropA(hWnd, "event_handler");
  void *event_handler_ctx = (void *)::GetPropA(hWnd, "event_handler_ctx");

  switch (uMsg) {
    case WM_GETICON: {
      // TODO(mtwilliams): Return application specific icons.
    } break;

    case WM_CREATE: {
      const CREATESTRUCT *cs = (CREATESTRUCT *)lParam;

      window = (Window *)cs->lpCreateParams;

      // Store property so we can recover `Window *` from handle.
      ::SetPropA(hWnd, "inst", (HANDLE)window);

      if (!window->closable()) {
        ::EnableMenuItem(::GetSystemMenu(hWnd, FALSE),
                         SC_CLOSE,
                         MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
      }
    } return 0;

    case WM_CLOSE: {
      Window::Event event;
      event.type = Window::Event::CLOSED;
      event_handler(window, event, event_handler_ctx);

      // Destruction is inevitable!
      ::DestroyWindow(hWnd);
    } return 0;

    case WM_NCDESTROY: {
      // According to MSDN, all entries in the property list of a window must
      // be removed (via RemoveProp) before it is destroyed. In practice, this
      // doesn't make any material difference. Perhaps a (small) memory leak?
      ::RemovePropA(hWnd, "inst");
      ::RemovePropA(hWnd, "event_handler");
      ::RemovePropA(hWnd, "event_handler_ctx");

      // And of course, we free any memory we've associated with |hWnd|.
      core::global_heap_allocator().deallocate((void *)window);
    } return 0;

    case WM_ERASEBKGND: {
      // Prevent flickering.
    } return 1;

    case WM_PAINT: {
      ::ValidateRect(hWnd, NULL);
    } return 0;
  }

  return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
#endif

} // yeti
