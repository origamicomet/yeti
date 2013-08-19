// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_H_
#define _BUTANE_RESOURCE_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Resource abstract {
    __foundation_trait(Resource, non_copyable);

    public:
      class Id;
      class Database;
      class Compiler;
      class Type;
      class Stream;
      template <typename T>
      class Handle;
      typedef Hash<uint32_t, murmur_hash> Property;
      typedef uint32_t Variation;

      #include <butane/resource/id.h>
      #include <butane/resource/database.h>
      #include <butane/resource/compiler.h>
      #include <butane/resource/type.h>
      #include <butane/resource/stream.h>
      #include <butane/resource/handle.h>

    protected:
      Resource(
        const Resource::Type& type,
        const Resource::Id id );

      virtual ~Resource();

    public:
      static Resource* find(
        const Type& type,
        const char* path );

      static Resource* find(
        const Type& type,
        const Id id );

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

    public:
      static Variation determine_variation_based_on_properties(
        const Array<Resource::Property>& properties );

    public:
      FOUNDATION_INLINE Id id() const
      { return _resource_id; }

      FOUNDATION_INLINE const Type& type() const
      { return _resource_type; }

    private:
      Id _resource_id;
      const Type& _resource_type;
      int32_t _resource_refs;
  };
} // butane

namespace butane {
  namespace Resources {
    extern BUTANE_EXPORT HashTable<Resource::Id, Resource*>& loaded();
  } // Resources
} // butane

#endif // _BUTANE_RESOURCE_H_
