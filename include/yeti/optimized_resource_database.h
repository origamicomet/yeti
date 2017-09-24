//===-- yeti/optimized_resource_database.h --------------*- mode: C++11 -*-===//
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

#ifndef _YETI_OPTIMIZED_RESOURCE_DATABASE_H_
#define _YETI_OPTIMIZED_RESOURCE_DATABASE_H_

#include "yeti/resource_database.h"

namespace yeti {

/// ...
class YETI_PUBLIC OptimizedResourceDatabase : public ResourceDatabase {
 YETI_DISALLOW_COPYING(OptimizedResourceDatabase)

 private:
  OptimizedResourceDatabase();
  ~OptimizedResourceDatabase();

 public:
  static OptimizedResourceDatabase *open_or_create(const char *path);
  void close();
};

} // yeti

#endif // _YETI_OPTIMIZED_RESOURCE_DATABASE_H_
