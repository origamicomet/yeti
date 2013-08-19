// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Compiler::Status Resource::Compiler::compile(
    const Input& input,
    const Output& output )
  {
    const Resource::Type* type;
    if (!(type = Resource::Type::determine(input.path)))
      return Skipped;

    if (!type->compile(input, output))
      return Unsuccessful;

    const bool has_memory_resident_data = (ftell(output.memory_resident_data) != 0);
    const bool has_streaming_data = (ftell(output.streaming_data) != 0);

    if (!has_memory_resident_data && !has_streaming_data)
      return Unsuccessful;

    return Successful;
  }

  void Resource::Compiler::extract_properties_from_path(
    const char* path,
    Array<Resource::Property>& properties )
  {
    assert(path != nullptr);

    const char* iter = path;
    while (true) {
      const char* seperator = foundation::find(iter, ".");
      const char* min = foundation::next(seperator);
      const char* next_seperator = foundation::find(min, ".");
      const char* max = next_seperator;
      if (!seperator || !next_seperator || !min || !max) break;
      const size_t property_len = (max - min);
      String extracted = String(Allocators::scratch(), property_len + 1);
      copy((void*)extracted.raw(), (const void*)min, property_len);
      extracted.raw()[property_len] = '\0';
      properties += Resource::Property(extracted);
      iter = next_seperator;
    }
  }

  Resource::Compiler::Status Resource::Compiler::compile(
    const char* source_data_dir,
    const char* data_dir,
    const char* path,
    Resource::Database::Record& record,
    const Log& log )
  {
    assert(source_data_dir != nullptr);
    assert(data_dir != nullptr);
    assert(path != nullptr);

    const char* relative_path =
      chomp("\\", chomp("/", chomp(source_data_dir, path)));

    const Resource::Type* type = Resource::Type::determine(relative_path);
    if (!type)
      return Skipped;

    const String relative_path_sans_ext_and_properties =
      Path::sans_extension(String(Allocators::scratch(), relative_path), true);

    log("Compiling '%s' into '%s'", relative_path, relative_path_sans_ext_and_properties.raw());

    const Resource::Id id =
      Resource::Id(*type, relative_path_sans_ext_and_properties);

    log(" type = '%s'", type->name().raw());
    log(" id = %016" PRIx64, (uint64_t)id);

    Array<Resource::Property> properties(Allocators::heap());
    Resource::Compiler::extract_properties_from_path(relative_path, properties);

    if (properties.size() > 32) {
      log("Too many properties specified (a maximum of 32 are supported).");
      return Unsuccessful; }

    if (properties.size() > 0) {
      log(" properties = ");
      for (size_t property = 0; property < properties.size(); ++property)
        log(" [%u] = %08x", (uint32_t)property, (uint32_t)properties[property]);
    }

    const Resource::Variation variation =
      Resource::determine_variation_based_on_properties(properties);

    log(" variation = %08x", (uint32_t)variation);

    record.set_path(relative_path_sans_ext_and_properties.raw());
    Resource::Database::Record::Variation* variation_ =
      record.find_or_add_variation_by_id(variation);
    variation_->set_source(relative_path);
    variation_->properties() = properties;
    variation_->set_compiled_at(0);

    const String resource_dir = String::format(Allocators::scratch(),
      "%s/%016" PRIx64, data_dir, (uint64_t)id);

    const String variation_dir = String::format(Allocators::scratch(),
      "%s/%08x", resource_dir.raw(), variation);

    if (!Directory::exists(resource_dir.raw())) {
      if (!Directory::create(resource_dir.raw())) {
        log("Unable to create output directory '%s'!", resource_dir.raw());
        return Unsuccessful;
      }
    }

    if (!Directory::exists(variation_dir.raw())) {
      if (!Directory::create(variation_dir.raw())) {
        log("Unable to create output directory '%s'!", variation_dir.raw());
        return Unsuccessful;
      }
    }

    const String memory_resident_data_path =
      String::format(Allocators::scratch(), "%s/memory_resident_data", variation_dir.raw());

    FILE* memory_resident_data =
      File::open(memory_resident_data_path.raw(), "wb");

    if (!memory_resident_data) {
      log("Unable to create output stream '%s'!", memory_resident_data_path.raw());
      return Unsuccessful;
    }

    const String streaming_data_path =
      String::format(Allocators::scratch(), "%s/streaming_data", variation_dir.raw());

    FILE* streaming_data =
      File::open(streaming_data_path.raw(), "wb");

    if (!streaming_data) {
      log("Unable to create output stream '%s'!", streaming_data_path.raw());
      return Unsuccessful;
    }

    Input input;
    input.root = source_data_dir;
    input.path = path;
    input.data = File::open(path, "rb");

    Output output;
    output.log = log;
    output.path = relative_path_sans_ext_and_properties.raw();
    output.memory_resident_data = memory_resident_data;
    output.streaming_data = streaming_data;

    const Status status = Resource::Compiler::compile(input, output);
    const bool has_memory_resident_data = (ftell(output.memory_resident_data) != 0);
    const bool has_streaming_data = (ftell(output.streaming_data) != 0);

    fclose(input.data);
    fclose(output.memory_resident_data);
    fclose(output.streaming_data);

    if (status != Successful) {
      File::remove(memory_resident_data_path.raw());
      File::remove(streaming_data_path.raw());
      Directory::remove(variation_dir.raw());
      record.remove_variation_by_id(variation);
      log("Compilation was unsuccessful!");
    } else {
      if (!has_memory_resident_data)
        File::remove(memory_resident_data_path.raw());
      if (!has_streaming_data)
        File::remove(streaming_data_path.raw());
      variation_->set_compiled_at(time(NULL));
      log("Compilation was successful (finished at %016" PRIx64 ")!", variation_->compiled_at());
    }

    return status;
  }

  Resource::Compiler::Status Resource::Compiler::compile_and_reflect_changes_onto_database(
    const char* source_data_dir,
    const char* data_dir,
    const char* path,
    Resource::Database* db,
    const Log& log )
  {
    assert(source_data_dir != nullptr);
    assert(data_dir != nullptr);
    assert(path != nullptr);
    assert(db != nullptr);

    Resource::Id id; {
      const char* relative_path =
        chomp("\\", chomp("/", chomp(source_data_dir, path)));
      const Resource::Type* type = Resource::Type::determine(relative_path);
      if (!type) return Skipped;
      const String relative_path_sans_ext_and_properties =
        Path::sans_extension(String(Allocators::scratch(), relative_path), true);
      id = Resource::Id(*type, relative_path_sans_ext_and_properties);
    }

    return Resource::Compiler::compile(
      source_data_dir, data_dir, path, *db->find_or_add(id), log);
  }

  void Resource::Compiler::reflect_filesystem_changes_onto_database(
    const char* source_data_dir,
    const char* data_dir,
    Resource::Database* db )
  {
    struct ReflectFilesystemChangesOntoDatabase {
      private:
        struct PerVariation {
          public:
            static bool reflect(
              void* closure,
              const Resource::Variation id,
              const Resource::Database::Record::Variation* variation )
            {
              PerVariation* pv = (PerVariation*)closure;

              const String variation_dir = String::format(Allocators::scratch(),
               "%s/%08x", pv->resource_dir, id);

              Resource::Database::Record* record =
                pv->rfcodb->db->find(pv->resource_id);

              if (!Directory::exists(variation_dir.raw())) {
                record->remove_variation_by_id(id);
                return true; }

              const String source = String::format(Allocators::scratch(),
                "%s/%s", pv->rfcodb->source_data_dir, variation->source());

              if (!File::exists(source.raw())) {
                Directory::remove(variation_dir.raw(), true);
                record->remove_variation_by_id(id);
                return true; }

              return true;
            }

          public:
            ReflectFilesystemChangesOntoDatabase* rfcodb;
            Resource::Id resource_id;
            const char* resource_dir;
        };

      public:
        static bool reflect(
          void* closure,
          const Resource::Id id,
          const Resource::Database::Record* record )
        {
          ReflectFilesystemChangesOntoDatabase* rfcodb =
            (ReflectFilesystemChangesOntoDatabase*)closure;

          const String resource_dir = String::format(Allocators::scratch(),
            "%s/%016" PRIx64, rfcodb->data_dir, (uint64_t)id);

          if (!Directory::exists(resource_dir.raw())) {
            rfcodb->db->remove(id);
            return true; }

          PerVariation pv;
          pv.rfcodb = rfcodb;
          pv.resource_id = id;
          pv.resource_dir = resource_dir.raw();

          record->for_each_variation(&PerVariation::reflect, (void*)&pv);

          return true;
        }

      public:
        const char* data_dir;
        const char* source_data_dir;
        Resource::Database* db;
    };

    ReflectFilesystemChangesOntoDatabase rfcodb;
    rfcodb.data_dir = data_dir;
    rfcodb.source_data_dir = source_data_dir;
    rfcodb.db = db;

    db->for_each(&ReflectFilesystemChangesOntoDatabase::reflect, (void*)&rfcodb);
  }

  bool Resource::Compiler::is_out_of_date(
    const char* source_data_dir,
    const char* data_dir,
    const char* path,
    uint64_t time,
    Resource::Database* db )
  {
    assert(source_data_dir != nullptr);
    assert(data_dir != nullptr);
    assert(path != nullptr);

    Resource::Id id; Resource::Variation variation; {
      const char* relative_path =
        chomp("\\", chomp("/", chomp(source_data_dir, path)));
      const Resource::Type* type = Resource::Type::determine(relative_path);
      if (!type) return false;
      const String relative_path_sans_ext_and_properties =
        Path::sans_extension(String(Allocators::scratch(), relative_path), true);
      id = Resource::Id(*type, relative_path_sans_ext_and_properties);
      Array<Resource::Property> properties(Allocators::heap());
      Resource::Compiler::extract_properties_from_path(relative_path, properties);
      if (properties.size() > 32)
        return true;
      variation = Resource::determine_variation_based_on_properties(properties);
    }

    const Resource::Database::Record* record = db->find(id);
    if (!record) return true;
    const Resource::Database::Record::Variation* variation_ =
      record->find_variation_by_id(variation);
    if (!variation_) return true;
    return (variation_->compiled_at() < time);
  }

  void Resource::Compiler::run(
    const char* source_data_dir,
    const char* data_dir,
    bool daemon )
  {
    assert(source_data_dir != nullptr);
    assert(data_dir != nullptr);

    if (!Directory::exists(data_dir))
      if (!Directory::create(data_dir))
        fail("Unable to create output data directory!");

    if (!Directory::exists(source_data_dir))
      fail("Invalid source data directory!");

    const String database_path =
      String::format(Allocators::scratch(), "%s/resource.db", data_dir);
    Resource::Database* db =
      Resource::Database::create_or_load(database_path.raw());
    if (!db)
      fail("Unable to find or create resource database!");

    /* Updated the database to reflect delete source or compiled data. */ {
      Resource::Compiler::reflect_filesystem_changes_onto_database(
        source_data_dir, data_dir, db);
    }

    struct Console {
      public:
        static void log(
          void* closure,
          const char* format,
          va_list ap )
        {
          foundation::log(format, ap);
        }
    };

    /* Compile new or modified resources since the last invocation. */ {
      Array<Directory::Entry> entries(Allocators::heap());
      if (!Directory::scan(source_data_dir, entries))
        fail("Unable to scan source data directory!");
      for (auto iter = entries.begin(); iter != entries.end(); ++iter) {
        const bool out_of_date = Resource::Compiler::is_out_of_date(
          source_data_dir, data_dir,
          &(*iter).path[0], (*iter).last_modified, db);
        if (!out_of_date)
          continue;
        Resource::Compiler::compile_and_reflect_changes_onto_database(
          source_data_dir, data_dir,
          &(*iter).path[0],
          db, Log(&Console::log));
      }
    }

    struct Daemon {
      public:
        struct Event {
          Hash<uint32_t, murmur_hash> hash;
          char path[256];
        };

      public:
        Daemon()
          : events(Allocators::heap())
        {}

      public:
        static void on_directory_event(
          Daemon* daemon,
          Directory::Event event,
          const char* path )
        {
          if ((event != Directory::CREATED) && (event != Directory::MODIFIED))
            return;
          Event event_;
          copy((void*)&event_.path, (const void*)&path[0], strlen(path) + 1);
          event_.hash = Hash<uint32_t, murmur_hash>(&path[0]);
          daemon->events += event_;
        }

      public:
        Array<Event> events;
    };

    /* Then watch for source data modifications. */ if (daemon) {
      Daemon daemon_;
      Directory::Watched* watched;
      if (!Directory::watch(source_data_dir, (Directory::OnEventHandler)&Daemon::on_directory_event, (void*)&daemon_))
        fail("Unable to watch source data directory!");
      while(true) {
        Timer timer;
        while (timer.miliseconds() <= 300)
          Directory::poll(watched);
        size_t num_of_compilations = 0;
        for (size_t event = 0; event < daemon_.events.size(); ++event) {
          for (size_t event_ = event + 1; event_ < daemon_.events.size(); ++event_)
            if (daemon_.events[event].hash == daemon_.events[event_].hash)
              goto duplicate;
          Resource::Compiler::compile_and_reflect_changes_onto_database(
            source_data_dir, data_dir,
            &daemon_.events[event].path[0],
            db, Log(&Console::log));
          ++num_of_compilations;
        duplicate:
          continue; }
        daemon_.events.resize(0);
        if (num_of_compilations > 0)
          db->save(database_path.raw());
      }
    }

    db->save(database_path.raw());
    db->close();
  }
} // butane
