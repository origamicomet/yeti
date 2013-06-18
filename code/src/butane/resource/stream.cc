// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("resource streams", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  Resource::Stream::Stream(
    const Resource::Type& type,
    const Resource::Id id
  ) : _memory_resident_data(nullptr)
    , _memory_resident_data_len(0)
    , _streaming_data(nullptr)
  {
    const LogScope log_scope("Resource::Stream");

    const String streams_dir =
      String::format(Allocators::scratch(), "data/%016" PRIx64, (uint64_t)id);

    if (!Directory::exists(streams_dir.raw()))
      fail("Streams directory, aka '%s', does not exist!", streams_dir.raw());

    const String memory_resident_data_path =
      String::format(Allocators::scratch(), "%s/memory_resident_data", streams_dir.raw());

    FILE* memory_resident_data =
      File::open(memory_resident_data_path.raw(), "rb");

    const String streaming_data_path =
      String::format(Allocators::scratch(), "%s/streaming_data", streams_dir.raw());

    _streaming_data =
      File::open(streaming_data_path.raw(), "rb");

    if (!memory_resident_data && !_streaming_data)
      fail("No streams exist in '%s'!", streams_dir.raw());

    _memory_resident_data =
      File::read(memory_resident_data, allocator(), &_memory_resident_data_len);

    if (!_memory_resident_data)
      fail("Unable to load memory-resident data from '%s'!", streams_dir.raw());

    if (memory_resident_data)
      fclose(memory_resident_data);
  }

  Resource::Stream::~Stream()
  {
    if (_memory_resident_data)
      allocator().free((void*)_memory_resident_data);

    if (_streaming_data)
      fclose(_streaming_data);
  }
} // butane
