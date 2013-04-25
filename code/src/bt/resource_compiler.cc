// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/resource_compiler.h>

#include <bt/resource.h>
#include <bt/compile.h>
#include <bt/resource_database.h>

namespace bt {
namespace ResourceCompiler {
  namespace {
    class Resource {
      public:
        // The engine path, and hash (see ResourceCompiler::compile).
        String path;
        Hash path_h;

        // The source data directory path.
        String real_path;

        // When the source data was last modified.
        TimeStamp last_modified;

      public:
        Resource()
          : path_h(nullptr)
          , last_modified(0)
        {}
    };

    struct Instance {
      String source_data_directory;
      String data_directory;
      ResourceDatabase* rdb;
      Array<Resource> queue;
    };
  }

  static bool run( Instance& inst )
  {
    for (auto iter = inst.queue.begin(); iter != inst.queue.end(); ++iter) {
      Resource& res = iter.to_ref();

      const uint32_t revision = 1;

      log("[ResourceCompiler] Compiling `%s` => `%s` (rev=%u).\n",
        iter.to_ref().real_path.to_ptr(),
        iter.to_ref().path.to_ptr(),
        revision);

      Compile compile;
      Compile::a_resource(
        res.path, res.real_path, 
        inst.data_directory, inst.source_data_directory,
        revision, compile);

      if (compile.status() != Compile::Successful) {
        if (compile.log().empty())
          log("[ResourceCompiler] Failed:\n");
        else
          log("[ResourceCompiler] Failed:\n\n%s\n", compile.log().to_ptr());
      }
    }

    return true;
  }

  bool compile(
    const String& data_directory,
    const String& source_data_directory,
    bool daemon )
  {
    if (source_data_directory.empty())
      return false;

    if (data_directory.empty())
      return false;

    if (!Directory::exists(source_data_directory.to_ptr())) {
      log("[ResourceCompiler] invalid source data directory!\n");
      return false;
    }

    if (!Directory::exists(data_directory.to_ptr())) {
      Directory* dir = Directory::create(data_directory.to_ptr());
      
      if (!dir) {
        log("[ResourceCompiler] invalid data directory!\n");
        return false;
      }

      dir->close(); 
    }

    Instance inst;

    inst.source_data_directory = source_data_directory;
    inst.data_directory = data_directory;

    const String rdb = data_directory + "/resource.db";
    if (!(inst.rdb = ResourceDatabase::open(rdb.to_ptr()))) {
      if (!(inst.rdb = ResourceDatabase::create(rdb.to_ptr()))) {
        log("[ResourceCompiler] unable to open or create resource database!\n");
        return false;
      }
    }

    /* Look for any new or updated resources, and build them: */ {
      Array<Directory::Entry> entries;
      if (!Directory::scan(source_data_directory.to_ptr(), entries, true)) {
        log("[ResourceCompiler] unable to perform initial scan!\n");
        inst.rdb->close();
        return false;
      }

      for (auto iter = entries.begin(); iter != entries.end(); ++iter) {
        const Directory::Entry dir_entry = iter.to_ref();

        // Ignore directories:
        if (dir_entry.type != Directory::Entry::FILE)
          continue;

        Resource res;
        res.real_path = String(&dir_entry.path[0]);

        const String real_path_ext =
          Path::extension(res.real_path);
        const bt::Resource::Type* type =
          bt::Resource::determine_type_from_extension(real_path_ext.to_ptr());

        if (!type) {
          const char* path = res.real_path.to_ptr();
          log("[ResourceCompiler] skipping `%s`, unknown file extension.\n",
            /*res.real_path.to_ptr()*/ path);
          continue;
        }
        
        res.path = Path::dirname(res.real_path).chomp(source_data_directory).chomp(Path::seperator());
        if (res.path.length())
          res.path += '/';
        res.path += Path::basename(res.real_path);
        res.path_h = Hash(res.path);
        res.last_modified = dir_entry.last_modified;

        inst.queue.push_back(res);
      }

      run(inst);
    }

    inst.rdb->close();
    return true;
  }
} // ResourceCompiler
} // bt