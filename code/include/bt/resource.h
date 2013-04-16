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
          static void expose( const Type& type );

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
          Array<bt::Hash> _assoc_src_extensions;
          Load _load;
          Unload _unload;
          Compile _compile;
      };

    public:
      static const Resource::Type* determine_type_from_extension(
        const char* extension );

    protected:
  };
} // bt

#endif // _BUTANE_RESOURCE_H_