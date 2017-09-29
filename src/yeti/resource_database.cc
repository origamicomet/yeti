//===-- yeti/resource_database.cc -----------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/resource_database.h"

#include "yeti/sophisticated_resource_database.h"
#include "yeti/optimized_resource_database.h"

namespace yeti {

namespace core {
namespace log {

extern const core::log::Category::Id RESOURCE_DATABASE =
  Category::add("resource_database", GENERAL);

} // log
} // core

ResourceDatabase::ResourceDatabase() {
}

ResourceDatabase::~ResourceDatabase() {
}

ResourceDatabase *ResourceDatabase::open(const char *path) {
  yeti_assert_debug(path != NULL);

  // TODO(mtwilliams): Guess type.
  return SophisticatedResourceDatabase::open(path);
}

void ResourceDatabase::close() {
  YETI_DELETE(ResourceDatabase, core::global_heap_allocator(), this);
}

} // yeti
