//===-- yeti/input/keyboard.h ---------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_INPUT_KEYBOARD_H_
#define _YETI_INPUT_KEYBOARD_H_

#include "yeti/core.h"

namespace yeti {

namespace Keys {
  enum _ {
    #define _(Id, Mnemonic, _) Mnemonic = Id,
      #include "yeti/input/keys.inl"
    #undef _

    _COUNT
  };
} typedef Keys::_ Key;

namespace Keyboard {

/// Returns whether a keyboard is connected and functioning.
extern YETI_PUBLIC bool connected();
extern YETI_PUBLIC bool disconnected();

/// Returns whether the @key is up.
extern YETI_PUBLIC bool up(const Key key);

/// Returns whether the @key is down.
extern YETI_PUBLIC bool down(const Key key);

/// Returns whether the @key was pressed this frame.
extern YETI_PUBLIC bool pressed(const Key key);

/// Returns whether the @key has been held down for more than one frame.
extern YETI_PUBLIC bool held(const Key key);

/// Returns whether the @key was released this frame.
extern YETI_PUBLIC bool released(const Key key);

/// \internal Keyboard event from platform.
struct YETI_PRIVATE Event {
  enum Type {
    // Key was pressed.
    PRESSED  = 1,
    // Key was released.
    RELEASED = 2,
  };

  Type type;

  union {
    struct { Key key; } pressed;
    struct { Key key; } released;
  };

  Event() {
    core::memory::zero((void *)this, sizeof(Event));
  }

  Event(const Event &event) {
    core::memory::copy((const void *)&event, (void *)this, sizeof(Event));
  }

  Event operator=(const Event &event) {
    core::memory::copy((const void *)&event, (void *)this, sizeof(Event));
    return *this;
  }
};

/// \internal Handles @event.
extern YETI_PRIVATE void handle(const Event event);

/// \internal
extern YETI_PRIVATE void update();

} // Keyboard

} // yeti

#endif // _YETI_INPUT_KEYBOARD_H_
