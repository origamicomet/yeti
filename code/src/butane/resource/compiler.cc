// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resource.h>

namespace butane {
  Resource::Compiler::Status Resource::Compiler::compile(
    const char* source_path,
    const char* data_dir,
    const char* source_data_dir,
    Logger logger,
    void* closure )
  {
    assert(source_path != nullptr);
    assert(data_dir != nullptr);
    assert(source_data_dir != nullptr);
    assert(logger != nullptr);

    // Resource::Type* type;
    // if (!(type = Resource::Type::determine_from_path(source_path)))
    //   return Skipped;

    // const String path =
    //   String(Allocators::scratch(), chomp(source_data_dir, source_path))
    //     .sans_extension();

    return Unsuccessful;
  }
} // butane
