//===-- yeti/input/keyboard.cc --------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/input/keyboard.h"

namespace yeti {

namespace Keyboard {
  namespace {
    // This is essentially a rolling buffer (of bits) that tracks the state of
    // each key up to 31 frames ago. This appears needlessly complex, but will
    // allow us to track keys being pushed and released in a single frame.
    static u32 _state[Keys::_COUNT] = { 0, };
  }
}

bool Keyboard::connected() {
  // TODO(mtwilliams): Check if a keyboard is connected.
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // Refer to http://stackoverflow.com/questions/9930958.
  // As well as http://stackoverflow.com/questions/6881114.
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_IOS || \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  // HACK(mtwilliams): Assume no hardware keyboard is connected.
  return false;
#endif
}

bool Keyboard::disconnected() {
  return !connected();
}

bool Keyboard::pressed(const Key key) {
  yeti_assert_debug(key > Keys::UNKNOWN);
  yeti_assert_debug(key < Keys::_COUNT);
  const u32 prev_state = (_state[key] >> 1) & 1;
  const u32 state = _state[key] & 1;
  return (prev_state ^ state) & state;
}

bool Keyboard::held(const Key key) {
  yeti_assert_debug(key > Keys::UNKNOWN);
  yeti_assert_debug(key < Keys::_COUNT);
  const u32 prev_state = (_state[key] >> 1) & 1;
  const u32 state = _state[key] & 1;
  return (prev_state & state);
}

bool Keyboard::released(const Key key) {
  yeti_assert_debug(key > Keys::UNKNOWN);
  yeti_assert_debug(key < Keys::_COUNT);
  const u32 prev_state = (_state[key] >> 1) & 1;
  const u32 state = _state[key] & 1;
  return (prev_state ^ state) & ~state;
}

void Keyboard::up(const Key key) {
  yeti_assert_debug(key > Keys::UNKNOWN);
  yeti_assert_debug(key < Keys::_COUNT);
  _state[key] &= ~1;
}

void Keyboard::down(const Key key) {
  yeti_assert_debug(key > Keys::UNKNOWN);
  yeti_assert_debug(key < Keys::_COUNT);
  _state[key] |= 1;
}

void Keyboard::update() {
  for (u32 key = 0; key < Keys::_COUNT; ++key)
    _state[key] = (_state[key] << 1) | (_state[key] & 1);
}

} // yeti
