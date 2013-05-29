// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/compile.h>

#include <butane/resource.h>

namespace butane {
namespace Application {
  static void on_compiler_log(
    void* closure,
    const char* format, ... )
  {
    va_list ap;
    va_start(format, ap);
    log(format, ap);
    va_end(ap);
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
      if (database->find(id, record) && (last_modified <= record.last_modified))
        return;
    }

    if (Resource::Compiler::compile(data_dir, source_data_dir, source_path, &on_compiler_log) != Resource::Compiler::Successful) {
      // Force a recompile by marking it as out of date.
      last_modified = 0;
    }

    Resource::Database::Record record;
    record.last_modified = last_modified;
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
    const LogScope log_scope("Application::compile");

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
