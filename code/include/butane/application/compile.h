// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_APPLICATION_COMPILE_H_
#define _BUTANE_APPLICATION_COMPILE_H_

#include <butane/application.h>

namespace butane {
namespace Application {
  extern BUTANE_EXPORT void compile(
    const Array<const char*>& args );
} // Application
} // butane

#endif // _BUTANE_APPLICATION_COMPILE_H_
