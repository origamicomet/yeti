// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/compile.h>

#include <butane/resource.h>

namespace butane {
namespace Application {
  static void logger(
    void* closure,
    const char* format,
    va_list ap )
  {
    log(format, ap);
  }

  struct ReflectChangesOnDatabaseClosure {
    Resource::Database* db;
    const char* data_dir;
    const char* source_data_dir;
  };

  static bool reflect_changes_on_database_for_each(
    void* closure,
    const Resource::Id id,
    const Resource::Database::Record& record )
  {
    ReflectChangesOnDatabaseClosure* closure_ =
      (ReflectChangesOnDatabaseClosure*)closure;
    const String streams_dir =
      String::format(Allocators::scratch(), "%s/%016" PRIx64, closure_->data_dir, (uint64_t)id);
    if (!Directory::exists(streams_dir.raw()))
      closure_->db->remove(id);
    const Resource::Type* type = Resource::Type::determine(id);
    if (type) {
      const String source =
        String::format(Allocators::scratch(), "%s/%s.%s", closure_->source_data_dir, record.path.raw(), type->associated_file_extension().raw());
      if (!File::exists(source.raw())) {
        Directory::remove(streams_dir.raw(), true);
        closure_->db->remove(id);
      }
    }

    return true;
  }

  static void reflect_changes_on_database(
    Resource::Database* db,
    const char* data_dir,
    const char* source_data_dir )
  {
    assert(db != nullptr);
    assert(data_dir != nullptr);
    assert(source_data_dir != nullptr);

    ReflectChangesOnDatabaseClosure closure;
    closure.db = db;
    closure.data_dir = data_dir;
    closure.source_data_dir = source_data_dir;

    db->for_each(&reflect_changes_on_database_for_each, (void*)&closure);
  }

  static Resource::Id id_from_source_path(
    const Resource::Type* type,
    const char* data_dir,
    const char* source_data_dir,
    const char* source_path )
  {
    const char* relative_path =
      chomp("\\", chomp("/", chomp(source_data_dir, source_path)));

    const String path =
      Path::sans_extension(String(Allocators::scratch(), relative_path));

    return Resource::Id(*type, path.raw());
  }

  static void compile(
    Resource::Database* database,
    time_t last_modified,
    const char* data_dir,
    const char* source_data_dir,
    const char* source_path )
  {
    const Resource::Type* type;
    if (!(type = Resource::Type::determine(source_path)))
      return;

    const Resource::Id id =
      id_from_source_path(type, data_dir, source_data_dir, source_path);

    /* Skip up to date resources: */ {
      Resource::Database::Record record;
      if (database->find(id, record) && (last_modified <= record.compiled))
        return;
    }

    if (Resource::Compiler::compile(data_dir, source_data_dir, source_path, &logger) != Resource::Compiler::Successful) {
      database->remove(id);
      return;
    }

    Resource::Database::Record record;
    record.path = Path::sans_extension(String(Allocators::scratch(),
      chomp("\\", chomp("/", chomp(source_data_dir, source_path)))));
    record.compiled = last_modified;
    database->update(id, record);
  }

  struct Event {
    Directory::Entry entry;
    Hash<uint32_t, murmur_hash> hash;

    public:
      FOUNDATION_INLINE bool operator< ( const Event& e ) const
      { return (hash < e.hash); }

      FOUNDATION_INLINE bool operator> ( const Event& e ) const
      { return (hash > e.hash); }

      FOUNDATION_INLINE bool operator== ( const Event& e ) const
      { return (hash == e.hash); }

      FOUNDATION_INLINE bool operator!= ( const Event& e ) const
      { return (hash != e.hash); }
  };

  static void on_directory_event(
    Array<Event>* events,
    Directory::Event event,
    const char* path )
  {
    if ((event != Directory::CREATED) && (event != Directory::MODIFIED))
      return;
    Event event_;
    copy((void*)&event_.entry.path[0], (const void*)&path[0], min(sizeof(event_.entry.path) - 1, strlen(path) + 1));
    event_.entry.path[sizeof(event_.entry.path) - 1] = '\0';
    event_.entry.last_modified = time(nullptr);
    event_.hash = Hash<uint32_t, murmur_hash>(&event_.entry.path[0]);
    (*events) += event_;
  }

  void compile(
    const Array<const char*>& args )
  {
    const LogScope _("Application::compile");

    if (args.size() < 2) {
      log("Malformed compiler invocation!");
      log("  Expected: compile [source data directory] [data directory] [options]");
      log("                              ^---------------------^-- not supplied");
      Application::quit(); }

    if (!Directory::exists(args[1])) {
      log("Malformed compiler invocation!");
      log("  Expected: compile [source data directory] [data directory] [option]");
      log("                              ^ directory doesn't exist");
      Application::quit(); }

    bool daemon = false;
    for (auto iter = args.begin(); iter != args.end(); ++iter) {
      if (strcmp("-daemon", *iter) == 0) {
        daemon = true;
      } else if (strcmp("-log-to-network", *iter) == 0) {
        if ((++iter) == args.end()) {
          log("Malformed command-line argument!");
          log("  Expected: -log-to-network [address/hostname]");
          log("                                    ^ not supplied");
          Application::quit(); }
        warn("Network logging is not implemented, yet.");
      }
    }

    const String database_path =
      String::format(Allocators::scratch(), "%s/resource.db", args[2]);

    Resource::Database* database =
      Resource::Database::open(database_path.raw());

    if (!database)
      database = Resource::Database::create(database_path.raw());
    else
      reflect_changes_on_database(database, args[2], args[1]);

    if (!database)
      fail("Unable to open or create resource database, aka '%s'", database_path.raw());

    /* Initially compile new or modified resources since the last invocation. */ {
      Array<Directory::Entry> entries(Allocators::heap());
      if (!Directory::scan(args[1], entries))
        fail("Unable to scan source directory, aka '%s'", args[1]);
      for (auto iter = entries.begin(); iter != entries.end(); ++iter)
        compile(database, (*iter).last_modified, args[2], args[1], &(*iter).path[0]);
      database->save(database_path.raw());
    }

    /* Then daemonize (watch for file modification). */
    if (daemon) {
      Directory::Watched* watched;
      Array<Event> events(Allocators::heap());
      if (!Directory::watch(args[1], (Directory::OnEventHandler)&on_directory_event, (void*)&events))
        fail("Unable to watch source directory, aka '%s'", args[1]);
      log("Watching '%s' for changes...", args[1]);
      while (true) {
        Timer timer;
        while (timer.miliseconds() <= 500) {
          Directory::poll(watched); }
        sort(events.raw(), events.size());
        for (size_t event = 0; event < events.size(); ++event) {
          if (event > 0) {
            if (events[event].hash == events[event - 1].hash)
              continue; }
          compile(database, events[event].entry.last_modified, args[2], args[1], &events[event].entry.path[0]); }
        if (events.size() > 0)
          database->save(database_path.raw());
        events.resize(0);
      }
    }

    database->close();
    Application::quit();
  }
} // Application
} // butane
