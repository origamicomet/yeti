// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Type::Type(
    Hash name,
    const char* assoc_file_ext,
    Load load,
    Unload unload,
    Compile compile
  ) : _name(name)
    , _assoc_file_ext(Allocators::heap(), assoc_file_ext)
    , _load(load), _unload(unload), _compile(compile)
  {}
} // butane
