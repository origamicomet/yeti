// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/db/records/resource.h>

namespace bt {
namespace db {
namespace Records {
  Resource::Resource()
    : _id(0)
  {
  }

  Resource::~Resource()
  {
  }

  Resource* Resource::insert(
    bt::ResourceDatabase* db_,
    const TimeStamp& last_modified,
    const char* path )
  {
    assert(db_ != nullptr);
    assert(path != nullptr);

    int err;
    sqlite3* db = db_->sqlite();
    sqlite3_stmt* stmt;

    static const char* sql = "INSERT INTO (last_modified, path) VALUES ($1, $2)";
    if ((err = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL)) != SQLITE_OK) {
      log("[Resource::insert] unable to prepare statement, err=%d!\n", err);
      return nullptr;
    }

    if ((err = sqlite3_bind_int64(stmt, 1, last_modified)) != SQLITE_OK) {
      log("[Resource::insert] unable to bind id to prepared statement, err=%d!\n", err);
      sqlite3_finalize(stmt);
      return nullptr;
    }

    if ((err = sqlite3_bind_text(stmt, 2, path, strlen(path), SQLITE_TRANSIENT)) != SQLITE_OK) {
      log("[Resource::insert] unable to bind path to prepared statement, err=%d!\n", err);
      sqlite3_finalize(stmt);
      return nullptr;
    }

    Resource* resource = make_new(Resource, Allocator::heap())();
    resource->_db = db_;
    resource->_id = sqlite3_last_insert_rowid(db);
    resource->_last_modified = last_modified;
    resource->_path = path;

    sqlite3_finalize(stmt);
    return resource;
  }

  Resource* Resource::find(
    bt::ResourceDatabase* db_,
    const char* path )
  {
    assert(db_ != nullptr);
    assert(path != nullptr);

    int err;
    sqlite3* db = db_->sqlite();
    sqlite3_stmt* stmt;

    static const char* sql = "SELECT id, last_modified, path FROM resources WHERE path='$1'";
    if ((err = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL)) != SQLITE_OK) {
      log("[Resource::find] unable to prepare statement, err=%d!\n", err);
      return nullptr;
    }

    if ((err = sqlite3_bind_text(stmt, 1, path, strlen(path), SQLITE_TRANSIENT)) != SQLITE_OK) {
      log("[Resource::find] unable to bind path to prepared statement, err=%d!\n", err);
      sqlite3_finalize(stmt);
      return nullptr;
    }

    if (((err = sqlite3_step(stmt)) != SQLITE_ROW) && (err != SQLITE_DONE)) {
      log("[Resource::find] no resource with path=`%s` found, err=%d!\n", path, err);
      sqlite3_finalize(stmt);
      return nullptr;
    }

    Resource* resource = make_new(Resource, Allocator::heap())();
    resource->_db = db_;
    resource->_id = sqlite3_column_int(stmt, 0);
    resource->_last_modified = TimeStamp(sqlite3_column_int(stmt, 1));
    resource->_path = (const char*)sqlite3_column_text(stmt, 2);

    sqlite3_finalize(stmt);
    return resource;
  }

  bool Resource::save()
  {
    int err;
    sqlite3* db = _db->sqlite();
    sqlite3_stmt* stmt;

    static const char* sql = "UPDATE resources SET last_modified=$2 WHERE id=$1";
    if ((err = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL)) != SQLITE_OK) {
      log("[Resource::save] unable to prepare statement, err=%d!\n", err);
      return false;
    }

    if ((err = sqlite3_bind_int(stmt, 1, _id)) != SQLITE_OK) {
      log("[Resource::save] unable to bind id to prepared statement, err=%d!\n", err);
      sqlite3_finalize(stmt);
      return false;
    }

    if ((err = sqlite3_bind_int64(stmt, 2, _last_modified)) != SQLITE_OK) {
      log("[Resource::save] unable to bind last_modified to prepared statement, err=%d!\n", err);
      sqlite3_finalize(stmt);
      return false;
    }

    if ((err = sqlite3_step(stmt)) != SQLITE_DONE) {
      log("[Resource::save] unable to save resource with id=%d, err=%d!\n", _id, err);
      sqlite3_finalize(stmt);
      return false;
    }

    sqlite3_finalize(stmt);
    return true;
  }

  bool Resource::remove()
  {
    int err;
    sqlite3* db = _db->sqlite();
    sqlite3_stmt* stmt;

    static const char* sql = "DELETE FROM resources WHERE id='$1'";
    if ((err = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL)) != SQLITE_OK) {
      log("[Resource::remove] unable to prepare statement, err=%d!\n", err);
      return false;
    }

    if ((err = sqlite3_bind_int(stmt, 1, _id)) != SQLITE_OK) {
      log("[Resource::remove] unable to bind id to prepared statement, err=%d!\n", err);
      sqlite3_finalize(stmt);
      return false;
    }

    if ((err = sqlite3_step(stmt)) != SQLITE_DONE) {
      log("[Resource::remove] unable to remove resource with id=%d, err=%d!\n", _id, err);
      sqlite3_finalize(stmt);
      return false;
    }

    make_delete(Resource, Allocator::heap(), this);
    sqlite3_finalize(stmt);
    return true;
  }

  bool Resource::schema(
    sqlite3* db )
  {
    static const char* sql = "CREATE TABLE resources ("
                             "  id INT NOT NULL,"
                             "  last_modified BIGINT,"
                             "  path CHAR(255),"
                             "  PRIMARY KEY (id)"
                             ")";

    int err;
    sqlite3_stmt* stmt;

    if ((err = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL)) != SQLITE_OK) {
      log("[Resource::schema] unable to prepare statement, err=%d!\n", err);
      return false;
    }

    if ((err = sqlite3_step(stmt)) != SQLITE_DONE) {
      log("[Resource::schema] unable to create table `resources` err=%d!\n", err);
      sqlite3_finalize(stmt);
      return false;
    }

    sqlite3_finalize(stmt);
    return true;
  }
} // Records
} // db
} // bt