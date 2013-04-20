// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/resource_compiler.h>
#include <bt/resource_database.h>
#include <bt/db/records/resource.h>

namespace bt {
namespace ResourceCompiler {
  bool compile(
    const String& source_data_directory,
    const String& data_directory,
    bool daemon )
  {
    if (source_data_directory.empty())
      return false;

    if (data_directory.empty())
      return false;

    if (!Directory::exists(source_data_directory.to_ptr())) {
      log("[ResourceCompiler] invalid source data directory!\n");
      return false;
    }

    if (!Directory::exists(data_directory.to_ptr())) {
      Directory* dir = Directory::create(data_directory.to_ptr());
      
      if (!dir) {
        log("[ResourceCompiler] invalid data directory!\n");
        return false;
      }

      dir->close(); 
    }

    ResourceDatabase* rdb;
    const String rdb_ = data_directory + "/resource.db";
    if (!(rdb = ResourceDatabase::open(rdb_.to_ptr()))) {
      if (!(rdb = ResourceDatabase::create(rdb_.to_ptr()))) {
        log("[ResourceCompiler] unable to open or create resource database!\n");
        return false;
      }
    }

    rdb->close();
    return true;
  }
} // ResourceCompiler
} // bt