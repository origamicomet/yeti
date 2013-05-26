// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("resource databases", Allocators::heap());
    return allocator;
  }

  Resource::Database::Database(
    size_t size
  ) : _entries(allocator(), size)
  {}

  Resource::Database::~Database()
  {}

  Resource::Database* Resource::Database::open(
    const char* path )
  {
    assert(path != nullptr);

    if (!File::exists(path))
      return make_new(Database, allocator())();

    FILE* fh = File::open(path, "rb");

    if (!fh)
      return nullptr;

    uint32_t size = 0;
    if (!File::read_in(fh, (void*)&size, sizeof(uint32_t)) || (size == 0)) {
      fclose(fh);
      return nullptr; }

    Database* db =
      make_new(Database, allocator())(size);

    if (!File::read_in(fh, (void*)&db->_entries.raw()[0], size * sizeof(Entry))) {
      fclose(fh);
      return nullptr; }

    fclose(fh);
    return db;
  }

  bool Resource::Database::save(
    const char* path )
  {
    assert(path != nullptr);

    FILE* fh = File::open(path, "wb");

    if (!fh)
      return false;

    const uint32_t size = _entries.raw().size();
    if (!File::write_out(fh, (const void*)&size, sizeof(uint32_t))) {
      fclose(fh);
      File::destroy(path);
      return false; }

    if (!File::write_out(fh, (const void*)&_entries.raw()[0], size * sizeof(Entry))) {
      fclose(fh);
      File::destroy(path);
      return false; }

    fclose(fh);
    return true;
  }

  void Resource::Database::close()
  {
    make_delete(Database, allocator(), this);
  }

  bool Resource::Database::find(
    const Resource::Id id,
    Record& record )
  {
    return _entries.find(id, record);
  }

  bool Resource::Database::update(
    const Resource::Id id,
    const Record& record )
  {
    _entries.remove(id);
    return _entries.insert(id, record);
  }
} // butane
