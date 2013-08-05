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

    static Vec3f _axes[3] = { Vec3f(), Vec3f(), Vec3f() };

    Vec3f axis(
      const Axis axis )
    {
      assert(axis >= X_AXIS);
      assert(axis <= Y_AXIS);

      return _axes[axis];
    }

    void set_axis(
      const Axis axis,
      const Vec3f& value )
    {
      assert(axis >= X_AXIS);
      assert(axis <= Y_AXIS);

      _axes[axis] = value;
    }
  } // Mouse
} // butane

