// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/resource.h>

namespace bt {
  Resource::Resource( Type& type, Id id )
    : _type(type)
    , _id(id)
  {
  }

  Resource::~Resource()
  {
  }

  void Resource::reload()
  {
    if (_id == Resource::invald_id)
      return;

    log(
      "Reloading #<Resource:%016" PRIx64 "> [id=%016u]\n",
      this, _id);

    return _type.reload(this);
  }
} // bt