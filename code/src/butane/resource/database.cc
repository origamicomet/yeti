// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  static Allocator* __allocator_initializer() {
    return new ProxyAllocator("resource databases", Allocators::heap());
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  Resource::Database::Database(
    size_t size
  ) : _entries(allocator(), size)
  {}

  Resource::Database::~Database()
  {}

  Resource::Database* Resource::Database::create(
    const char* path )
  {
    assert(path != nullptr);
    if (File::exists(path))
      return nullptr;
    return make_new(Database, allocator())();
  }

  Resource::Database* Resource::Database::load(
    const char* path )
  {
    assert(path != nullptr);

    FILE* fh = File::open(path, "rb");
    if (!fh)
      return nullptr;

    uint32_t num_of_entries = 0;
    if (!File::read(fh, (void*)&num_of_entries, sizeof(uint32_t))) {
      fclose(fh);
      return nullptr; }

    Resource::Database* db =
      make_new(Database, allocator())(num_of_entries + num_of_entries / 3);

    for (uint32_t entry = 0; entry < num_of_entries; ++entry) {
      Record record;
      if (!File::read(fh, (void*)&record, sizeof(Record))) {
        fclose(fh);
        make_delete(Database, allocator(), db);
        return nullptr; }
      db->_entries.insert(record.id, record);
    }

    fclose(fh);
    return db;
  }

  Resource::Database* Resource::Database::create_or_load(
    const char* path )
  {
    Resource::Database* db = Resource::Database::load(path);
    return db ? db : Resource::Database::create(path);
  }

  bool Resource::Database::save(
    const char* path )
  {
    assert(path != nullptr);

    FILE* fh = File::open(path, "wb");
    if (!fh)
      return false;

    uint32_t num_of_entries = _entries.load();
    if (!File::write(fh, (const void*)&num_of_entries, sizeof(uint32_t))) {
      fclose(fh);
      return false; }

    for (auto iter = _entries.raw().begin(); iter != _entries.raw().end(); ++iter) {
      const Resource::Id invalid = Resource::Id();
      if ((*iter).key == invalid)
        continue;
      if (!File::write(fh, (const void*)&(*iter).value, sizeof(Record))) {
        fclose(fh);
        return false; }
    }

    fclose(fh);
    return true;
  }

  void Resource::Database::close()
  {
    make_delete(Database, allocator(), this);
  }

  bool Resource::Database::insert(
    const Resource::Id id,
    const Record& record )
  {
    return _entries.insert(id, record);
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

  bool Resource::Database::remove(
    const Resource::Id id )
  {
    _entries.remove(id);
    return true;
  }

  void Resource::Database::for_each(
    bool (*callback)(
      void* closure,
      const Resource::Id id,
      const Record& record ),
    void* closure )
  {
    assert(callback != nullptr);
    static const Resource::Id empty;
    for (uint32_t e = 0; e < _entries.raw().size(); ++e) {
      const HashTable<Resource::Id, Record>::Pair& entry = _entries.raw()[e];
      if (entry.key == empty)
        continue;
      if (!callback(closure, entry.key, entry.value))
        break;
    }
  }
} // butane
