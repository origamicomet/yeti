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
    const char* format, ... ) const
  {
    const char* msg; {
      va_list ap;
      va_start(ap, format);
      const size_t len = vsnprintf(nullptr, 0, format, ap) + 1;
      msg = (const char*)alloca(len);
      vsnprintf((char*)msg, len, format, ap);
      va_end(ap);
    }

    if (_error_handler)
      _error_handler(*this, _error_handler_closure);
    else
      fail("Unhandled script error: %s", msg);
  }
} // butane
