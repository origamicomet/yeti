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

#include <sqlite3.h>

namespace yeti {

ResourceDatabase::ResourceDatabase() {
}

ResourceDatabase::~ResourceDatabase() {
}

ResourceDatabase *ResourceDatabase::open_or_create(const char *path) {
  yeti_assert_debug(path != NULL);

  // TODO(mtwilliams): Open existing databases.
  core::fs::destroy(path);

  // TODO(mtwilliams): Modify |flags| based on additional parameters.
  static const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

  ResourceDatabase *rdb = YETI_NEW(ResourceDatabase, core::global_heap_allocator());
  const int status = sqlite3_open_v2(path, &rdb->db, flags, NULL);
  yeti_assert(status == SQLITE_OK);

  rdb->prepare_();

  return rdb;
}

void ResourceDatabase::close() {
  this->cleanup_();

  sqlite3_close_v2(this->db);

  YETI_DELETE(ResourceDatabase, core::global_heap_allocator(), this);
}

void ResourceDatabase::prepare_() {
  this->prepare_behaviour_();
  this->prepare_version_();
  this->prepare_schema_();
  this->prepare_statements_();
}

void ResourceDatabase::prepare_behaviour_() {
  static const char *sql = "PRAGMA encoding = \"UTF-8\";"
                           "PRAGMA foreign_keys = ON;\n"
                           "PRAGMA recursive_triggers = ON;\n";

  this->exec_(sql);
}

void ResourceDatabase::prepare_version_() {
  // TODO(mtwilliams): Check version to determine if we are compatible.
  #define APPLICATION_IDENTIFIER_SQL \
    "PRAGMA application_id = 0x59455449;"

  #define USER_VERSION_SQL \
    "PRAGMA user_version = " YETI_STRINGIFY(__YETI_REVISION__) ";"

  this->exec_(APPLICATION_IDENTIFIER_SQL);
  this->exec_(USER_VERSION_SQL);

  #undef APPLICATION_IDENTIFIER_SQL
  #undef USER_VERSION_SQL
}

void ResourceDatabase::prepare_schema_() {
  // TODO(mtwilliams): Explicitly specify width of types.
  static const char *migration =
    "CREATE TABLE IF NOT EXISTS roots (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "name VARCHAR(31) NOT NULL,\n"
      "path VARCHAR(255) NOT NULL\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS files (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "root_id INTEGER NOT NULL,\n"
      "path VARCHAR(255) NOT NULL,\n"
      "age DATETIME NOT NULL,\n"
      "hash INTEGER NOT NULL,\n"
      "FOREIGN KEY(root_id) REFERENCES roots(id),\n"
      "UNIQUE (path)\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS resources (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "type INTEGER NOT NULL,\n"
      "name VARCHAR(255) NOT NULL\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS variants (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "resource_id INTEGER NOT NULL,\n"
      "FOREIGN KEY(resource_id) REFERENCES resources(id)\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS properties (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "name VARCHAR(31) NOT NULL\n"
    ");\n"

    // TODO(mtwilliams): Extract into macro.
    "INSERT OR IGNORE INTO properties (id, name) VALUES (0, 'pc');\n"
    "INSERT OR IGNORE INTO properties (id, name) VALUES (1, 'web');\n"
    "INSERT OR IGNORE INTO properties (id, name) VALUES (2, 'ios');\n"
    "INSERT OR IGNORE INTO properties (id, name) VALUES (3, 'android');\n"
    "INSERT OR IGNORE INTO properties (id, name) VALUES (4, 'xb1');\n"
    "INSERT OR IGNORE INTO properties (id, name) VALUES (5, 'ps4');\n"

    "CREATE TABLE IF NOT EXISTS properties_of_variants (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "variant_id INTEGER NOT NULL,\n"
      "property_id INTEGER NOT NULL,\n"
      "FOREIGN KEY(variant_id) REFERENCES variants(id),\n"
      "FOREIGN KEY(property_id) REFERENCES properties(id)\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS versions (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "variant_id INTEGER NOT NULL,\n"
      "FOREIGN KEY(variant_id) REFERENCES variants(id)\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS builds (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "version_id INTEGER NOT NULL,\n"
      "status VARCHAR(31) NOT NULL,\n"
      "FOREIGN KEY(version_id) REFERENCES versions(id)\n"
    ");\n"

    // Online and offline dependencies.
    "CREATE TABLE IF NOT EXISTS dependencies (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "type INTEGER NOT NULL,"
      "build_id INTEGER NOT NULL,\n"
      "dependency_id INTEGER NOT NULL,\n"
      "FOREIGN KEY(build_id) REFERENCES builds(id)\n"
      "FOREIGN KEY(dependency_id) REFERENCES resources(id)\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS logs (\n"
      "id INTEGER PRIMARY KEY NOT NULL,\n"
      "build_id INTEGER NOT NULL,\n"
      "level VARCHAR(7) NOT NULL,\n"
      "message TEXT NOT NULL,\n"
      "FOREIGN KEY(build_id) REFERENCES builds(id)\n"
    ");\n";

  this->exec_(migration);
}

void ResourceDatabase::prepare_statements_() {
}

void ResourceDatabase::cleanup_() {
  this->cleanup_statements_();
}

void ResourceDatabase::cleanup_statements_() {
}

void ResourceDatabase::exec_(const char *sql) {
  while (sql) {
    sqlite3_stmt *stmt;
    int status;

    status = sqlite3_prepare_v2(this->db, sql, -1, &stmt, &sql);
    yeti_assert_with_reason(status == SQLITE_OK, sqlite3_errmsg(this->db));

    if (stmt == NULL)
      // We prepared an empty statement, probably due to trailing whitespace. Ingore.
      break;

    status = sqlite3_step(stmt);
    yeti_assert_with_reason(status == SQLITE_DONE, sqlite3_errmsg(this->db));

    sqlite3_finalize(stmt);
  }
}

} // yeti
