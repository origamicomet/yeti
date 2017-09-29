//===-- yeti/sophisticated_resource_database.cc ---------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/sophisticated_resource_database.h"

// For resource types.
#include "yeti/resource_manager.h"

#include <sqlite3.h>

namespace yeti {

namespace core {
namespace log {

// Defined in `yeti/resource_database.cc`.
extern const core::log::Category::Id RESOURCE_DATABASE;

} // log
} // core

SophisticatedResourceDatabase::SophisticatedResourceDatabase()
  : db_(NULL)
  , transactions_(false)
  , find_a_type_by_name_stmt_(NULL)
  , find_a_type_by_extension_stmt_(NULL)
  , insert_a_type_stmt_(NULL)
  , update_a_type_stmt_(NULL)
  , associate_an_extension_stmt_(NULL)
  , disassociate_all_extensions_stmt_(NULL)
  , find_a_file_by_id_stmt_(NULL)
  , find_a_file_by_path_stmt_(NULL)
  , insert_a_file_stmt_(NULL)
  , update_a_file_stmt_(NULL)
  , find_a_resource_by_id_stmt_(NULL)
  , find_a_resource_by_name_stmt_(NULL)
  , insert_a_resource_stmt_(NULL)
  , queue_a_build_stmt_(NULL)
  , start_a_build_stmt_(NULL)
  , finish_a_build_stmt_(NULL)
  , log_a_message_stmt_(NULL)
  , builds_of_resource_stmt_(NULL) {
}


SophisticatedResourceDatabase::~SophisticatedResourceDatabase() {
  // Finalize prepared statements.
  this->cleanup_();

  // Close database.
  sqlite3_close_v2(db_);
}

SophisticatedResourceDatabase *SophisticatedResourceDatabase::open(const char *path) {
  yeti_assert_debug(path != NULL);

  static const int flags = SQLITE_OPEN_READWRITE;

  SophisticatedResourceDatabase *db =
    YETI_NEW(SophisticatedResourceDatabase, core::global_heap_allocator());

  const int status = sqlite3_open_v2(path, &db->db_, flags, NULL);
  yeti_assert(status == SQLITE_OK);

  db->prepare_();

  return db;
}

SophisticatedResourceDatabase *SophisticatedResourceDatabase::open_or_create(const char *path) {
  yeti_assert_debug(path != NULL);

  static const int flags = SQLITE_OPEN_CREATE
                         | SQLITE_OPEN_READWRITE;

  SophisticatedResourceDatabase *db =
    YETI_NEW(SophisticatedResourceDatabase, core::global_heap_allocator());

  const int status = sqlite3_open_v2(path, &db->db_, flags, NULL);
  yeti_assert(status == SQLITE_OK);

  db->prepare_();

  return db;
}

void SophisticatedResourceDatabase::prepare_() {
  this->prepare_behaviour_();
  this->prepare_version_();
  this->prepare_schema_();
  this->prepare_statements_();
}

void SophisticatedResourceDatabase::prepare_behaviour_() {
  static const char *sql = "PRAGMA encoding = \"UTF-8\";"
                           "PRAGMA foreign_keys = ON;\n"
                           "PRAGMA recursive_triggers = ON;\n";

  this->exec_(sql);
}

#define APPLICATION_IDENTIFIER_SQL \
  "PRAGMA application_id = 0x59455449;"

#define USER_VERSION_SQL \
  "PRAGMA user_version = 20170927;"

void SophisticatedResourceDatabase::prepare_version_() {
  if (u32 magic = this->pragma_("PRAGMA application_id;")) {
    // Prevent idiots from opening the wrong type of database.
    yeti_assert_with_reason(magic == 0x59455449, "Not a resource database?");

    if (u32 version = this->pragma_("PRAGMA user_version;")) {
      // TODO(mtwilliams): Check version to determine if we are compatible and
      // perform migrations if not.
    }
  }

  this->exec_(APPLICATION_IDENTIFIER_SQL);
  this->exec_(USER_VERSION_SQL);
}

void SophisticatedResourceDatabase::prepare_schema_() {
  static const char *migration =
    // TODO(mtwilliams): Introduce a foreign key to map files to resources.
    "CREATE TABLE IF NOT EXISTS files (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  path VARCHAR(255) NOT NULL,\n"
    "  timestamp DATETIME NULL,\n"
    "  fingerprint VARCHAR(40) NULL,\n"
    "  UNIQUE (path)\n"
    ");\n\n"

    "CREATE TABLE IF NOT EXISTS types (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  name VARCHAR(255) NOT NULL,\n"
    "  version INTEGER DEFAULT 0,\n"
    "  UNIQUE (name)\n"
    ");\n\n"

    "CREATE TABLE IF NOT EXISTS extensions_for_type (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  type INTEGER NOT NULL,\n"
    "  extension VARCHAR(255) NOT NULL,\n"
    "  FOREIGN KEY(type) REFERENCES types(id)\n"
    ");\n\n"

    "CREATE TABLE IF NOT EXISTS resources (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  type INTEGER NOT NULL,\n"
    "  name VARCHAR(255) NOT NULL,\n"
    "  UNIQUE (type, name),\n"
    "  FOREIGN KEY(type) REFERENCES types(id)\n"
    ");\n\n"

    "CREATE TABLE IF NOT EXISTS builds (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  resource INTEGER NOT NULL,\n"
    "  version INTEGER NOT NULL,\n"
    "  queued_at DATETIME NULL,\n"
    "  started_at DATETIME NULL,\n"
    "  finished_at DATETIME NULL,\n"
    "  success BOOLEAN NULL,\n"
    "  FOREIGN KEY(resource) REFERENCES resources(id)\n"
    ");\n\n"

    // Online and offline dependencies.
    "CREATE TABLE IF NOT EXISTS dependencies (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  type INTEGER NOT NULL,"
    "  build INTEGER NOT NULL,\n"
    "  dependent_on_resource INTEGER NULL,\n"
    "  dependent_on_file INTEGER NULL,\n"
    "  FOREIGN KEY(build) REFERENCES builds(id),\n"
    "  FOREIGN KEY(dependent_on_resource) REFERENCES resources(id),\n"
    "  FOREIGN KEY(dependent_on_file) REFERENCES files(id)\n"
    ");\n\n"

    "CREATE TABLE IF NOT EXISTS logs (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  build INTEGER NOT NULL,\n"
    "  level INTEGER NOT NULL,\n"
    "  message TEXT NOT NULL,\n"
    "  timestamp DATETIME NOT NULL,\n"
    "  FOREIGN KEY(build) REFERENCES builds(id)\n"
    ");\n\n";

  this->exec_(migration);
}

#define FIND_A_TYPE_BY_NAME_SQL \
  "SELECT id, name, version FROM types WHERE name = ?1 LIMIT 1;"

#define FIND_A_TYPE_BY_EXTENSION_SQL \
  "SELECT type.id, type.name, type.version\n" \
  "  FROM types AS type\n" \
  "  JOIN extensions_for_type association\n" \
  "    ON (association.extension = ?1)\n" \
  " LIMIT 1;"

#define INSERT_A_TYPE_SQL \
  "INSERT INTO types (id, name, version) VALUES (?1, ?2, ?3);"

#define UPDATE_A_TYPE_SQL \
  "UPDATE types SET version = ?2 WHERE id = ?1;"

#define ASSOCIATE_AN_EXTENSION_SQL \
  "INSERT INTO extensions_for_type (type, extension) VALUES (?1, ?2);"

#define DISASSOCIATE_ALL_EXTENSIONS_SQL \
  "DELETE FROM extensions_for_type WHERE type = ?1;"

#define FIND_A_FILE_BY_ID_SQL \
  "SELECT id, path, strftime('%s', timestamp) AS timestamp, fingerprint\n" \
  "  FROM files\n" \
  " WHERE id = ?1\n" \
  " LIMIT 1;"

#define FIND_A_FILE_BY_PATH_SQL \
  "SELECT id, path, strftime('%s', timestamp) AS timestamp, fingerprint\n" \
  "  FROM files\n" \
  " WHERE path = ?1\n" \
  " LIMIT 1;"

#define INSERT_A_FILE_SQL \
  "INSERT INTO files (path) VALUES (?1);"

#define REMOVE_A_FILE_SQL \
  "..."

#define UPDATE_A_FILE_SQL \
  "UPDATE files SET timestamp = datetime(?2, 'unixepoch'), fingerprint = ?3 WHERE id = ?1;"

#define FIND_A_RESOURCE_BY_ID_SQL \
  "SELECT id, type, name FROM resources WHERE id = ?1 LIMIT 1;"

#define FIND_A_RESOURCE_BY_NAME_SQL \
  "SELECT id, type, name FROM resources WHERE type = ?1 AND name = ?2 LIMIT 1;"

#define INSERT_A_RESOURCE_SQL \
  "INSERT INTO resources (id, type, name) VALUES (?1, ?2, ?3);"

#define QUEUE_A_BUILD_SQL \
  "INSERT INTO builds (resource, version, queued_at, started_at, finished_at, success) VALUES (?1, 0, datetime('now', 'utc'), NULL, NULL, NULL);"

#define START_A_BUILD_SQL \
  "UPDATE builds SET started_at = datetime('now', 'utc'), version = ?2 WHERE id = ?1 AND started_at IS NULL;"

#define FINISH_A_BUILD_SQL \
  "UPDATE builds SET finished_at = datetime('now', 'utc'), success = ?2 WHERE id = ?1 AND started_at IS NOT NULL AND finished_at IS NULL;"

#define LOG_A_MESSAGE_SQL \
  "INSERT INTO logs (build, level, message, timestamp) VALUES (?1, ?2, ?3, datetime('now', 'utc'));"

#define BUILDS_OF_RESOURCE_SQL \
  "  SELECT id,\n" \
  "         resource,\n" \
  "         version,\n" \
  "         strftime('%s', queued_at) AS queued_at,\n" \
  "         strftime('%s', started_at) AS started_at,\n" \
  "         strftime('%s', finished_at) AS finished_at,\n" \
  "         (finished_at IS NOT NULL) AS finished,\n" \
  "         success\n" \
  "    FROM builds\n" \
  "   WHERE resource = ?1\n" \
  "     AND finished = 1\n" \
  "     AND (success = 1 OR ?2 = 1)\n" \
  "ORDER BY id DESC;"

void SophisticatedResourceDatabase::prepare_statements_() {
  find_a_type_by_name_stmt_ = this->prepare_(FIND_A_TYPE_BY_NAME_SQL);
  find_a_type_by_extension_stmt_ = this->prepare_(FIND_A_TYPE_BY_EXTENSION_SQL);
  insert_a_type_stmt_ = this->prepare_(INSERT_A_TYPE_SQL);
  update_a_type_stmt_ = this->prepare_(UPDATE_A_TYPE_SQL);
  associate_an_extension_stmt_ = this->prepare_(ASSOCIATE_AN_EXTENSION_SQL);
  disassociate_all_extensions_stmt_ = this->prepare_(DISASSOCIATE_ALL_EXTENSIONS_SQL);
  find_a_file_by_id_stmt_ = this->prepare_(FIND_A_FILE_BY_ID_SQL);
  find_a_file_by_path_stmt_ = this->prepare_(FIND_A_FILE_BY_PATH_SQL);
  insert_a_file_stmt_ = this->prepare_(INSERT_A_FILE_SQL);
  update_a_file_stmt_ = this->prepare_(UPDATE_A_FILE_SQL);
  find_a_resource_by_id_stmt_ = this->prepare_(FIND_A_RESOURCE_BY_ID_SQL);
  find_a_resource_by_name_stmt_ = this->prepare_(FIND_A_RESOURCE_BY_NAME_SQL);
  insert_a_resource_stmt_ = this->prepare_(INSERT_A_RESOURCE_SQL);
  queue_a_build_stmt_ = this->prepare_(QUEUE_A_BUILD_SQL);
  start_a_build_stmt_ = this->prepare_(START_A_BUILD_SQL);
  finish_a_build_stmt_ = this->prepare_(FINISH_A_BUILD_SQL);
  log_a_message_stmt_ = this->prepare_(LOG_A_MESSAGE_SQL);
  builds_of_resource_stmt_ = this->prepare_(BUILDS_OF_RESOURCE_SQL);
}

void SophisticatedResourceDatabase::cleanup_() {
  this->cleanup_statements_();
}

void SophisticatedResourceDatabase::cleanup_statements_() {
  sqlite3_finalize(find_a_type_by_name_stmt_);
  sqlite3_finalize(find_a_type_by_extension_stmt_);
  sqlite3_finalize(insert_a_type_stmt_);
  sqlite3_finalize(update_a_type_stmt_);
  sqlite3_finalize(associate_an_extension_stmt_);
  sqlite3_finalize(disassociate_all_extensions_stmt_);
  sqlite3_finalize(find_a_file_by_id_stmt_);
  sqlite3_finalize(find_a_file_by_path_stmt_);
  sqlite3_finalize(insert_a_file_stmt_);
  sqlite3_finalize(update_a_file_stmt_);
  sqlite3_finalize(find_a_resource_by_id_stmt_);
  sqlite3_finalize(find_a_resource_by_name_stmt_);
  sqlite3_finalize(insert_a_resource_stmt_);
  sqlite3_finalize(queue_a_build_stmt_);
  sqlite3_finalize(start_a_build_stmt_);
  sqlite3_finalize(finish_a_build_stmt_);
  sqlite3_finalize(log_a_message_stmt_);
  sqlite3_finalize(builds_of_resource_stmt_);
}

// Resets a statement prior to returning.
#define RESET_ON_RETURN(Statement) \
  const ResetOnReturn YETI_PASTE(__reset_on_return__, __LINE__)(Statement);

class ResetOnReturn {
 YETI_DISALLOW_COPYING(ResetOnReturn)

 public:
  ResetOnReturn(sqlite3_stmt *stmt) : stmt_(stmt) {}
  ~ResetOnReturn() { sqlite3_reset(stmt_); }

 private:
  sqlite3_stmt *stmt_;
};

void SophisticatedResourceDatabase::begin() {
  if (++transactions_ == 1)
    sqlite3_exec(db_, "BEGIN TRANSACTION;", NULL, NULL, NULL);
}

void SophisticatedResourceDatabase::end() {
  if (--transactions_ == 0)
    sqlite3_exec(db_, "END TRANSACTION;", NULL, NULL, NULL);
}

void SophisticatedResourceDatabase::register_a_type(const Resource::Type *type) {
  YETI_SCOPED_LOCK(lock_);

  const Resource::Type::Id id = resource::id_from_type(type);

  this->begin();

  if (!this->insert_a_type_(type))
    this->update_a_type_(id, type->version);

  this->disassociate_all_extensions_(id);

  for (const char **extension = &type->extensions[0]; *extension; ++extension)
    this->associate_an_extension_(id, *extension);

  this->end();
}

Resource::File::Id SophisticatedResourceDatabase::add_a_file(const char *path) {
  YETI_SCOPED_LOCK(lock_);

  RESET_ON_RETURN(insert_a_file_stmt_);
  RESET_ON_RETURN(find_a_file_by_path_stmt_);

  sqlite3_bind_text(insert_a_file_stmt_, 1, path, -1, SQLITE_STATIC);

  switch (sqlite3_step(insert_a_file_stmt_)) {
    case SQLITE_DONE:
      return sqlite3_last_insert_rowid(db_);

    case SQLITE_CONSTRAINT:
      sqlite3_bind_text(find_a_file_by_path_stmt_, 1, path, -1, SQLITE_STATIC);
      sqlite3_step(find_a_file_by_path_stmt_);
      return sqlite3_column_int(find_a_file_by_path_stmt_, 0);
  }

  return 0;
}

void SophisticatedResourceDatabase::remove_a_file(Resource::File::Id id) {
  YETI_TRAP();
}

void SophisticatedResourceDatabase::info(Resource::File::Id file,
                                         Resource::File *info) {
  YETI_SCOPED_LOCK(lock_);

  sqlite3_bind_int(find_a_file_by_id_stmt_, 1, file);

  sqlite3_step(find_a_file_by_id_stmt_);

  info->id = sqlite3_column_int(find_a_file_by_id_stmt_, 0);

  strncpy(&info->path[0],
          (const char *)sqlite3_column_text(find_a_file_by_id_stmt_, 1),
          sizeof(info->path));

  info->timestamp = (u64)sqlite3_column_int64(find_a_file_by_id_stmt_, 2);

  if (sqlite3_column_type(find_a_file_by_id_stmt_, 3) != SQLITE_NULL) {
    core::memory::copy((const void *)sqlite3_column_text(find_a_file_by_id_stmt_, 3),
                       (void *)&info->fingerprint[0],
                       40);
  } else {
    core::memory::copy((const void *)"0000000000000000000000000000000000000000",
                       (void *)&info->fingerprint[0],
                       40);
  }

  sqlite3_reset(find_a_file_by_id_stmt_);
}

void SophisticatedResourceDatabase::touch(Resource::File::Id file,
                                           u64 timestamp,
                                           const char fingerprint[40]) {
  YETI_SCOPED_LOCK(lock_);

  sqlite3_bind_int(update_a_file_stmt_, 1, file);
  sqlite3_bind_int64(update_a_file_stmt_, 2, timestamp);
  sqlite3_bind_text(update_a_file_stmt_, 3, fingerprint, 40, SQLITE_STATIC);

  sqlite3_step(update_a_file_stmt_);

  sqlite3_reset(update_a_file_stmt_);
}

Resource::Id SophisticatedResourceDatabase::add_a_resource(Resource::Type::Id type,
                                                           const char *name) {
  YETI_SCOPED_LOCK(lock_);

  const Resource::Id id = resource::id_from_name(type, name);

  sqlite3_bind_int64(insert_a_resource_stmt_, 1, id);
  sqlite3_bind_int(insert_a_resource_stmt_, 2, type);
  sqlite3_bind_text(insert_a_resource_stmt_, 3, name, -1, SQLITE_STATIC);

  sqlite3_step(insert_a_resource_stmt_);

  sqlite3_reset(insert_a_resource_stmt_);

  return id;
}

void SophisticatedResourceDatabase::remove_a_resource(Resource::Id resource) {
  YETI_TRAP();
}

Resource::Id SophisticatedResourceDatabase::resource_by_name(Resource::Type::Id type,
                                                             const char *name) {
  YETI_SCOPED_LOCK(lock_);

  RESET_ON_RETURN(find_a_resource_by_name_stmt_);

  sqlite3_bind_int(find_a_resource_by_name_stmt_, 1, type);
  sqlite3_bind_text(find_a_resource_by_name_stmt_, 2, name, -1, SQLITE_STATIC);

  sqlite3_step(find_a_resource_by_name_stmt_);

  return (Resource::Id)sqlite3_column_int64(find_a_resource_by_name_stmt_, 0);
}

Resource::Id SophisticatedResourceDatabase::resource_by_source(Resource::File::Id source) {
  YETI_SCOPED_LOCK(lock_);

  RESET_ON_RETURN(find_a_file_by_id_stmt_);
  RESET_ON_RETURN(find_a_resource_by_id_stmt_);

  sqlite3_bind_int(find_a_file_by_id_stmt_, 1, source);

  switch (sqlite3_step(find_a_file_by_id_stmt_)) {
    case SQLITE_ROW: {
      const char *path =
        (const char *)sqlite3_column_text(find_a_file_by_id_stmt_, 1);

      const Resource::Id id = resource::id_from_path(path);

      sqlite3_bind_int64(find_a_resource_by_id_stmt_, 1, id);

      sqlite3_step(find_a_resource_by_id_stmt_);

      return (Resource::Id)sqlite3_column_int64(find_a_resource_by_id_stmt_, 0);
    }
  }

  return 0;
}

Resource::Build::Id SophisticatedResourceDatabase::queue_a_build(Resource::Id resource) {
  YETI_SCOPED_LOCK(lock_);

  sqlite3_bind_int64(queue_a_build_stmt_, 1, resource);
  sqlite3_step(queue_a_build_stmt_);

  const Resource::Build::Id id = (Resource::Build::Id)sqlite3_last_insert_rowid(db_);

  sqlite3_reset(queue_a_build_stmt_);

  return id;
}


void SophisticatedResourceDatabase::start_a_build(Resource::Build::Id build) {
  YETI_SCOPED_LOCK(lock_);

  // TODO(mtwilliams): Track version.
  static const u32 version = 0;

  sqlite3_bind_int(start_a_build_stmt_, 1, build);
  sqlite3_bind_int(start_a_build_stmt_, 2, version);

  sqlite3_step(start_a_build_stmt_);

  yeti_assert_with_reason(sqlite3_changes(db_) == 1, "Build was already started!");

  sqlite3_reset(start_a_build_stmt_);
}

void SophisticatedResourceDatabase::finish_a_build(Resource::Build::Id build,
                                                   bool success) {
  YETI_SCOPED_LOCK(lock_);

  sqlite3_bind_int(finish_a_build_stmt_, 1, build);
  sqlite3_bind_int(finish_a_build_stmt_, 2, success ? 1 : 0);

  sqlite3_step(finish_a_build_stmt_);

  yeti_assert_with_reason(sqlite3_changes(db_) == 1, "Build wasn't started or is already finished!");

  sqlite3_reset(finish_a_build_stmt_);
}

void SophisticatedResourceDatabase::queued(core::Array<Resource::Build::Id> &builds) {
  YETI_TRAP();
}

void SophisticatedResourceDatabase::log(Resource::Build::Id build,
                                        Resource::Build::Log::Level level,
                                        const char *message) {
  YETI_SCOPED_LOCK(lock_);

  sqlite3_bind_int(log_a_message_stmt_, 1, build);
  sqlite3_bind_int(log_a_message_stmt_, 2, level);
  sqlite3_bind_text(log_a_message_stmt_, 3, message, -1, SQLITE_STATIC);

  sqlite3_step(log_a_message_stmt_);

  sqlite3_reset(log_a_message_stmt_);
}

u32 SophisticatedResourceDatabase::version(Resource::Id resource) {
  YETI_SCOPED_LOCK(lock_);

  RESET_ON_RETURN(builds_of_resource_stmt_);

  sqlite3_bind_int64(builds_of_resource_stmt_, 1, resource);
  sqlite3_bind_int64(builds_of_resource_stmt_, 2, 0 /* Successful only. */);

  switch (sqlite3_step(builds_of_resource_stmt_)) {
    case SQLITE_ROW:
      return (u32)sqlite3_column_int(builds_of_resource_stmt_, 2);

    case SQLITE_DONE:
      // Never built successfully.
      return 0;
  }

  return 0;
}

u64 SophisticatedResourceDatabase::built(Resource::Id resource) {
  YETI_SCOPED_LOCK(lock_);

  RESET_ON_RETURN(builds_of_resource_stmt_);

  sqlite3_bind_int64(builds_of_resource_stmt_, 1, resource);
  sqlite3_bind_int64(builds_of_resource_stmt_, 2, 1 /* Successful or not. */);

  switch (sqlite3_step(builds_of_resource_stmt_)) {
    case SQLITE_ROW:
      return (u64)sqlite3_column_int64(builds_of_resource_stmt_, 4);

    case SQLITE_DONE:
      // Never built.
      return 0;
  }

  return 0;
}

bool SophisticatedResourceDatabase::insert_a_type_(const Resource::Type *type) {
  YETI_SCOPED_LOCK(lock_);

  const Resource::Type::Id id = resource::id_from_type(type);

  RESET_ON_RETURN(insert_a_type_stmt_);

  sqlite3_bind_int(insert_a_type_stmt_, 1, id);
  sqlite3_bind_text(insert_a_type_stmt_, 2, type->name, -1, SQLITE_STATIC);
  sqlite3_bind_int(insert_a_type_stmt_, 3, type->version);

  switch (sqlite3_step(insert_a_type_stmt_)) {
    case SQLITE_DONE:
      // New!
      return true;

    case SQLITE_CONSTRAINT:
      // Already exists.
      return false;
  }

  yeti_assert_with_reason(0, "Something went horribly wrong while registering a type.");

  return false;
}

void SophisticatedResourceDatabase::update_a_type_(Resource::Type::Id type, u32 version) {
  YETI_SCOPED_LOCK(lock_);

  sqlite3_bind_int(update_a_type_stmt_, 1, type);
  sqlite3_bind_int(update_a_type_stmt_, 2, version);

  sqlite3_step(update_a_type_stmt_);

  sqlite3_reset(update_a_type_stmt_);
}

void SophisticatedResourceDatabase::associate_an_extension_(Resource::Type::Id type, const char *extension) {
  YETI_SCOPED_LOCK(lock_);

  RESET_ON_RETURN(associate_an_extension_stmt_);

  sqlite3_bind_int(associate_an_extension_stmt_, 1, type);
  sqlite3_bind_text(associate_an_extension_stmt_, 2, extension, -1, SQLITE_STATIC);

  switch (sqlite3_step(associate_an_extension_stmt_)) {
    case SQLITE_DONE:
      // Associated.
      return;

    case SQLITE_CONSTRAINT:
      yeti_assert_with_reason(0, "The extension `%s` is already associated with a type.", extension);
  }
}

void SophisticatedResourceDatabase::disassociate_all_extensions_(Resource::Type::Id type) {
  YETI_SCOPED_LOCK(lock_);

  sqlite3_bind_int(disassociate_all_extensions_stmt_, 1, type);

  sqlite3_step(disassociate_all_extensions_stmt_);

  sqlite3_reset(disassociate_all_extensions_stmt_);
}

bool SophisticatedResourceDatabase::optimized() const {
  return false;
}

bool SophisticatedResourceDatabase::sophisticated() const {
  return true;
}

// TODO(mtwilliams): Use `sqlite3_prepare_v3` with `SQLITE_PREPARE_PERSISTENT`
// after moving to SQLite 3.20.1 (or newer) to reduce pressure on the lookaside
// allocator.

sqlite3_stmt *SophisticatedResourceDatabase::prepare_(const char *sql) {
  sqlite3_stmt *stmt;
  int status;

  status = sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL);

  if (status != SQLITE_OK) {
    core::logf(core::log::RESOURCE_DATABASE, core::log::FATAL, "Compilation of prepared statement failed!\n");
    core::logf(core::log::RESOURCE_DATABASE, core::log::FATAL, "While preparing: %s", sql);
    core::logf(core::log::RESOURCE_DATABASE, core::log::FATAL, "Error: '%s'\n", sqlite3_errmsg(db_));

    // TODO(mtwilliams): Panic.
    YETI_TRAP();
  }

  return stmt;
}

void SophisticatedResourceDatabase::exec_(const char *sql) {
  while (sql) {
    sqlite3_stmt *stmt;
    int status;

    status = sqlite3_prepare_v2(db_, sql, -1, &stmt, &sql);
    yeti_assert_with_reason(status == SQLITE_OK, "Compilation failed: \"%s\"", sqlite3_errmsg(db_));

    if (stmt == NULL)
      break;

    status = sqlite3_step(stmt);
    yeti_assert_with_reason(status == SQLITE_DONE, sqlite3_errmsg(db_));

    sqlite3_finalize(stmt);
  }
}

u32 SophisticatedResourceDatabase::pragma_(const char *sql) {
  sqlite3_stmt *stmt;

  sqlite3_prepare_v2(db_, sql, -1, &stmt, NULL);

  sqlite3_step(stmt);

  const u32 result = sqlite3_column_int(stmt, 0);

  sqlite3_finalize(stmt);

  return result;
}

} // yeti
