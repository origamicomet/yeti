// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WINDOW_MODE_H_
#define _BUTANE_WINDOW_MODE_H_

#include <bt/foundation.h>
#include <bt/config.h>

namespace bt {
  enum WindowMode {
    BT_WINDOWED            = 0,
    BT_FULLSCREEN          = 1,
    BT_FULLSCREEN_WINDOWED = 2
  };
} // bt

#endif // _BUTANE_WINDOW_MODE_H_