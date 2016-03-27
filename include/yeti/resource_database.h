//===-- yeti/resource_database.h --------------------------*- mode: C++ -*-===//
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

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

extern "C" {
  struct sqlite3;
}

namespace yeti {

/// ...
class YETI_PUBLIC ResourceDatabase {
 YETI_DISALLOW_COPYING(ResourceDatabase);

 private:
  ResourceDatabase();
  ~ResourceDatabase();

 private:
  // REFACTOR(mtwilliams): Into static functions with only |db| passed in.
  void prepare_();
  void prepare_version_();
  void prepare_behaviour_();
  void prepare_schema_();
  void prepare_statements_();

  void cleanup_();
  void cleanup_statements_();

 private:
  sqlite3 *db;
};

} // yeti

#endif // _YETI_RESOURCE_DATABASE_H_
