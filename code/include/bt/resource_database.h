// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_DATABASE_H_
#define _BUTANE_RESOURCE_DATABASE_H_

#include <bt/foundation.h>
#include <bt/config.h>

#include <sqlite3.h>

namespace bt {
  class ResourceDatabase {
    FOUNDATION_NON_COPYABLE(ResourceDatabase);

    private:
      ResourceDatabase();
      ~ResourceDatabase();

    public:
      static ResourceDatabase* open(
        const char* path );

      static ResourceDatabase* create(
        const char* path );

      void close();

    public:
      FOUNDATION_INLINE sqlite3* sqlite() const
      { return _sqlite; }

    private:
      sqlite3* _sqlite;
  };
} // bt

#endif // _BUTANE_RESOURCE_DATABASE_H_