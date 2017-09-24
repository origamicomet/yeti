//===-- yeti/sophisticated_resource_database.h ----------*- mode: C++11 -*-===//
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

#ifndef _YETI_SOPHISTICATED_RESOURCE_DATABASE_H_
#define _YETI_SOPHISTICATED_RESOURCE_DATABASE_H_

#include "yeti/resource_database.h"

extern "C" {
  struct sqlite3;
  struct sqlite3_stmt;
}

namespace yeti {

/// ...
class YETI_PUBLIC SophisticatedResourceDatabase : public ResourceDatabase {
 YETI_DISALLOW_COPYING(SophisticatedResourceDatabase)

 private:
  SophisticatedResourceDatabase();
  ~SophisticatedResourceDatabase();

 public:
  static SophisticatedResourceDatabase *open(const char *path);
  static SophisticatedResourceDatabase *open_or_create(const char *path);

 private:
  void prepare_();
  void prepare_behaviour_();
  void prepare_version_();
  void prepare_schema_();
  void prepare_statements_();

 private:
  void cleanup_();
  void cleanup_statements_();

 public:
  void begin();
  void end();

 public:
  Resource::Id id_from_name(Resource::Type::Id type, const char *name);
  Resource::Id id_from_path(const char *path);

  Resource::Type::Id type_from_id(Resource::Id id);

 public:
  Resource::Source::Id source_from_path(const char *path);

  void info(Resource::Source::Id source,
            Resource::Source *info);

  void touch(Resource::Source::Id source,
             u64 timestamp,
             const char fingerprint[41]);

  Resource::Build::Id start_a_build(Resource::Id resource);
  void finish_a_build(Resource::Build::Id build, bool success);

  void log(Resource::Build::Id build,
           Resource::Build::Log::Level level,
           const char *message);

 public:
  bool optimized() const;
  bool sophisticated() const;

 private:
  void upsert_a_resource(Resource::Type::Id type,
                         const char *name);

 private:
  /// \internal Prepares a query for repeated use.
  sqlite3_stmt *prepare_(const char *sql);

  /// \internal Executes all queries in @sql.
  void exec_(const char *sql);

  /// \internal Executes a pragma, returning result.
  u32 pragma_(const char *sql);

 private:
  mutable core::Lock lock_;

  sqlite3 *db_;

  /// \internal Tracks if we're currently in a transaction.
  bool in_transaction_;

  /// \internal Tracks "transaction" recursion.
  size_t nesting_;

  /// \internal Upserts a resource, assigning a unique identifier and returning it.
  sqlite3_stmt *upsert_a_resource_stmt_;

  /// \internal Looks up a resource by id.
  sqlite3_stmt *resource_by_id_stmt_;

  /// \internal Looks up a resource by name (and type).
  sqlite3_stmt *find_by_name_stmt_;

  /// \internal Upserts a file, assigning a unique identifier and returning it.
  sqlite3_stmt *upsert_a_file_stmt_;

  /// \internal Looks up a file by unique identifier.
  sqlite3_stmt *file_by_id_stmt_;

  /// \internal Looks up a file by path.
  sqlite3_stmt *file_by_path_stmt_;

  /// \internal Updates a file's timestamp and fingerprint.
  sqlite3_stmt *update_timestamp_and_fingerprint_stmt_;

  /// \internal Inserts a build for a resource, setting `started_at` to now.
  sqlite3_stmt *start_a_build_stmt_;

  /// \internal Updates a build, setting `success` as specified and setting
  /// `finished_at` to now.
  sqlite3_stmt *finish_a_build_stmt_;

  /// \internal Logs a message associated with a particular build.
  sqlite3_stmt *log_a_message_stmt_;
};

} // yeti

#endif // _YETI_SOPHISTICATED_RESOURCE_DATABASE_H_
