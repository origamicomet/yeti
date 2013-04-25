// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_H_
#define _BUTANE_RESOURCE_H_

#include <bt/foundation.h>
#include <bt/config.h>
#include <bt/compile.h>

namespace bt {
  class Resource {
    FOUNDATION_NON_COPYABLE(Resource);

    public:
      class Id {
        public:
          Id( Hash type, Hash hash, uint32_t revision = 1 )
            : _type(type), _hash(hash), _revision(revision)
          {}

          Id( const Id& id )
            : _type(id._type), _hash(id._hash), _revision(id._revision)
          {}

          Id& operator= ( const Id& id )
          {
            _type = id._type;
            _hash = id._hash;
            _revision = id._revision;
            return *this;
          }

        public:
          FOUNDATION_INLINE Hash type() const
          { return _type; }

          FOUNDATION_INLINE Hash hash() const
          { return _hash; }

          FOUNDATION_INLINE uint32_t revision() const
          { return _revision; }

        private:
          Hash _type;
          Hash _hash;
          uint32_t _revision;
      };

      class Stream {
        FOUNDATION_NON_COPYABLE(Stream);
      };

      class Type {
        FOUNDATION_NON_COPYABLE(Type);

        private:
          friend class Resource;

        public:
          typedef bt::Resource* (*Load)(
            bt::Resource::Stream* stream );
          typedef void (*Unload)(
            bt::Resource* resource );
          typedef bt::Compile::Status (*Compile)(
            const bt::Compile::Data& compile_data );

        public:
          Type(
            bt::Hash associated_extension,
            bt::Hash associated_source_extension,
            Load load,
            Unload unload,
            Compile compile );

          Type(
            bt::Hash associated_extension,
            const Array<bt::Hash>& associated_source_extensions,
            Load load,
            Unload unload,
            Compile compile );

        public:
          bool operator== ( const char* extension );

        public:
          void expose() const;

        public:
          bt::Hash associated_extension() const;
          const Array<bt::Hash>& associated_source_extensions() const;

          bt::Resource* load(
            bt::Resource::Stream* stream ) const;

          void unload(
            bt::Resource* resource ) const;

          bt::Compile::Status compile(
            const bt::Compile::Data& compile_data ) const;

        private:
          bt::Hash _assoc_extension;
          String _assoc_extension_s;
          Array<bt::Hash> _assoc_src_extensions;
          Load _load;
          Unload _unload;
          Compile _compile;
      };

    protected:
      Resource( Id id, const Resource::Type& type );
      virtual ~Resource();

    public:
      static const Resource::Type* determine_type_from_extension(
        const char* extension );

    public:
      FOUNDATION_INLINE const Id& id() const
      { return _resource_id; }

      FOUNDATION_INLINE const Resource::Type& type() const
      { return _resource_type; }

      FOUNDATION_INLINE const uint32_t& references() const
      { return _resource_refs; }

    public:
      virtual String to_string();

    protected:
      Id _resource_id;
      const Resource::Type& _resource_type;
      uint32_t _resource_refs;
  };
} // bt

#endif // _BUTANE_RESOURCE_H_