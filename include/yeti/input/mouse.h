//===-- yeti/input/mouse.h ------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_INPUT_MOUSE_H_
#define _YETI_INPUT_MOUSE_H_

#include "yeti/core.h"

#include "yeti/math.h"

namespace yeti {

namespace MouseButtons {
  enum _ {
    UNKNOWN = 0,

    LEFT    = 1,
    MIDDLE  = 2,
    RIGHT   = 3,

    EXTRA_1 = 4,
    EXTRA_2 = 5,

    _COUNT
  };
} typedef MouseButtons::_ MouseButton;

namespace MouseAxes {
  enum _ {
    UNKNOWN  = 0,

    ABSOLUTE = 1,
    RELATIVE = 2,
    DELTA    = 3,
    WHEEL    = 4,

    _COUNT
  };
} typedef MouseAxes::_ MouseAxis;

namespace Mouse {

/// Returns whether a mouse is connected and functioning.
extern YETI_PUBLIC bool connected();
extern YETI_PUBLIC bool disconnected();

/// Returns the value of the @axis this frame.
extern YETI_PUBLIC Vec2 axis(const MouseAxis axis);

/// Returns whether the @btn is up.
extern YETI_PUBLIC bool up(const MouseButton btn);

/// Returns whether the @btn is down.
extern YETI_PUBLIC bool down(const MouseButton btn);

/// Returns whether the @btn was pressed this frame.
extern YETI_PUBLIC bool pressed(const MouseButton btn);

/// Returns whether the @btn has been held down for more than one frame.
extern YETI_PUBLIC bool held(const MouseButton btn);

/// Returns whether the @btn was released this frame.
extern YETI_PUBLIC bool released(const MouseButton btn);

/// \internal Mouse event from platform.
struct YETI_PRIVATE Event {
  enum Type {
    // Button was pressed.
    PRESSED  = 1,
    // Button was released.
    RELEASED = 2,
    // Mouse was moved.
    MOVED    = 3,
  };

  Type type;

  union {
    struct { MouseButton button; } pressed;
    struct { MouseButton button; } released;
    struct { MouseAxis axis; Vec2 value; } moved;
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

#endif // _YETI_INPUT_MOUSE_H_
