// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_FOUNDATION_H_
#define _BUTANE_FOUNDATION_H_

#include <foundation/foundation.h>
namespace butane {
  using namespace foundation;
} // butane

#include <butane/config.h>

namespace butane {
  extern BUTANE_EXPORT void warn(
    const char* format, ... );

  extern BUTANE_EXPORT void fail(
    const char* format, ... );
} // butane

#endif // _BUTANE_FOUNDATION_H_
