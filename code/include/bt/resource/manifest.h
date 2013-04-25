// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_MANIFEST_H_
#define _BUTANE_RESOURCE_MANIFEST_H_

#include <bt/resource.h>
#include <sjson.h>

namespace bt {
  class Manifest
    : public bt::Resource
  {
    FOUNDATION_NON_COPYABLE(Manifest);

    private:
      Manifest( Resource::Id id );
      ~Manifest();

    public:
      static Manifest* load(
        bt::Resource::Stream* stream );

      static void unload(
        Manifest* resource );

      static bt::Compile::Status compile(
        const bt::Compile::Data& compile_data );

    public:
      String to_string() const;

    private:
      sjson::Value* _root;
  };
} // bt

#endif // _BUTANE_RESOURCE_MANIFEST_H_