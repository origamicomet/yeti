// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/input/mouse.h>

namespace butane {
  namespace Mouse {
    static Input::State _buttons[3] = { Input::UP, Input::UP, Input::UP };

    Input::State button(
      const Button button )
    {
      assert(button >= LEFT_BUTTON);
      assert(button <= RIGHT_BUTTON);

      return _buttons[button];
    }

    void set_button(
      const Button button,
      const Input::State state )
    {
      assert(button >= LEFT_BUTTON);
      assert(button <= RIGHT_BUTTON);
      assert((state == Input::UP) || (state == Input::DOWN));

      _buttons[button] = state;
    }
  } // Mouse
} // butane

