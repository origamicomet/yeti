// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/foundation.h>

namespace butane {
  void warn(
    const char* format, ... )
  {
    va_list ap;
    va_start(ap, format);
    log(format, ap);
    va_end(ap);
  }

  void fail(
    const char* format, ... )
  {
    va_list ap;
    va_start(ap, format);
    log(format, ap);
    va_end(ap);
    die(true);
  }
} // butane
