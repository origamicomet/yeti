// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("resource databases", Allocators::heap());
    return allocator;
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

  Resource::Database* Resource::Database::open(
    const char* path )
  {
    assert(path != nullptr);

    FILE* fh = File::open(path, "rb");
    if (!fh)
      return nullptr;

    uint32_t num_of_entries = 0;
    if (!File::read(fh, (void*)&num_of_entries, 4))
      { fclose(fh); return nullptr; }

    Database* db =
      make_new(Database, allocator())();

    for (uint32_t e = 0; e < num_of_entries; ++e) {
      Record::Serialized serialized;
      if (!File::read(fh, (void*)&serialized, sizeof(Record::Serialized)))
        { make_delete(Database, allocator(), db); fclose(fh); return nullptr; }
      Record record;
      record.path = &serialized.path[0];
      // record.properties = [];
      record.compiled = serialized.compiled;
      db->insert(serialized.id, record);
    }

    fclose(fh);
    return db;
  }

  bool Resource::Database::save(
    const char* path )
  {
    assert(path != nullptr);

    FILE* fh = File::open(path, "wb");
    if (!fh)
      return nullptr;

    uint32_t num_of_entries = _entries.load();
    if (!File::write(fh, (const void*)&num_of_entries, 4))
      { fclose(fh); return false; }

    static const Resource::Id empty;
    for (uint32_t e = 0; e < _entries.raw().size(); ++e) {
      const HashTable<Resource::Id, Record>::Pair& entry = _entries.raw()[e];
      if (entry.key == empty)
        continue;
      Record::Serialized serialized;
      serialized.id = entry.key;
      assert(entry.value.path.size() <= 256);
      assert(!entry.value.path.empty());
      copy((void*)&serialized.path[0], (const void*)entry.value.path.raw(), entry.value.path.size());
      zero((void*)&serialized.path[entry.value.path.size()], 256 - entry.value.path.size());
      serialized.compiled = entry.value.compiled;
      if (!File::write(fh, (const void*)&serialized, sizeof(Record::Serialized)))
        { fclose(fh); return false; }
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
