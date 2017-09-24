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

SophisticatedResourceDatabase::SophisticatedResourceDatabase()
  : db_(NULL)
  , in_transaction_(false)
  , nesting_(0)
  , upsert_a_resource_stmt_(NULL)
  , resource_by_id_stmt_(NULL)
  , find_by_name_stmt_(NULL)
  , upsert_a_file_stmt_(NULL)
  , file_by_id_stmt_(NULL)
  , file_by_path_stmt_(NULL)
  , update_timestamp_and_fingerprint_stmt_(NULL)
  , start_a_build_stmt_(NULL)
  , finish_a_build_stmt_(NULL)
  , log_a_message_stmt_(NULL) {
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
  "PRAGMA user_version = " YETI_STRINGIFY(__YETI_REVISION__) ";"

void SophisticatedResourceDatabase::prepare_version_() {
  if (u32 magic = this->pragma_("PRAGMA application_id;")) {
    // Prevent idiots from opening the wrong type of database.
    yeti_assert_with_reason(magic == 0x59455449, "Not a resource database?");

    if (u32 version = this->pragma_("PRAGMA user_version;")) {
      // TODO(mtwilliams): Check version to determine if we are compatible.
      // TODO(mtwilliams): Migrations, if not.
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

    "CREATE TABLE IF NOT EXISTS resources (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  type INTEGER NOT NULL,\n"
    "  name VARCHAR(255) NOT NULL,\n"
    "  UNIQUE (type, name)"
    ");\n\n"

    "CREATE TABLE IF NOT EXISTS builds (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  resource_id INTEGER NOT NULL,\n"
    "  started_at DATETIME NULL,\n"
    "  finished_at DATETIME NULL,\n"
    "  success BOOLEAN NULL,\n"
    "  FOREIGN KEY(resource_id) REFERENCES resources(id)\n"
    ");\n\n"

    // Online and offline dependencies.
    "CREATE TABLE IF NOT EXISTS dependencies (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  type INTEGER NOT NULL,"
    "  build_id INTEGER NOT NULL,\n"
    "  dependent_on_resource_id INTEGER NULL,\n"
    "  dependent_on_file_id INTEGER NULL,\n"
    "  FOREIGN KEY(build_id) REFERENCES builds(id),\n"
    "  FOREIGN KEY(dependent_on_resource_id) REFERENCES resources(id),\n"
    "  FOREIGN KEY(dependent_on_file_id) REFERENCES files(id)\n"
    ");\n\n"

    "CREATE TABLE IF NOT EXISTS logs (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  build_id INTEGER NOT NULL,\n"
    "  level INTEGER NOT NULL,\n"
    "  message TEXT NOT NULL,\n"
    "  FOREIGN KEY(build_id) REFERENCES builds(id)\n"
    ");\n\n";

  this->exec_(migration);
}

#define UPSERT_A_RESOURCE_SQL \
  "INSERT OR IGNORE INTO resources (type, name) VALUES (?1, ?2);"

#define RESOURCE_BY_ID_SQL \
  "SELECT * FROM resources WHERE id = ?1 LIMIT 1;"

#define FIND_BY_NAME_SQL \
  "SELECT id FROM resources WHERE type = ?1 AND name = ?2 LIMIT 1;"

#define UPSERT_A_FILE_SQL \
  "INSERT OR IGNORE INTO files (path) VALUES (?1);"

#define FILE_BY_ID_SQL \
  "SELECT id, path, strftime('%s', timestamp) AS timestamp, fingerprint FROM files WHERE id = ?1 LIMIT 1;"

#define FILE_BY_PATH_SQL \
  "SELECT id, path, strftime('%s', timestamp) AS timestamp, fingerprint FROM files WHERE path = ?1 LIMIT 1;"

#define UPDATE_TIMESTAMP_AND_FINGERPRINT_SQL \
  "UPDATE files SET timestamp = datetime(?2, 'unixepoch'), fingerprint = ?3 WHERE id = ?1;"

#define START_A_BUILD_SQL \
  "INSERT INTO builds (resource_id, started_at, finished_at, success) VALUES (?1, datetime('now', 'utc'), NULL, NULL);"

#define FINISH_A_BUILD_SQL \
  "UPDATE builds SET finished_at = datetime('now', 'utc'), success = ?2 WHERE id = ?1;"

#define LOG_A_MESSAGE_SQL \
  "INSERT INTO logs (build_id, level, message) VALUES (?1, ?2, ?3);"

void SophisticatedResourceDatabase::prepare_statements_() {
  upsert_a_resource_stmt_ = this->prepare_(UPSERT_A_RESOURCE_SQL);
  resource_by_id_stmt_ = this->prepare_(RESOURCE_BY_ID_SQL);
  find_by_name_stmt_ = this->prepare_(FIND_BY_NAME_SQL);
  upsert_a_file_stmt_ = this->prepare_(UPSERT_A_FILE_SQL);
  file_by_id_stmt_ = this->prepare_(FILE_BY_ID_SQL);
  file_by_path_stmt_ = this->prepare_(FILE_BY_PATH_SQL);
  update_timestamp_and_fingerprint_stmt_ = this->prepare_(UPDATE_TIMESTAMP_AND_FINGERPRINT_SQL);
  start_a_build_stmt_ = this->prepare_(START_A_BUILD_SQL);
  finish_a_build_stmt_ = this->prepare_(FINISH_A_BUILD_SQL);
  log_a_message_stmt_ = this->prepare_(LOG_A_MESSAGE_SQL);
}

void SophisticatedResourceDatabase::cleanup_() {
  this->cleanup_statements_();
}

void SophisticatedResourceDatabase::cleanup_statements_() {
  sqlite3_finalize(upsert_a_resource_stmt_);
  sqlite3_finalize(resource_by_id_stmt_);
  sqlite3_finalize(find_by_name_stmt_);
  sqlite3_finalize(upsert_a_file_stmt_);
  sqlite3_finalize(file_by_id_stmt_);
  sqlite3_finalize(file_by_path_stmt_);
  sqlite3_finalize(update_timestamp_and_fingerprint_stmt_);
  sqlite3_finalize(start_a_build_stmt_);
  sqlite3_finalize(finish_a_build_stmt_);
  sqlite3_finalize(log_a_message_stmt_);
}

void SophisticatedResourceDatabase::begin() {
  sqlite3_exec(db_, "BEGIN TRANSACTION;", NULL, NULL, NULL);

  in_transaction_ = true;
  nesting_++;
}

void SophisticatedResourceDatabase::end() {
  sqlite3_exec(db_, "END TRANSACTION;", NULL, NULL, NULL);

  if (--nesting_ == 0)
    in_transaction_ = false;
}

Resource::Id SophisticatedResourceDatabase::id_from_name(Resource::Type::Id type,
                                                         const char *name) {
  YETI_SCOPED_LOCK(lock_);

  this->upsert_a_resource(type, name);

  int status;

  sqlite3_bind_int(find_by_name_stmt_, 1, type);
  sqlite3_bind_text(find_by_name_stmt_, 2, name, -1, SQLITE_STATIC);

  status = sqlite3_step(find_by_name_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_ROW, "No rows returned?");

  const u64 id = (u64)sqlite3_column_int(find_by_name_stmt_, 0);

  sqlite3_reset(find_by_name_stmt_);

  yeti_assert_with_reason_debug(id != 0, "Couldn't find resource?");

  return id;
}

Resource::Id SophisticatedResourceDatabase::id_from_path(const char *path) {
  YETI_SCOPED_LOCK(lock_);

  const char *ext = core::path::extension(path);
  yeti_assert_debug(ext != NULL);

  const Resource::Type *type = resource_manager::type_from_ext(ext);
  yeti_assert_debug(type != NULL);

  char name[256] = { 0, };
  const size_t name_len = ext - path - 1;
  yeti_assert_debug(name_len <= 255);
  strncpy(&name[0], path, name_len);

  return this->id_from_name(resource_manager::id_from_type(type), &name[0]);
}

Resource::Type::Id SophisticatedResourceDatabase::type_from_id(Resource::Id id) {
  YETI_SCOPED_LOCK(lock_);

  int status;

  sqlite3_bind_int64(resource_by_id_stmt_, 1, id);

  status = sqlite3_step(resource_by_id_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_ROW, "No rows returned?");

  const Resource::Type::Id type = sqlite3_column_int(resource_by_id_stmt_, 1);

  sqlite3_reset(resource_by_id_stmt_);

  return type;
}

Resource::Source::Id SophisticatedResourceDatabase::source_from_path(const char *path) {
  YETI_SCOPED_LOCK(lock_);

  int status;

  sqlite3_bind_text(upsert_a_file_stmt_, 1, path, -1, SQLITE_STATIC);

  status = sqlite3_step(upsert_a_file_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_DONE, "Couldn't upsert source?");

  sqlite3_bind_text(file_by_path_stmt_, 1, path, -1, SQLITE_STATIC);

  status = sqlite3_step(file_by_path_stmt_);
  yeti_assert_with_reason(status == SQLITE_ROW, "Couldn't find source?");

  const Resource::Source::Id id = sqlite3_column_int(file_by_path_stmt_, 0);

  sqlite3_reset(upsert_a_file_stmt_);
  sqlite3_reset(file_by_path_stmt_);

  return id;
}

void SophisticatedResourceDatabase::info(Resource::Source::Id source,
                                         Resource::Source *info) {
  YETI_SCOPED_LOCK(lock_);

  int status;

  sqlite3_bind_int(file_by_id_stmt_, 1, source);

  status = sqlite3_step(file_by_id_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_ROW, "Couldn't find source?");

  info->id = sqlite3_column_int(file_by_id_stmt_, 0);

  strncpy(&info->path[0],
          (const char *)sqlite3_column_text(file_by_id_stmt_, 1),
          sizeof(info->path));

  info->timestamp = (u64)sqlite3_column_int64(file_by_id_stmt_, 2);

  if (sqlite3_column_type(file_by_id_stmt_, 3) != SQLITE_NULL)
    strncpy(&info->fingerprint[0], (const char *)sqlite3_column_text(file_by_id_stmt_, 3), 40);
  else
    strncpy(&info->fingerprint[0], "0000000000000000000000000000000000000000", 40);

  sqlite3_reset(file_by_id_stmt_);
}

void SophisticatedResourceDatabase::touch(Resource::Source::Id source,
                                          u64 timestamp,
                                          const char fingerprint[41]) {
  YETI_SCOPED_LOCK(lock_);

  int status;

  sqlite3_bind_int(update_timestamp_and_fingerprint_stmt_, 1, source);
  sqlite3_bind_int64(update_timestamp_and_fingerprint_stmt_, 2, timestamp);
  sqlite3_bind_text(update_timestamp_and_fingerprint_stmt_, 3, fingerprint, -1, SQLITE_STATIC);

  status = sqlite3_step(update_timestamp_and_fingerprint_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_DONE, "Couldn't update source?");

  sqlite3_reset(update_timestamp_and_fingerprint_stmt_);
}

Resource::Build::Id SophisticatedResourceDatabase::start_a_build(Resource::Id resource) {
  YETI_SCOPED_LOCK(lock_);

  int status;

  sqlite3_bind_int64(start_a_build_stmt_, 1, resource);

  status = sqlite3_step(start_a_build_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_DONE, "Couldn't insert a build?");

  const Resource::Build::Id id = sqlite3_last_insert_rowid(db_);

  sqlite3_reset(start_a_build_stmt_);

  return id;
}

void SophisticatedResourceDatabase::finish_a_build(Resource::Build::Id build,
                                                   bool success) {
  YETI_SCOPED_LOCK(lock_);

  int status;

  sqlite3_bind_int64(finish_a_build_stmt_, 1, build);
  sqlite3_bind_int(finish_a_build_stmt_, 2, success ? 1 : 0);

  status = sqlite3_step(finish_a_build_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_DONE, "Couldn't update build?");

  sqlite3_reset(finish_a_build_stmt_);
}

void SophisticatedResourceDatabase::log(Resource::Build::Id build,
                                        Resource::Build::Log::Level level,
                                        const char *message) {
  YETI_SCOPED_LOCK(lock_);

  int status;

  sqlite3_bind_int(log_a_message_stmt_, 1, build);
  sqlite3_bind_int(log_a_message_stmt_, 2, level);
  sqlite3_bind_text(log_a_message_stmt_, 3, message, -1, SQLITE_STATIC);

  status = sqlite3_step(log_a_message_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_DONE, "Couldn't log a message?");

  sqlite3_reset(log_a_message_stmt_);
}

bool SophisticatedResourceDatabase::optimized() const {
  return false;
}

bool SophisticatedResourceDatabase::sophisticated() const {
  return true;
}

void SophisticatedResourceDatabase::upsert_a_resource(Resource::Type::Id type,
                                                      const char *name) {
  int status;

  sqlite3_bind_int(upsert_a_resource_stmt_, 1, type);
  sqlite3_bind_text(upsert_a_resource_stmt_, 2, name, -1, SQLITE_STATIC);

  status = sqlite3_step(upsert_a_resource_stmt_);
  yeti_assert_with_reason_debug(status == SQLITE_DONE, "Couldn't upsert resource?");

  sqlite3_reset(upsert_a_resource_stmt_);
}

// TODO(mtwilliams): Use `sqlite3_prepare_v3` with `SQLITE_PREPARE_PERSISTENT`
// after moving to SQLite 3.20.1 (or newer) to reduce pressue on lookaside
// allocator.

sqlite3_stmt *SophisticatedResourceDatabase::prepare_(const char *sql) {
  sqlite3_stmt *stmt;
  int status;

  status = sqlite3_prepare_v2(db_, sql, -1, &stmt, &sql);
  yeti_assert_with_reason(status == SQLITE_OK, "Compilation of prepared statement failed: \"%s\"", sqlite3_errmsg(db_));

  return stmt;
}

void SophisticatedResourceDatabase::exec_(const char *sql) {
  while (sql) {
    sqlite3_stmt *stmt;
    int status;

    status = sqlite3_prepare_v2(db_, sql, -1, &stmt, &sql);
    yeti_assert_with_reason(status == SQLITE_OK, "Compilation failed: \"%s\"", sqlite3_errmsg(db_));

    if (stmt == NULL)
      // We prepared an empty statement. End of |sql|.
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
