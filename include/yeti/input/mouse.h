//===-- yeti/input/mouse.h --------------------------------*- mode: C++ -*-===//
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

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

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

/// Returns wether a mouse is connected and functioning.
extern YETI_PUBLIC bool connected();
extern YETI_PUBLIC bool disconnected();

/// Returns the value of the @axis this frame.
extern YETI_PUBLIC Vec3 axis(const MouseAxis axis);

/// Returns wether the @btn was pressed this frame.
extern YETI_PUBLIC bool pressed(const MouseButton btn);

/// Returns wether the @btn has been held down for more than one frame.
extern YETI_PUBLIC bool held(const MouseButton btn);

/// Returns wether the @btn was released this frame.
extern YETI_PUBLIC bool released(const MouseButton btn);

/// \internal Updates the @axis for this frame.
extern YETI_LOCAL void update(const MouseAxis axis, const Vec3 &new_value);

/// \internal Marks the @btn as released for this frame.
extern YETI_LOCAL void up(const MouseButton btn);

/// \internal Marks the @btn as pressed for this frame.
extern YETI_LOCAL void down(const MouseButton btn);

/// \internal
extern YETI_LOCAL void update();

} // Keyboard

} // yeti

#endif // _YETI_INPUT_MOUSE_H_
