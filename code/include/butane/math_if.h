// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_MATH_INTERFACE_H_
#define _BUTANE_MATH_INTERFACE_H_

#include <butane/butane.h>
#include <butane/script.h>

namespace butane {
namespace script_interface {
namespace Math {
  extern BUTANE_EXPORT void expose(
    butane::Script& script );
} // Math
} // script_interface
} // butane

#endif // _BUTANE_MATH_INTERFACE_H_
