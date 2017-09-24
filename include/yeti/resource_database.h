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

// Need various definitions.
#include "yeti/resource.h"

namespace yeti {

/// ...
class YETI_PUBLIC ResourceDatabase {
 YETI_DISALLOW_COPYING(ResourceDatabase)

 protected:
  ResourceDatabase();
  virtual ~ResourceDatabase();

 public:
  static ResourceDatabase *open(const char *path);
  static ResourceDatabase *open_or_create(const char *path);
  void close();

 public:
  virtual void begin() = 0;
  virtual void end() = 0;

 public:
  virtual Resource::Id id_from_name(Resource::Type::Id type,
                                    const char *name) = 0;

  virtual Resource::Id id_from_path(const char *path) = 0;

  virtual Resource::Type::Id type_from_id(Resource::Id id) = 0;

 public:
  virtual Resource::Source::Id source_from_path(const char *path) = 0;

  virtual void info(Resource::Source::Id source,
                    Resource::Source *info) = 0;

  virtual void touch(Resource::Source::Id source,
                     u64 timestamp,
                     const char fingerprint[41]) = 0;

  virtual Resource::Build::Id start_a_build(Resource::Id resource) = 0;
  virtual void finish_a_build(Resource::Build::Id build, bool success) = 0;

  virtual void log(Resource::Build::Id build,
                   Resource::Build::Log::Level level,
                   const char *message) = 0;

 public:
  virtual bool optimized() const = 0;
  virtual bool sophisticated() const = 0;
};

} // yeti

#endif // _YETI_RESOURCE_DATABASE_H_
