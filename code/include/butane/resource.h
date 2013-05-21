// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_H_
#define _BUTANE_RESOURCE_H_

#include <butane/foundation.h>
#include <butane/config.h>

namespace butane {
  class Resource abstract {
    __foundation_trait(Resource, non_copyable);

    public:
      class Database;
      class Compiler;

      class Id;
      class Type;
      class Stream;
      
      #include <butane/resource/database.h>
      #include <butane/resource/compiler.h>
      
      #include <butane/resource/id.h>
      #include <butane/resource/type.h>
      #include <butane/resource/stream.h>

    protected:
      Resource(
        const Resource::Type& type,
        const Resource::Id id );

      virtual ~Resource();

    public:
      static Resource* load(
        const Type& type,
        const char* path );

      static Resource* load(
        const Type& type,
        const Id id );

      void unload();

    public:
      void reference();
      void dereference();

    private:
      void mark_for_unload();

    private:
      Id _resource_id;
      const Type& _resource_type;
      int32_t _resource_refs;
  };
} // butane

#endif // _BUTANE_RESOURCE_H_
