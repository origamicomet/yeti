// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_H_
#define _BUTANE_RESOURCE_H_

#include <bt/foundation.h>
#include <bt/config.h>

namespace bt {
  class Resource {
    public:
      typedef uint32_t Id;
      static const Id invalid_id = 0xFFFFFFFFu;

      struct CompileData {
        void (*log)( const char* format, ... );
        const char *data, *data_src, *path;
        FILE *in, *mem_resident_data, *streaming_data;
        bool debuggable;
        bool optimized;
      };

      struct Type {
        uint32_t id;
        String assoc_ext;
        Array<String> assoc_src_ext;
        Resource* (*load)( ResourceStream* stream );
        Resource* (*reload)( Resource* resource );
        void (*unload)( Resource* resource );
        bool (*compile)( CompileData& compile_data );
      };

    protected:
      Resource( Type& type, Id id );
      virtual ~Resource();

    public:
      Resource* reload();

    public:
      FOUNDATION_INLINE Type& type() const
      { return _type; }

      FOUNDATION_INLINE Id& id() const
      { return _id; }

    private:
      Type& _type;
      Id _id;
  };
} // bt

#endif // _BUTANE_RESOURCE_H_