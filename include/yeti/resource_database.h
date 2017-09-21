//===-- yeti/resource_database.h ------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_RESOURCE_DATABASE_H_
#define _YETI_RESOURCE_DATABASE_H_

#include "yeti/core.h"

extern "C" {
  struct sqlite3;
}

namespace yeti {

// TODO(mtwilliams): Hide internals in a subclass.

/// ...
class YETI_PUBLIC ResourceDatabase {
 YETI_DISALLOW_COPYING(ResourceDatabase)

 private:
  ResourceDatabase();
  ~ResourceDatabase();

 public:
  static ResourceDatabase *open_or_create(const char *path);
  void close();

 private:
  void prepare_();
  void prepare_behaviour_();
  void prepare_version_();
  void prepare_schema_();
  void prepare_statements_();

  void cleanup_();
  void cleanup_statements_();

 private:
  void exec_(const char *sql);

 private:
  sqlite3 *db;
};

} // yeti

#endif // _YETI_RESOURCE_DATABASE_H_
