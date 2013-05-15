// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Stream::Stream(
    const Resource::Type& type,
    const Resource::Id id
  ) : _memory_resident_data(nullptr)
    , _memory_resident_data_len(0)
    , _streaming_data(nullptr)
  {
    const String path = String::format(
      Allocators::scratch(),
      "data/%016" PRIx64, (uint64_t)id);
    
    // if (!Directory::exists(path))
      fail("The requested resource `%s` does not exist!", path.raw());
  }
} // butane
