// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_DATABASE_RECORDS_RESOURCE_H_
#define _BUTANE_DATABASE_RECORDS_RESOURCE_H_

#include <bt/resource_database.h>

namespace bt {
namespace db {
namespace Records {
  class Resource {
    FOUNDATION_NON_COPYABLE(Resource);

    private:
      Resource();
      ~Resource();

    public:
      static Resource* insert(
        bt::ResourceDatabase* database,
        const TimeStamp& last_modified,
        const char* path );

      static Resource* find(
        bt::ResourceDatabase* database,
        const char* path );

      bool save();

      bool remove();

    public:
      static bool schema(
        sqlite3* database );

    public:
      FOUNDATION_INLINE TimeStamp last_modified() const
      { return _last_modified; }

      FOUNDATION_INLINE TimeStamp set_last_modified( const TimeStamp& ts )
      { _last_modified = ts; }

      FOUNDATION_INLINE const String& path() const
      { return _path; }

    private:
      bt::ResourceDatabase* _db;
      int _id;
      TimeStamp _last_modified;
      String _path;
  };
} // Records
} // db
} // bt

#endif // _BUTANE_DATABASE_RECORDS_RESOURCE_H_