// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/resource_database.h>
#include <bt/db/records/resource.h>

namespace bt {
  ResourceDatabase::ResourceDatabase()
    : _sqlite(nullptr)
  {
  }

  ResourceDatabase::~ResourceDatabase()
  {
    if (_sqlite)
      sqlite3_close_v2(_sqlite);
  }

  ResourceDatabase* ResourceDatabase::open(
    const char* path )
  {
    assert(path != nullptr);

    if (!File::exists(path))
      return nullptr;

    sqlite3* db;
    if (sqlite3_open_v2(path, &db, SQLITE_OPEN_READWRITE, NULL) != SQLITE_OK)
      return nullptr;

    ResourceDatabase* rdb = make_new(ResourceDatabase, Allocator::heap())();
    rdb->_sqlite = db;
    return rdb;
  }

  static bool schema( sqlite3* db )
  {
    assert(db != nullptr);

    if (!db::Records::Resource::schema(db))
      return false;

    return true;
  }

  ResourceDatabase* ResourceDatabase::create(
    const char* path )
  {
    assert(path != nullptr);

    if (File::exists(path))
      return nullptr;

    sqlite3* db;
    if (sqlite3_open_v2(path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK)
      return nullptr;

    if (!schema(db)) {
      sqlite3_close_v2(db);
      File::destroy(path);
      return nullptr;
    }

    ResourceDatabase* rdb = make_new(ResourceDatabase, Allocator::heap())();
    rdb->_sqlite = db;
    return rdb;
  }

  void ResourceDatabase::close()
  {
    make_delete(ResourceDatabase, Allocator::heap(), this);
  }
} // bt