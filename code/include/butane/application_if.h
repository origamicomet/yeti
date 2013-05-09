// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_APPLICATION_INTERFACE_H_
#define _BUTANE_APPLICATION_INTERFACE_H_

#include <butane/foundation.h>
#include <butane/config.h>
#include <butane/script.h>

namespace butane {
namespace Application {
  extern BUTANE_EXPORT void expose(
    butane::Script& script );
} // Application
} // butane

#endif // _BUTANE_APPLICATION_INTERFACE_H_
