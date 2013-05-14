// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_STREAM_H_
#define _BUTANE_RESOURCE_STREAM_H_

#include <butane/foundation.h>
#include <butane/config.h>

namespace butane {
  class Resource::Stream final {
    __foundation_trait(Stream, non_copyable);
  };
} // butane

#endif // _BUTANE_RESOURCE_STREAM_H_
