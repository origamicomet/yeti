// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_APPLICATION_H_
#define _BUTANE_APPLICATION_H_

#include <bt/foundation.h>
#include <bt/config.h>

#include <bt/window.h>
// #include <bt/swap_chain.h>

namespace bt {
namespace Application {
  extern Array<Window*>& windows();
  // extern Array<SwapChain*>& swap_chains();

  extern void boot(
    size_t num_args,
    const char* args[] );

  extern void run(
    const Array<const char*>& args );

  extern void compile(
    const Array<const char*>& args );

  extern void quit();
} // Application
} // bt

#endif // _BUTANE_APPLICATION_H_