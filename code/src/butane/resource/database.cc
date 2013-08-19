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
} // butane

namespace butane {
  Resource::Database::Record::Variation::Variation()
    : _source(allocator())
    , _properties(allocator())
    , _compiled_at(0)
  {}

  Resource::Database::Record::Variation::Variation( const Variation& variation )
    : _source(variation._source)
    , _properties(variation._properties)
    , _compiled_at(variation._compiled_at)
  {}

  Resource::Database::Record::Variation::~Variation()
  {}

  const char* Resource::Database::Record::Variation::source() const
  { return _source.raw(); }

  void Resource::Database::Record::Variation::set_source( const char* source ) {
    assert(source != nullptr);
    _source = String(allocator(), source);
  }

  Array<butane::Resource::Property>& Resource::Database::Record::Variation::properties() {
    return _properties;
  }

  const Array<butane::Resource::Property>& Resource::Database::Record::Variation::properties() const
  { return _properties; }

  uint64_t Resource::Database::Record::Variation::compiled_at() const
  { return _compiled_at; }

  void Resource::Database::Record::Variation::set_compiled_at( const uint64_t at ) {
    _compiled_at = at;
  }

  Resource::Database::Record::Record()
    : _path(allocator())
    , _variations(allocator())
  {}


  Resource::Database::Record::Record( const Record& resource )
    : _path(resource._path)
    , _variations(resource._variations)
  {}

  Resource::Database::Record::~Record()
  {}

  const char* Resource::Database::Record::path() const
  { return _path.raw(); }

  void Resource::Database::Record::set_path( const char* path ) {
    assert(path != nullptr);
    _path = String(allocator(), path);
  }

  const Resource::Database::Record::Variation* Resource::Database::Record::find_variation_by_id(
    const butane::Resource::Variation id ) const {
    for (auto iter = _variations.begin(); iter != _variations.end(); ++iter)
      if ((*iter).key == id)
        return &(*iter).value;
    return nullptr;
  }

  Resource::Database::Record::Variation* Resource::Database::Record::find_or_add_variation_by_id(
    const butane::Resource::Variation id ) {
    for (auto iter = _variations.begin(); iter != _variations.end(); ++iter)
      if ((*iter).key == id)
        return &(*iter).value;
    const size_t num_of_variations = _variations.size();
    _variations.reserve(1);
    _variations[num_of_variations].key = id;
    return &_variations[num_of_variations].value;
  }

  bool Resource::Database::Record::remove_variation_by_id(
    const butane::Resource::Variation id ) {
    for (auto iter = _variations.begin(); iter != _variations.end(); ++iter) {
      if ((*iter).key == id) {
        _variations.remove(iter);
        return true; }}
    return false;
  }

  void Resource::Database::Record::for_each_variation(
    bool (*callback)(
      void* closure,
      const Resource::Variation id,
      const Variation* variation ),
    void* closure ) const
  {
    assert(callback != nullptr);

    for (auto iter = _variations.begin(); iter != _variations.end(); ++iter)
      if (!callback(closure, (*iter).key, &(*iter).value))
        return;
  }
} // butane

namespace butane {
  Resource::Database::Database(
    size_t size
  ) : _records(allocator(), size)
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

    uint32_t num_of_records;
    if (!File::read<uint32_t>(fh, num_of_records)) {
      fclose(fh);
      return nullptr; }

    Resource::Database* db =
      make_new(Database, allocator())(num_of_records + num_of_records / 3);

    while (num_of_records > 0) {
      Resource::Id id;
      if (!File::read(fh, *((uint64_t*)&id)))
        goto failure;
      Record* record;
      if (!db->_records.insert(id, record))
        goto failure;
      /* record->path = */ {
        String path(Allocators::scratch());
        if (!File::read<String>(fh, path))
          goto failure;
        record->set_path(path.raw()); }
      /* record->variations = */ {
        uint32_t num_of_variations;
        if (!File::read<uint32_t>(fh, num_of_variations))
          goto failure;
        while (num_of_variations > 0) {
          Resource::Variation id;
          if (!File::read<uint32_t>(fh, *((uint32_t*)&id)))
            goto failure;
          Record::Variation* variation =
            record->find_or_add_variation_by_id(id);
          /* variation->source = */ {
            String source(Allocators::scratch());
            if (!File::read<String>(fh, source))
              goto failure;
            variation->set_source(source.raw()); }
          /* variation->properties = */ {
            uint32_t num_of_properties;
            if (!File::read<uint32_t>(fh, num_of_properties))
              goto failure;
            variation->properties().resize(num_of_properties);
            while (num_of_properties > 0) {
              Resource::Property property;
              if (!File::read<uint32_t>(fh, *((uint32_t*)&property)))
                goto failure;
              variation->properties()[variation->properties().size() - num_of_properties] = property;
              --num_of_properties; }}
          /* variation->compiled_at = */ {
            uint64_t compiled_at;
            if (!File::read<uint64_t>(fh, compiled_at))
              goto failure;
            variation->set_compiled_at(compiled_at); }
          --num_of_variations; }}
      --num_of_records;
    }

    fclose(fh);
    return db;

  failure:
    fclose(fh);
    make_delete(Database, allocator(), db);
    return nullptr;
  }

  Resource::Database* Resource::Database::create_or_load(
    const char* path )
  {
    Resource::Database* db = Resource::Database::load(path);
    return db ? db : Resource::Database::create(path);
  }

  bool Resource::Database::save(
    const char* path ) const
  {
    assert(path != nullptr);

    const String temporary_path = String::format(Allocators::scratch(), "%s.saving", path);
    FILE* fh = File::open(temporary_path.raw(), "wb");
    if (!fh)
      return false;

    if (!File::write<uint32_t>(fh, _records.load()))
      goto failure;

    for (auto iter = _records.raw().begin(); iter != _records.raw().end(); ++iter) {
      const Resource::Id unoccupied = Resource::Id();
      if ((*iter).key == unoccupied)
        continue;
      if (!File::write<uint64_t>(fh, (*iter).key))
        goto failure;
      const Record& record = (*iter).value;
      if (!File::write<const char*>(fh, record._path.raw()))
        goto failure;
      if (!File::write<uint32_t>(fh, record._variations.size()))
        goto failure;
      for (auto iter_ = record._variations.begin(); iter_ != record._variations.end(); ++iter_) {
        const Record::Variation& variation = (*iter_).value;
        if (!File::write<uint32_t>(fh, (*iter_).key))
          goto failure;
        if (!File::write<const char*>(fh, variation._source.raw()))
          goto failure;
        if (!File::write<uint32_t>(fh, (uint32_t)variation._properties.size()))
          goto failure;
        for (auto property = variation._properties.begin(); property != variation._properties.end(); ++property)
          if (!File::write<uint32_t>(fh, *property))
            goto failure;
        if (!File::write<uint64_t>(fh, variation._compiled_at))
          goto failure;
      }
    }

    fclose(fh);
    File::remove(path);
    File::move(temporary_path.raw(), path);
    return true;

  failure:
    fclose(fh);
    File::remove(temporary_path.raw());
    return false;
  }

  void Resource::Database::close()
  {
    make_delete(Database, allocator(), this);
  }

  Resource::Database::Record* Resource::Database::add(
    const butane::Resource::Id id )
  {
    Record* record;
    return (_records.insert(id, record) ? record : nullptr);
  }

  Resource::Database::Record* Resource::Database::find(
    const butane::Resource::Id id )
  {
    Record* record;
    return (_records.find(id, record) ? record : nullptr);
  }

  Resource::Database::Record* Resource::Database::find_or_add(
    const butane::Resource::Id id )
  {
    Record* record = find(id);
    return (record ? record : add(id));
  }

  void Resource::Database::remove(
    const butane::Resource::Id id )
  {
    _records.remove(id);
  }

  void Resource::Database::for_each(
    bool (*callback)(
      void* closure,
      const Resource::Id id,
      const Resource::Database::Record* record ),
    void* closure ) const
  {
    assert(callback != nullptr);

    for (auto iter = _records.raw().begin(); iter != _records.raw().end(); ++iter) {
      const Resource::Id unoccupied = Resource::Id();
      if ((*iter).key == unoccupied)
        continue;
      if (!callback(closure, (*iter).key, &(*iter).value))
        return;
    }
  }
} // butane
