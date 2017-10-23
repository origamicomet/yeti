//===-- yeti/input/feeder.cc ----------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/input/feeder.h"

#include "yeti/input/keyboard.h"
#include "yeti/input/mouse.h"

#include "yeti/window.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
  #include <windowsx.h>

  #undef ABSOLUTE
  #undef RELATIVE
  #undef DELETE

  // HACK(mtwilliams): Define `RI_MOUSE_HWHEEL` until we specify build target
  // correctly.
  #define RI_MOUSE_HWHEEL 0x0800
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

namespace yeti {

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS

static void on_raw_keyboard_input(RAWKEYBOARD kb) {
  if (kb.VKey == 0xFF)
    // Part of an escape sequence; ignore.
    return;

  if (kb.VKey == VK_SHIFT) {
    // Map to left or right variants.
    kb.VKey = ::MapVirtualKey(kb.MakeCode, MAPVK_VSC_TO_VK_EX);
  } else if (kb.VKey == VK_NUMLOCK) {
    // For some reason, the "Numlock" key shares the same scan-code as
    // the Pause/Break key, so we have to fix it manually.
    kb.MakeCode = (::MapVirtualKey(kb.VKey, MAPVK_VK_TO_VSC) | 0x100);
  }

  const bool E0 = !!(kb.Flags & RI_KEY_E0);
  const bool E1 = !!(kb.Flags & RI_KEY_E1);

  if (E1) {
    if (kb.VKey == VK_PAUSE)
      // Manually map VK_PAUSE because MapVirtualKey does so incorrectly.
      kb.MakeCode = 0x45;
    else
      kb.MakeCode = ::MapVirtualKey(kb.VKey, MAPVK_VK_TO_VSC);
  }

  Key key = Keys::UNKNOWN;
  switch (kb.VKey) {
    case VK_ESCAPE:
      key = Keys::ESCAPE;
      break;
    case VK_TAB:
      key = Keys::TAB;
      break;
    case VK_SHIFT:
    case VK_LSHIFT:
    case VK_RSHIFT:
      // TODO(mtwilliams): Expose Keys::LEFT_SHIFT and Keys::RIGHT_SHIFT?
      key = Keys::SHIFT;
      break;
    case VK_CAPITAL:
      key = Keys::CAPSLOCK;
      break;
    case VK_RETURN:
      key = E0 ? Keys::NUMPAD_ENTER : Keys::ENTER;
      break;
    case VK_SPACE:
      key = Keys::SPACE;
      break;
    case VK_BACK:
      key = Keys::BACKSPACE;
      break;
    case VK_CONTROL:
      key = E0 ? Keys::RIGHT_CONTROL : Keys::LEFT_CONTROL;
      break;
    case VK_MENU:
      key = E0 ? Keys::RIGHT_ALT : Keys::LEFT_ALT;
      break;
    case VK_LWIN:
      key = Keys::LEFT_SUPER;
      break;
    case VK_RWIN:
      key = Keys::RIGHT_SUPER;
      break;
    case VK_NUMLOCK:
      key = Keys::NUMLOCK;
      break;
    case VK_HOME:
      key = E0 ? Keys::HOME : Keys::NUMPAD_7;
      break;
    case VK_PRIOR:
      key = E0 ? Keys::PAGE_UP : Keys::NUMPAD_9;
      break;
    case VK_NEXT:
      key = E0 ? Keys::PAGE_DOWN : Keys::NUMPAD_3;
      break;
    case VK_END:
      key = E0 ? Keys::END : Keys::NUMPAD_1;
      break;
    case VK_INSERT:
      key = E0 ? Keys::INSERT : Keys::NUMPAD_0;
      break;
    case VK_DELETE:
      key = E0 ? Keys::DELETE : Keys::NUMPAD_DELETE;
      break;
    case VK_UP:
      key = E0 ? Keys::UP : Keys::NUMPAD_8;
      break;
    case VK_DOWN:
      key = E0 ? Keys::DOWN : Keys::NUMPAD_2;
      break;
    case VK_LEFT:
      key = E0 ? Keys::LEFT : Keys::NUMPAD_4;
      break;
    case VK_RIGHT:
      key = E0 ? Keys::RIGHT : Keys::NUMPAD_6;
      break;
    case VK_CLEAR:
      // TODO(mtwilliams): Expose a NUMPAD_CLEAR?
      key = E0 ? Keys::UNKNOWN : Keys::NUMPAD_5;
      break;
    case VK_ADD:
      key = Keys::NUMPAD_ADD;
      break;
    case VK_SUBTRACT:
      key = Keys::NUMPAD_SUBTRACT;
      break;
    case VK_MULTIPLY:
      key = Keys::NUMPAD_MULTIPLY;
      break;
    case VK_DIVIDE:
      key = Keys::NUMPAD_DIVIDE;
      break;
    default: {
      if (kb.VKey >= '0' && kb.VKey <= '9')
        key = (Key)((kb.VKey - '0') + Keys::NUMBER_0);
      else if (kb.VKey >= 'A' && kb.VKey <= 'Z')
        key = (Key)((kb.VKey - 'A') + Keys::A);
      else if (kb.VKey >= VK_F1 && kb.VKey <= VK_F12)
        key = (Key)((kb.VKey - VK_F1) + Keys::F1);
      else if (kb.VKey >= VK_NUMPAD0 && kb.VKey <= VK_NUMPAD9)
        // TODO(mtwilliams): Determine if this is redundant.
        key = (Key)((kb.VKey - VK_NUMPAD0) + Keys::NUMPAD_0);
      else
        // Some other (rare) key. Ignore.
        return;
    } break;
  }

  if (kb.Flags & RI_KEY_BREAK) {
    Keyboard::Event event;
    event.type = Keyboard::Event::RELEASED;
    event.released.key = key;
    Keyboard::handle(event);
  } else {
    Keyboard::Event event;
    event.type = Keyboard::Event::PRESSED;
    event.pressed.key = key;
    Keyboard::handle(event);
  }
}

static void on_mouse_moved(const MouseAxis axis, const Vec2 &value) {
  Mouse::Event event;

  event.type = Mouse::Event::MOVED;
  event.moved.axis = axis;
  event.moved.value = value;

  Mouse::handle(event);
}

static void on_raw_mouse_input(RAWMOUSE mouse) {
  static const struct { USHORT up, down; MouseButton btn; } button_mappings[MouseButtons::_COUNT] = {
    { RI_MOUSE_LEFT_BUTTON_UP,   RI_MOUSE_LEFT_BUTTON_DOWN,   MouseButtons::LEFT    },
    { RI_MOUSE_MIDDLE_BUTTON_UP, RI_MOUSE_MIDDLE_BUTTON_DOWN, MouseButtons::MIDDLE  },
    { RI_MOUSE_RIGHT_BUTTON_UP,  RI_MOUSE_RIGHT_BUTTON_DOWN,  MouseButtons::RIGHT   },
    { RI_MOUSE_BUTTON_4_UP,      RI_MOUSE_BUTTON_4_DOWN,      MouseButtons::EXTRA_1 },
    { RI_MOUSE_BUTTON_5_UP,      RI_MOUSE_BUTTON_5_DOWN,      MouseButtons::EXTRA_2 }
  };

  for (u32 btn = 0; btn < MouseButtons::_COUNT; ++btn) {
    if (mouse.usButtonFlags & button_mappings[btn].up) {
      Mouse::Event event;
      event.type = Mouse::Event::RELEASED;
      event.released.button = button_mappings[btn].btn;
      Mouse::handle(event);
    }

    if (mouse.usButtonFlags & button_mappings[btn].down) {
      Mouse::Event event;
      event.type = Mouse::Event::PRESSED;
      event.pressed.button = button_mappings[btn].btn;
      Mouse::handle(event);
    }
  }

  Vec2 delta = Vec2::ZERO;
  delta.x = (f32)mouse.lLastX;
  delta.y = (f32)mouse.lLastY;

  Vec2 wheel = Vec2::ZERO;
  if (mouse.usButtonFlags & RI_MOUSE_WHEEL)
    wheel.y = -(f32)((short)mouse.usButtonData);
  if (mouse.usButtonFlags & RI_MOUSE_HWHEEL)
    wheel.x = (f32)((short)mouse.usButtonData);
  wheel = wheel / (f32)WHEEL_DELTA;

  on_mouse_moved(MouseAxes::DELTA, delta);
  on_mouse_moved(MouseAxes::WHEEL, wheel);
}

static LRESULT WINAPI _WindowProcW(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  const WNDPROC original_window_proc = (WNDPROC)::GetPropA(hWnd, "orginal_window_proc");

  switch (uMsg) {
    case WM_INPUT_DEVICE_CHANGE: {
      // TODO(mtwilliams): Handle device connections and disconnections.
    } break;

    case WM_INPUTLANGCHANGE: {
      // TODO(mtwilliams): Handle different keyboard layouts.
    } break;

    case WM_INPUT: {
      RAWINPUT raw_input;
      UINT raw_input_sz = sizeof(RAWINPUT);

      ::GetRawInputData((HRAWINPUT)lParam,
                        RID_INPUT,
                        (LPVOID)&raw_input,
                        &raw_input_sz,
                        sizeof(RAWINPUTHEADER));

      if (raw_input.header.dwType == RIM_TYPEKEYBOARD)
        on_raw_keyboard_input(raw_input.data.keyboard);
      else if (raw_input.header.dwType == RIM_TYPEMOUSE)
        on_raw_mouse_input(raw_input.data.mouse);
    } break;

    case WM_MOUSEMOVE: {
      POINT absolute;
      ::GetCursorPos(&absolute);

      POINT relative;
      relative.x = GET_X_LPARAM(lParam);
      relative.y = GET_Y_LPARAM(lParam);

      on_mouse_moved(MouseAxes::ABSOLUTE, Vec2((f32)absolute.x, (f32)absolute.y));
      on_mouse_moved(MouseAxes::RELATIVE, Vec2((f32)relative.x, (f32)relative.y));
    } break;

    case WM_NCDESTROY: {
      // According to MSDN, all entries in the property list of a window must
      // be removed (via RemoveProp) before it is destroyed. In practice, this
      // doesn't make any material difference. Perhaps a (small) memory leak?
      ::RemovePropA(hWnd, "orginal_window_proc");
    } break;
  }

  return ::CallWindowProcW(original_window_proc, hWnd, uMsg, wParam, lParam);
}

void input::from(const Window *window) {
  yeti_assert_debug(window != NULL);

  const HWND hndl = (HWND)window->to_native_hndl();

  // Register for raw-input events from keyboards and mice. Also register for
  // device connection and disconnection events.
  //
  // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf for an
  // explanation of the magic values.
  //
  RAWINPUTDEVICE raw_input_devices[2];

  // Keyboard:
  raw_input_devices[0].usUsagePage = 0x01;
  raw_input_devices[0].usUsage     = 0x06;
  raw_input_devices[0].hwndTarget  = hndl;
  raw_input_devices[0].dwFlags     = RIDEV_DEVNOTIFY;

  // Mouse:
  raw_input_devices[1].usUsagePage = 0x01;
  raw_input_devices[1].usUsage     = 0x02;
  raw_input_devices[1].hwndTarget  = hndl;
  raw_input_devices[1].dwFlags     = RIDEV_DEVNOTIFY;

  ::RegisterRawInputDevices(raw_input_devices, 2, sizeof(RAWINPUTDEVICE));

  // TODO(mtwilliams): Determine if Windows caches sub-classes, i.e. does
  // something akin to find-or-create.
  LONG_PTR orginal_window_proc =
    ::SetWindowLongPtrW(hndl, GWLP_WNDPROC, (LONG_PTR)&_WindowProcW);

  ::SetPropA(hndl, "orginal_window_proc", (HANDLE)orginal_window_proc);
}

#elif YETI_PLATFORM == YETI_PLATFORM_MAC

void input::from(const Window *window) {
  yeti_assert_debug(window != NULL);
}

#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

} // yeti
