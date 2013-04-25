// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/compile.h>
#include <bt/resource.h>

namespace bt {
  Compile::Data::Data(
    String& log,
    const String& path,
    const String& source_path,
    const String& data_directory,
    const String& source_data_directory,
    FILE* input,
    FILE* memory_resident_data,
    FILE* streaming_data
  ) : _log(log)
    , _path(path)
    , _src_path(source_path)
    , _data_dir(data_directory)
    , _src_data_dir(source_data_directory)
    , _input(input)
    , _memory_resident_data(memory_resident_data)
    , _streaming_data(streaming_data)
  {
  }

  void Compile::Data::log( const char* format, ... ) const
  {
    va_list va;
    va_start(va, format);
    _log += String(Allocator::scratch(), format, va);
    va_end(va);
  }

  const String& Compile::Data::path() const
  { return _path; }

  const String& Compile::Data::source_path() const
  { return _src_path; }

  const String& Compile::Data::data_directory() const
  { return _data_dir; }

  const String& Compile::Data::source_data_directory() const
  { return _src_data_dir; }

  FILE* Compile::Data::input() const
  { return _input; }

  FILE* Compile::Data::memory_resident_data() const
  { return _memory_resident_data; }

  FILE* Compile::Data::streaming_data() const
  { return _streaming_data; }

  // ============================================================

  Compile::Compile()
    : _status(Unknown)
    , _path(Allocator::heap())
    , _log(Allocator::heap())
  {
  }

  Compile::~Compile()
  {
  }

  bool Compile::a_resource(
    const String& path,
    const String& source_path,
    const String& data_directory,
    const String& source_data_directory,
    size_t revision,
    Compile& compile )
  {
    const String source_path_ext =
      Path::extension(source_path);

    const Resource::Type* type =
      Resource::determine_type_from_extension(source_path_ext.to_ptr());

    compile._status = Unknown;
    compile._path = path;

    if (!type) {
      compile._status = Failed;
      compile._log = String(Allocator::heap(),
        "No resource type associated with the extension `%s`!\n",
        source_path_ext.to_ptr());
      return false;
    }

    const Hash hash = Hash(compile._path);
    
    FILE* input = File::open(source_path.to_ptr(), "rb");
    if (!input) {
      compile._status = Failed;
      compile._log = String(Allocator::heap(),
        "Unable to open `%s`!\n",
        source_path_ext.to_ptr());
      return false;
    }

    const String memory_resident_data_path = String(Allocator::scratch(),
      "%s/%08X%08X%u.memory_resident_data",
      data_directory.to_ptr(),
      (uint32_t)type->associated_extension(), hash, revision);

    FILE* memory_resident_data = File::open(
      memory_resident_data_path.to_ptr(), "wb");

    if (!memory_resident_data) {
      compile._status = Failed;
      compile._log += String(Allocator::scratch(),
        "Unable to create `%s`!\n", memory_resident_data_path.to_ptr());
      return false;
    }

    const String streaming_data_path = String(Allocator::scratch(),
      "%s/%08X%08X%u.streaming_data",
      data_directory.to_ptr(),
      (uint32_t)type->associated_extension(), hash, revision);

    FILE* streaming_data = File::open(
      streaming_data_path.to_ptr(), "wb");

    if (!streaming_data) {
      compile._status = Failed;
      compile._log += String(Allocator::scratch(),
        "Unable to create `%s`!\n", streaming_data_path.to_ptr());
      return false;
    }

    const Data data(
      compile._log, compile._path,
      source_path,
      data_directory, source_data_directory,
      input, memory_resident_data, streaming_data);

    compile._status = type->compile(data);

    const bool no_memory_resident_data = (ftell(memory_resident_data) == 0);
    const bool no_streaming_data = (ftell(streaming_data) == 0);

    fclose(input);
    fclose(memory_resident_data);
    fclose(streaming_data);

    if (compile._status != Successful) {
      File::destroy(memory_resident_data_path.to_ptr());
      File::destroy(streaming_data_path.to_ptr());
      return false;
    }

    if (no_memory_resident_data)
      File::destroy(memory_resident_data_path.to_ptr());

    if (no_streaming_data)
      File::destroy(streaming_data_path.to_ptr());

    return true;
  }
} // bt