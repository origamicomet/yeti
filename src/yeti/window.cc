//===-- yeti/window.cc ------------------------------------*- mode: C++ -*-===//
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

namespace yeti {

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
static LRESULT WINAPI _WindowProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

Window::Window() {
  this->native_hndl_ = NULL;
  this->is_closable_ = true;
}

Window::~Window() {
}

Window *Window::open(const Window::Description &desc) {
  yeti_assert_development(desc.title != NULL);
  yeti_assert_development(desc.dimensions.width > 0);
  yeti_assert_development(desc.dimensions.width <= 65535);
  yeti_assert_development(desc.dimensions.height > 0);
  yeti_assert_development(desc.dimensions.height <= 65535);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  Window *window = new (foundation::heap()) Window();

  WNDCLASSEXW wcx;
  memset(&wcx, 0, sizeof(wcx));
  wcx.cbSize        = sizeof(WNDCLASSEX);
  // TODO(mtwilliams): Investigate if CS_OWNDC needs to be specified.
  wcx.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
  wcx.lpfnWndProc   = &_WindowProcW;
  wcx.hInstance     = ::GetModuleHandle(NULL);
  wcx.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
  // TODO(mtwilliams): Use our own icon, IDI_ENGINE_ICON?
  wcx.hIcon         = ::LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_APPLICATION));
  wcx.hIconSm       = ::LoadIconW(wcx.hInstance, MAKEINTRESOURCEW(IDI_APPLICATION));
  wcx.lpszClassName = L"875d3758-bff3-11e5-93a6-4441a4143a20";

  const bool registered_class_succesfully = (::RegisterClassExW(&wcx) != 0);
  yeti_assert(registered_class_succesfully);

  const DWORD styles = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
  const DWORD ex_styles = WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;

  WCHAR title_w[256];
  ::MultiByteToWideChar(CP_UTF8, 0, desc.title, -1, title_w, 256);

  // TODO(mtwilliams): Refactor?
  RECT client_area = { 0, 0, desc.dimensions.width, desc.dimensions.height };
  ::AdjustWindowRectEx(&client_area, styles, FALSE, ex_styles);
  const DWORD adjusted_width = client_area.right - client_area.left;
  const DWORD adjusted_height = client_area.bottom - client_area.top;

  HWND hndl = ::CreateWindowExW(ex_styles, wcx.lpszClassName, title_w, styles, 0, 0,
                                adjusted_width, adjusted_height, NULL, NULL,
                                ::GetModuleHandle(NULL), NULL);

  const bool created_windowed_succesfully = (hndl != NULL);
  yeti_assert(created_windowed_succesfully);

  // TODO(mtwilliams): Use the (global) atom table?
  const bool inserted_ref_to_inst_successfully = !!(::SetPropA(hndl, "inst", (HANDLE)window));
  yeti_assert(inserted_ref_to_inst_successfully);

  // Register for keyboard and mouse raw-input events:
  // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf for an
  // explanation of the magic values.
  RAWINPUTDEVICE raw_input_devices[2];

  // Keyboard:
  raw_input_devices[0].usUsagePage = 0x01;
  raw_input_devices[0].usUsage = 0x06;
  raw_input_devices[0].hwndTarget = hndl;
  raw_input_devices[0].dwFlags = 0;

  // Mouse:
  raw_input_devices[1].usUsagePage = 0x01;
  raw_input_devices[1].usUsage = 0x02;
  raw_input_devices[1].hwndTarget = hndl;
  raw_input_devices[1].dwFlags = 0;

  const bool registered_for_raw_input_successfully = !!(::RegisterRawInputDevices(raw_input_devices, 2, sizeof(RAWINPUTDEVICE)));
  yeti_assert(registered_for_raw_input_successfully);

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
  return window;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::update(void (*event_handler)(void *ctx), void *event_handler_ctx) {
  yeti_assert_debug(event_handler != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  MSG msg;
  while (::PeekMessage(&msg, (HWND)native_hndl_, 0, 0, PM_REMOVE)) {
    // TODO(mtwilliams): Translate and pass messages to |event_handler|.
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::show() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_SHOW);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::hide() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_HIDE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::minimize() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_MINIMIZE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::maximize() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_MAXIMIZE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

void Window::restore() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::ShowWindow((HWND)native_hndl_, SW_RESTORE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
  // opposite of AdjustWindowRect(Ex)... so we have to convert our client area
  // into screen coordinates by manually adjusting GetClientRect's ouput.
  POINT offset = {0, 0};
  ::ClientToScreen((HWND)native_hndl_, &offset);
  clipping_area.left += offset.x;
  clipping_area.right += offset.x;
  clipping_area.top += offset.y;
  clipping_area.bottom += offset.y;

  return (::ClipCursor(&clipping_area) != 0);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
  const DWORD styles = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
  const DWORD ex_styles = WS_EX_APPWINDOW | WS_EX_OVERLAPPEDWINDOW;

  RECT new_client_area = { 0, 0, new_width, new_height };
  ::AdjustWindowRectEx(&new_client_area, styles, FALSE, ex_styles);
  const DWORD adjusted_width = new_client_area.right - new_client_area.left;
  const DWORD adjusted_height = new_client_area.bottom - new_client_area.top;

  ::SetWindowPos((HWND)native_hndl_, NULL, 0, 0, adjusted_width, adjusted_height,
                 SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

bool Window::closable() const {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return is_closable_;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return false;
#endif
}

bool Window::set_closable(bool new_closable) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS || \
    YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X || \
    YETI_PLATFORM == YETI_PLATFORM_LINUX
  is_closable_ = new_closable;
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return false;
#endif
}

bool Window::keyboard_focus() const {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return (::GetFocus() == (HWND)native_hndl_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
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

  switch (uMsg) {
    case WM_GETICON: {
      // TODO(mtwilliams): Return application specific icons.
    } break;

    case WM_CLOSE: {
      // Destruction is inevitable! Sometimes?
      if (window->closable())
        ::DestroyWindow(hWnd);
    } return TRUE;

    case WM_NCDESTROY: {
      // According to MSDN, all entries in the property list of a window must
      // be removed (via RemoveProp) before it is destroyed. In practice, this
      // doesn't make any material difference--perhaps a (small) memory leak?
      ::RemovePropA(hWnd, "inst");

      // And of course, we free any memory we've associated with |hWnd|.
      foundation::heap().deallocate((uintptr_t)window);
    } return TRUE;
  }

  return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
#endif

} // yeti
