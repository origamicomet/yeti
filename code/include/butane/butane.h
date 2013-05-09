// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_H_
#define _BUTANE_H_

#include <butane/foundation.h>
#include <butane/config.h>

namespace butane {
  class Script;
  extern BUTANE_EXPORT void expose(
    butane::Script& script );
} // butane

#endif // _BUTANE_H_
