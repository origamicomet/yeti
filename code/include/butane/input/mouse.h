// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MOUSE_H_
#define _BUTANE_MOUSE_H_

#include <butane/butane.h>
#include <butane/input/state.h>

namespace butane {
  namespace Mouse {
    enum Button {
      LEFT_BUTTON   = 0,
      MIDDLE_BUTTON = 1,
      RIGHT_BUTTON  = 2
    };

    extern BUTANE_EXPORT Input::State button(
      const Button button );

    extern BUTANE_EXPORT void set_button(
      const Button button,
      const Input::State state );
  } // Mouse
} // butane

#endif // _BUTANE_MOUSE_H_
