//===-- yeti/resource_database.cc -------------------------*- mode: C++ -*-===//
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

void ResourceDatabase::prepare_() {
  this->prepare_version_();
  this->prepare_behaviour_();
  this->prepare_schema_();
  this->prepare_statements_();
}

void ResourceDatabase::prepare_version_() {
  // TODO(mtwilliams): Check version to determine if we are incompatible.

  /* To differentiate ourselves from any ol' SQLite3 database: */ {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "PRAGMA application_id = ?1;", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, 0x59455449ul /* YETI */);
    yeti_assert(sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
  }

  /* To differentiate various (incompatible?) versions: */ {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "PRAGMA user_version = ?1;", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, __YETI_REVISION__);
    yeti_assert(sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
  }
}

void ResourceDatabase::prepare_behaviour_() {
  static const char *sql = "PRAGMA encoding = \"UTF-8\";"
                           "PRAGMA foreign_keys = ON;\n"
                           "PRAGMA recursive_triggers = ON;\n";
  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  yeti_assert(sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
}

void ResourceDatabase::prepare_schema_() {
  static const char *migration =
    "CREATE TABLE IF NOT EXIST files (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  path TEXT NOT NULL,\n"
    "  age INTEGER NOT NULL\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS resources (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  type TEXT NOT NULL,\n"
    "  path TEXT NOT NULL,\n"
    "  source_id INTEGER NOT NULL,\n"
    "  FOREIGN KEY(source_id) REFERENCES files(id),\n"
    ");\n"

    "CREATE TABLE IF NOT EXISTS dependencies (\n"
    "  id INTEGER PRIMARY KEY NOT NULL,\n"
    "  type TEXT NOT NULL,\n"
    "  depender_id INTEGER NOT NULL,\n"
    "  dependee_id INTEGER NOT NULL,\n"
    "  FOREIGN KEY(depender_id) REFERENCES files(id),\n"
    "  FOREIGN KEY(dependee_id) REFERENCES files(id),\n"
    ");\n";

  sqlite3_stmt *stmt;
  sqlite3_prepare_v2(db, migration, -1, &stmt, NULL);
  yeti_assert(sqlite3_step(stmt) == SQLITE_DONE);
  sqlite3_finalize(stmt);
}

void ResourceDatabase::prepare_statements_() {
}

void ResourceDatabase::cleanup_() {
  this->cleanup_statements_();
}

void ResourceDatabase::cleanup_statements_() {
}

} // yeti
