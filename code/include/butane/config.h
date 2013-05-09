// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_CONFIG_H_
#define _BUTANE_CONFIG_H_

#include <foundation/foundation.h>

// Uncomment this if you're linking to this as a library (and dynamically).
// #define BUTANE_DYNAMICALLY_LINKED 1

#if defined(BUTANE_DYNAMICALLY_LINKED)
  #if defined(BUTANE_COMPILING)
    #define BUTANE_EXPORT BUTANE_DYNAMIC_EXPORT
  #else
    #define BUTANE_EXPORT BUTANE_DYNAMIC_IMPORT
  #endif
#else
  #define BUTANE_EXPORT
#endif

#endif // _BUTANE_CONFIG_H_
