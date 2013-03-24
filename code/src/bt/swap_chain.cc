// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/swap_chain.h>

namespace bt {
  SwapChain::SwapChain()
    : _window(nullptr)
    , _render_target(nullptr)
  {
  }

  SwapChain::~SwapChain()
  {
    if (_render_target)
      _render_target->destroy();
  }
} // bt