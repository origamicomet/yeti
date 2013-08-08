// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_KEYBOARD_H_
#define _BUTANE_KEYBOARD_H_

#include <butane/butane.h>
#include <butane/input/state.h>

namespace butane {
  namespace Keyboard {
    typedef uint32_t Button;

    extern BUTANE_EXPORT Button name(
      const char* name );

    extern BUTANE_EXPORT Input::State button(
      const Button button );

    extern BUTANE_EXPORT void set_button(
      const Button button,
      const Input::State state );
  } // Keyboard
} // butane

#endif // _BUTANE_KEYBOARD_H_
