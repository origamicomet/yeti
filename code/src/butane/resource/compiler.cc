// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Compiler::Status Resource::Compiler::compile(
    const char* data_dir,
    const char* source_data_dir,
    const char* source_path,
    Logger logger,
    void* closure )
  {
    const LogScope log_scope("Resource::Compiler::compile");

    assert(data_dir != nullptr);
    assert(source_data_dir != nullptr);
    assert(source_path != nullptr);
    assert(logger != nullptr);

    if (!Directory::exists(data_dir)) {
      if (!Directory::create(data_dir)) {
        warn("Unable to create output directory, aka '%s'!", data_dir);
        return Unsuccessful;
      }
    }

    const Resource::Type* type;
    if (!(type = Resource::Type::determine(source_path)))
      return Skipped;

    const char* relative_path =
      chomp("\\", chomp("/", chomp(source_data_dir, source_path)));

    const String path =
      Path::sans_extension(String(Allocators::scratch(), relative_path));

    const Resource::Id id =
      Resource::Id(*type, path.raw());

    const String streams_dir =
      String::format(Allocators::scratch(), "%s/%016" PRIx64, data_dir, (uint64_t)id);

    if (!Directory::exists(streams_dir.raw())) {
      if (!Directory::create(streams_dir.raw())) {
        warn("Unable to create '%s' for '%s' (%s)!", streams_dir.raw(), path.raw(), type->name().raw());
        return Unsuccessful;
      }
    }

    // TODO: Use a temporary file and copy over.

    FILE* source_data =
      File::open(source_path, "rb");

    if (!source_data) {
      warn("Unable to open source data, aka '%s'!", source_path);
      return Unsuccessful;
    }

    const String memory_resident_data_path =
      String::format(Allocators::scratch(), "%s/memory_resident_data", streams_dir.raw());

    FILE* memory_resident_data =
      File::open(memory_resident_data_path.raw(), "wb");

    if (!memory_resident_data) {
      warn("Unable to create '%s' for '%s' (%s)!", memory_resident_data_path.raw(), path.raw(), type->name().raw());
      return Unsuccessful;
    }

    const String streaming_data_path =
      String::format(Allocators::scratch(), "%s/streaming_data", streams_dir.raw());

    FILE* streaming_data =
      File::open(streaming_data_path.raw(), "wb");

    if (!streaming_data) {
      warn("Unable to create '%s' for '%s' (%s)!", streaming_data_path.raw(), path.raw(), type->name().raw());
      return Unsuccessful;
    }

    Input input;
    input.root = source_data_dir;
    input.path = source_path;
    input.data = source_data;

    Output output;
    output.path = path.raw();
    output.memory_resident_data = memory_resident_data;
    output.streaming_data = streaming_data;

    if (type->compile(input, output)) {
      const bool no_memory_resident_data = (ftell(memory_resident_data) == 0);
      const bool no_streaming_data = (ftell(streaming_data) == 0);

      fclose(source_data);

      fclose(memory_resident_data);
      if (no_memory_resident_data)
        File::remove(memory_resident_data_path.raw());

      fclose(streaming_data);
      if (no_streaming_data)
        File::remove(streaming_data_path.raw());

      if (no_memory_resident_data && no_streaming_data) {
        warn("Compilation of '%s' (%s) resulted in no output!", path.raw(), type->name().raw());
        return Successful;
      }

      log("Compilation of '%s' successful.", path.raw(), type->name().raw());
      return Successful;
    }

    fclose(source_data);

    fclose(memory_resident_data);
    File::remove(memory_resident_data_path.raw());

    fclose(streaming_data);
    File::remove(streaming_data_path.raw());

    Directory::remove(streams_dir.raw());

    log("Compilation of '%s' unsuccessful.", path.raw(), type->name().raw());
    return Unsuccessful;
  }
} // butane
