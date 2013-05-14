// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Id::Id(
    const Type& type,
    const char* path )
  {
    _hash = (((uint64_t)type.name()) << 32);
    _hash |= ((uint64_t)murmur_hash((const void*)path, strlen(path), 0));
  }

  Resource::Id::Id(
    const Type& type,
    const String& path )
  {
    _hash = (((uint64_t)type.name()) << 32);
    _hash |= ((uint64_t)murmur_hash((const void*)path.raw(), path.size(), 0));
  }

  Resource::Id::operator uint64_t() const
  {
    return _hash;
  }
} // butane
