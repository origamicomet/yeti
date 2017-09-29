//===-- yeti/resource_manager.h -------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_RESOURCE_MANAGER_H_
#define _YETI_RESOURCE_MANAGER_H_

#include "yeti/core.h"

#include "yeti/resource.h"

namespace yeti {

class ResourceDatabase;

namespace resource_manager {

struct Config {
  /// Path to resource database.
  /// \note Resources cannot be loaded or unloaded if `NULL`.
  const char *database;

  /// Toggles automatic loading of resources as required rather than loading
  /// through packages.
  bool autoload;
};

extern YETI_PUBLIC void initialize(const Config &config);

extern YETI_PUBLIC void shutdown();

/// Returns resource database.
extern YETI_PUBLIC ResourceDatabase *database();

extern YETI_PUBLIC bool autoloads();

extern YETI_PUBLIC bool available(Resource::Id id);

extern YETI_PUBLIC Resource *lookup(Resource::Id id);

extern YETI_PUBLIC Resource *load(Resource::Id id);

extern YETI_PUBLIC void reload(Resource::Id id);

extern YETI_PUBLIC void unload(Resource *resource);

extern YETI_PUBLIC Resource::State state(Resource::Id id);

extern YETI_PUBLIC Resource::State state(Resource *resource);

} // resource_manager

} // yeti

#endif // _YETI_RESOURCE_MANAGER_H_

