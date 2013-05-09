// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_APPLICATION_H_
#define _BUTANE_APPLICATION_H_

#include <butane/foundation.h>
#include <butane/config.h>

namespace butane {
namespace Application {
  extern BUTANE_EXPORT void boot(
    size_t num_args,
    const char* args[] );

  extern BUTANE_EXPORT const char* platform();
  extern BUTANE_EXPORT const char* architecture();
  extern BUTANE_EXPORT const char* build();
  extern BUTANE_EXPORT void pause();
  extern BUTANE_EXPORT void unpause();
  extern BUTANE_EXPORT void quit();
} // Application
} // butane

#endif // _BUTANE_APPLICATION_H_
