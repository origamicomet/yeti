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
  void register_a_type(const Resource::Type *type);

  Resource::File::Id add_a_file(const char *path);

  void remove_a_file(Resource::File::Id file);

  void info(Resource::File::Id file,
            Resource::File *info);

  void touch(Resource::File::Id file,
             u64 timestamp,
             const char fingerprint[40]);

  Resource::Id add_a_resource(Resource::Type::Id type,
                              const char *name);

  void remove_a_resource(Resource::Id resource);

  Resource::Id resource_by_name(Resource::Type::Id type,
                                const char *name);

  Resource::Id resource_by_source(Resource::File::Id source);

  Resource::Build::Id queue_a_build(Resource::Id resource);

  void start_a_build(Resource::Build::Id build);

  void finish_a_build(Resource::Build::Id build,
                      bool success);

  void queued(core::Array<Resource::Build::Id> &builds);

  void log(Resource::Build::Id build,
           Resource::Build::Log::Level level,
           const char *message);

  u32 version(Resource::Id resource);

  u64 built(Resource::Id resource);

 public:
  bool optimized() const;
  bool sophisticated() const;

 private:
  bool insert_a_type_(const Resource::Type *type);
  void update_a_type_(Resource::Type::Id type, u32 version);
  void associate_an_extension_(Resource::Type::Id type, const char *extension);
  void disassociate_all_extensions_(Resource::Type::Id type);

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

  /// \internal Tracks transaction depth.
  size_t transactions_;

  /// \internal Prepared statements used to interact with the database.
  /// @{
  sqlite3_stmt *find_a_type_by_name_stmt_;
  sqlite3_stmt *find_a_type_by_extension_stmt_;
  sqlite3_stmt *insert_a_type_stmt_;
  sqlite3_stmt *update_a_type_stmt_;
  sqlite3_stmt *associate_an_extension_stmt_;
  sqlite3_stmt *disassociate_all_extensions_stmt_;
  sqlite3_stmt *find_a_file_by_id_stmt_;
  sqlite3_stmt *find_a_file_by_path_stmt_;
  sqlite3_stmt *insert_a_file_stmt_;
  sqlite3_stmt *update_a_file_stmt_;
  sqlite3_stmt *find_a_resource_by_id_stmt_;
  sqlite3_stmt *find_a_resource_by_name_stmt_;
  sqlite3_stmt *insert_a_resource_stmt_;
  sqlite3_stmt *queue_a_build_stmt_;
  sqlite3_stmt *start_a_build_stmt_;
  sqlite3_stmt *finish_a_build_stmt_;
  sqlite3_stmt *log_a_message_stmt_;
  sqlite3_stmt *builds_of_resource_stmt_;
  /// @}
};

} // yeti

#endif // _YETI_SOPHISTICATED_RESOURCE_DATABASE_H_
