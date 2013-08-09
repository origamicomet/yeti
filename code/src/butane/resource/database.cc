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
    return false;
  }

  bool Resource::Database::save(
    const char* path )
  {
    assert(path != nullptr);
    return false;
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
