// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/script.h>

namespace butane {
  Script::Script()
    : _error_handler(nullptr)
    , _error_handler_closure(nullptr)
  {
  }

  Script::~Script()
  {
  }

  void Script::error(
    const char* message )
  {
    if (_error_handler)
      _error_handler(*this, _error_handler_closure);
    else
      fail("Unhandled script error: %s", message);
  }
} // butane
