// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/foundation.h>

namespace bt {
  static FILE* _file_log = nullptr;

  void install_file_logger( const char* path )
  {
    if (_file_log)
      fclose(_file_log);

    _file_log = fopen(path, "ab");
    if (_file_log)
      setvbuf(_file_log, nullptr, _IONBF, 0);
  }

  static void log_to_file( const char* format, va_list ap ) {
    if (!_file_log)
      return;
    vfprintf(_file_log, format, ap);
  }

  // extern void install_network_logger( const net::Host& host );
  // static void log_to_network( const char* format, va_list ap );

  void log( const char* format, va_list ap )
  {
    static const int _unused = setvbuf(stdout, nullptr, _IONBF, 0);
    vfprintf(stdout, format, ap);
    log_to_file(format, ap);
    // log_to_network(format, ap);
  }

  void log( const char* format, ... )
  {
    va_list ap;
    va_start(ap, format);
    log(format, ap);
    va_end(ap);
  }

  void fail( const char* format, ... )
  {
    va_list ap;
    va_start(ap, format);
    log(format, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
  }
} // bt