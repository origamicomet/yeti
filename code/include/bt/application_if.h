// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_APPLICATION_INTERFACE_H_
#define _BUTANE_APPLICATION_INTERFACE_H_

#include <bt/script.h>
#include <bt/application.h>

namespace bt {
namespace ScriptInterface {
namespace Application {
  extern void expose( bt::Script* script );
} // Application
} // ScriptInterface
} // bt

#endif // _BUTANE_APPLICATION_INTERFACE_H_