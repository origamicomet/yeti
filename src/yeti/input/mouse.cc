//===-- yeti/input/mouse.cc -----------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/input/mouse.h"

namespace yeti {

namespace Mouse {
  namespace {
    // Refer to src/yeti/input/keyboard.cc for the reasoning behind this.
    static u32 _state[MouseButtons::_COUNT] = { 0, };
    static Vec3 _axes[MouseAxes::_COUNT] = { Vec3::ZERO, };
  }
}

bool Mouse::connected() {
  // TODO(mtwilliams): Check if a mouse is connected.
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  // HACK(mtwilliams): Assume no mouse is connected.
  return false;
#endif
}

bool Mouse::disconnected() {
  return !connected();
}

Vec3 Mouse::axis(const MouseAxis axis) {
  yeti_assert_debug(axis > MouseAxes::UNKNOWN);
  yeti_assert_debug(axis < MouseAxes::_COUNT);
  return _axes[axis];
}

bool Mouse::up(const MouseButton btn) {
  yeti_assert_debug(btn > MouseButtons::UNKNOWN);
  yeti_assert_debug(btn < MouseButtons::_COUNT);
  return (~_state[btn] & 1);
}

bool Mouse::down(const MouseButton btn) {
  yeti_assert_debug(btn > MouseButtons::UNKNOWN);
  yeti_assert_debug(btn < MouseButtons::_COUNT);
  return (_state[btn] & 1);
}

bool Mouse::pressed(const MouseButton btn) {
  yeti_assert_debug(btn > MouseButtons::UNKNOWN);
  yeti_assert_debug(btn < MouseButtons::_COUNT);
  const u32 prev_state = (_state[btn] >> 1) & 1;
  const u32 state = _state[btn] & 1;
  return (prev_state ^ state) & state;
}

bool Mouse::held(const MouseButton btn) {
  yeti_assert_debug(btn > MouseButtons::UNKNOWN);
  yeti_assert_debug(btn < MouseButtons::_COUNT);
  const u32 prev_state = (_state[btn] >> 1) & 1;
  const u32 state = _state[btn] & 1;
  return (prev_state & state);
}

bool Mouse::released(const MouseButton btn) {
  yeti_assert_debug(btn > MouseButtons::UNKNOWN);
  yeti_assert_debug(btn < MouseButtons::_COUNT);
  const u32 prev_state = (_state[btn] >> 1) & 1;
  const u32 state = _state[btn] & 1;
  return (prev_state ^ state) & ~state;
}

void Mouse::handle(const Event event) {
  switch (event.type) {
    case Event::PRESSED:
      _state[event.pressed.button] |= 1;
      break;

    case Event::RELEASED:
      _state[event.released.button] &= ~1;
      break;

    case Event::MOVED:
      switch (event.moved.axis) {
        case MouseAxes::ABSOLUTE:
        case MouseAxes::RELATIVE:
          _axes[event.moved.axis] = event.moved.value;
          break;

        case MouseAxes::DELTA:
        case MouseAxes::WHEEL:
          _axes[event.moved.axis] = _axes[event.moved.axis] + event.moved.value;
          break;
      }

      break;
  }
}

void Mouse::update() {
  // Assume state is unchanged.
  for (u32 btn = 0; btn < MouseButtons::_COUNT; ++btn)
    _state[btn] = (_state[btn] << 1) | (_state[btn] & 1);

  // New frame, so reset relative axes.
  _axes[MouseAxes::DELTA] = Vec3::ZERO;
  _axes[MouseAxes::WHEEL] = Vec3::ZERO;
}

} // yeti
