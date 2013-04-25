// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/resource_database.h>

namespace bt {
  ResourceDatabase::ResourceDatabase()
  {
  }

  ResourceDatabase::~ResourceDatabase()
  {
  }

  ResourceDatabase* ResourceDatabase::open(
    const char* path )
  {
    assert(path != nullptr);

    if (!File::exists(path))
      return nullptr;

    ResourceDatabase* rdb = make_new(ResourceDatabase, Allocator::heap())();
    return rdb;
  }

  ResourceDatabase* ResourceDatabase::create(
    const char* path )
  {
    assert(path != nullptr);

    if (File::exists(path))
      return nullptr;

    ResourceDatabase* rdb = make_new(ResourceDatabase, Allocator::heap())();
    return rdb;
  }

  void ResourceDatabase::close()
  {
    make_delete(ResourceDatabase, Allocator::heap(), this);
  }
} // bt