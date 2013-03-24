// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_FOUNDATION_H_
#define _BUTANE_FOUNDATION_H_

#include <foundation.h>
using namespace foundation;

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

namespace bt {
  extern void install_file_logger( const char* path );
  // extern void install_network_logger( const net::Host& host );

  extern void log( const char* format, va_list ap );
  extern void log( const char* format, ... );
  extern void fail( const char* format, ... );
} // bt

#endif // _BUTANE_FOUNDATION_H_