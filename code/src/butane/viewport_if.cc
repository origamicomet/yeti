// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/viewport_if.h>
#include <butane/graphics/viewport.h>

namespace butane {
namespace script_interface {
  namespace Viewport {
    static size_t create(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments != 4)
        script.error("Expected four arguments (top : Number, left : Number, bottom : Number, right : Number).");
      int top, left, bottom, right;
      arguments.to(0, top);
      arguments.to(1, left);
      arguments.to(2, bottom);
      arguments.to(3, right);
      if ((top < 0) || (left < 0) || (bottom < 0) || (right < 0))
        script.error("Expected top, left, bottom, and right to be non-negative.");
      butane::Viewport* viewport =
        make_new(butane::Viewport, Allocators::heap())(top, left, bottom, right);
      script.stack().push((void*)viewport);
      return 1;
    }

    static size_t destroy(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments != 1)
        script.error("Expected one argument (viewport : Viewport).");
      butane::Viewport* viewport;
      arguments.to(0, (void*&)viewport);
      make_delete(Viewport, Allocators::heap(), viewport);
      return 0;
    }

    static size_t update(
      Script& script,
      const Script::Arguments& arguments )
    {
      if (arguments != 5)
        script.error("Expected five arguments (viewport : Viewport, top : Number, left : Number, bottom : Number, right : Number).");
      butane::Viewport* viewport;
      int top, left, bottom, right;
      arguments.to(0, (void*&)viewport);
      arguments.to(1, top);
      arguments.to(2, left);
      arguments.to(3, bottom);
      arguments.to(4, right);
      if ((top < 0) || (left < 0) || (bottom < 0) || (right < 0))
        script.error("Expected top, left, bottom, and right to be non-negative.");
      viewport->update(top, left, bottom, right);
      return 0;
    }
  } // Viewport
} // script_interface
} // butane

namespace butane {
namespace script_interface {
namespace Viewport {
  void expose(
    butane::Script& script )
  {
    script.expose("Viewport.create", &create);
    script.expose("Viewport.destroy", &destroy);
    script.expose("Viewport.update", &update);
  }
} // Viewport
} // script_interface
} // butane
