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
  /// Opens any type of resource database at @path if it exists.
  static ResourceDatabase *open(const char *path);

  /// Closes a resource database.
  void close();

 public:
  /// \brief Begins a transaction.
  ///
  /// \detail Operations performed within a transaction are atomic. They either
  /// all succeed or they have no effect.
  ///
  virtual void begin() = 0;

  /// \brief Ends a transaction.
  virtual void end() = 0;

 public:
  /// Registers a type of resource.
  virtual void register_a_type(const Resource::Type *type) = 0;

  /// Upserts a file.
  virtual Resource::File::Id add_a_file(const char *path) = 0;

  /// Removes a file.
  virtual void remove_a_file(Resource::File::Id file) = 0;

  /// Returns information about a file.
  ///
  /// \note Timestamp will be `0` for new files.
  ///
  /// \note Fingerprint will be `0000000000000000000000000000000000000000` for
  /// new files.
  ///
  /// \see yeti::resource::File
  ///
  virtual void info(Resource::File::Id file,
                    Resource::File *info) = 0;

  /// Updates a file's timestamp and fingerprint.
  virtual void touch(Resource::File::Id file,
                     u64 timestamp,
                     const char fingerprint[40]) = 0;

  /// Upserts a resource, returning the unique identifier assigned to it.
  virtual Resource::Id add_a_resource(Resource::Type::Id type,
                                      const char *name) = 0;

  /// Removes a resource.
  virtual void remove_a_resource(Resource::Id resource) = 0;

  /// Determines if a resource exists.
  virtual bool exists(Resource::Id resource) = 0;

  /// Finds a resource by type and name.
  virtual Resource::Id resource_by_name(Resource::Type::Id type,
                                        const char *name) = 0;

  /// Finds a resource by source.
  virtual Resource::Id resource_by_source(Resource::File::Id source) = 0;

  /// Adds a new build of resource to the queue.
  virtual Resource::Build::Id queue_a_build(Resource::Id resource) = 0;

  /// Updates a build to reflect its commencement.
  virtual void start_a_build(Resource::Build::Id build) = 0;

  /// Updates a build to reflect its completion.
  virtual void finish_a_build(Resource::Build::Id build,
                              bool success) = 0;

  /// Finds all queued builds.
  virtual void queued(core::Array<Resource::Build::Id> &builds) = 0;

  /// Records a message associated with a build.
  virtual void log(Resource::Build::Id build,
                   Resource::Build::Log::Level level,
                   const char *message) = 0;

  /// Determines the version of the code used to compile the resource.
  ///
  /// \note Returns `0` if the resource has not been compiled.
  ///
  /// \see yeti::resource::Type::version
  ///
  virtual u32 version(Resource::Id id) = 0;

  /// Determines when the resource was last compiled, successfully or
  /// unsuccessfully.
  ///
  /// \note Returns `0` if the resource has not been compiled.
  ///
  virtual u64 built(Resource::Id resource) = 0;

 public:
  virtual bool optimized() const = 0;
  virtual bool sophisticated() const = 0;
};

} // yeti

#endif // _YETI_RESOURCE_DATABASE_H_
