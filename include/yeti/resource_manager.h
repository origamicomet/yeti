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
#include "yeti/resource_database.h"

namespace yeti {

namespace resource_manager {

struct Config {
  /// Path to resource database.
  /// \note Resources cannot be loaded or unloaded if `NULL`.
  const char *database;

  /// Toggles automatic loading of resources as required rather than loading
  /// through packages.
  bool autoload;
};

/// Derives the resource type id from @type.
extern YETI_PUBLIC Resource::Type::Id id_from_type(const Resource::Type *type);

/// Extracts the file extension from @path and uses it to find the associated
/// resource type via yeti::resource_manager::type_from_ext.
extern YETI_PUBLIC const Resource::Type *type_from_id(Resource::Type::Id id);

/// Finds a resource type via its name.
extern YETI_PUBLIC const Resource::Type *type_from_name(const char *name);

/// Extracts the file extension from @path and uses it to find the associated
/// resource type via yeti::resource_manager::type_from_ext.
extern YETI_PUBLIC const Resource::Type *type_from_path(const char *path);

/// Finds the yeti::Resource::Type associated with the file extension @ext.
/// Refer to yeti::Resource::Type::extensions for more information.
extern YETI_PUBLIC const Resource::Type *type_from_ext(const char *ext);

/// Returns resource database.
extern YETI_PUBLIC ResourceDatabase *database();

///
extern YETI_PUBLIC void initialize(const Config &config);

///
extern YETI_PUBLIC void shutdown();

///
extern YETI_PUBLIC void track(const Resource::Type *type);

///
extern YETI_PUBLIC Resource *find(Resource::Id id);

// TODO(mtwilliams): Only allow resources to be loaded (outside of the engine)
// via packages.

///
extern YETI_PUBLIC Resource *load(Resource::Id id);

///
extern YETI_PUBLIC void unload(Resource *resource);

} // resource_manager

} // yeti

#endif // _YETI_RESOURCE_MANAGER_H_
