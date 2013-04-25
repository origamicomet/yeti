// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_COMPILER_H_
#define _BUTANE_RESOURCE_COMPILER_H_

#include <bt/foundation.h>
#include <bt/config.h>

namespace bt {
namespace ResourceCompiler {
  extern bool compile(
    const String& data_directory,
    const String& source_data_directory,
    bool daemon = false );
} // ResourceCompiler
} // bt

#endif // _BUTANE_RESOURCE_COMPILER_H_