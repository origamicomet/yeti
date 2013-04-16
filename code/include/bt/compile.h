// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_COMPILE_H_
#define _BUTANE_COMPILE_H_

#include <bt/foundation.h>
#include <bt/config.h>

namespace bt {
  class Compile {
    FOUNDATION_NON_COPYABLE(Compile);

    public:
      enum Status {
        Unknown    = 0,
        Failed     = 1,
        Successful = 2
      };

      class Data {
        FOUNDATION_NON_COPYABLE(Data);

        public:
          Data(
            String& log,
            const String& path,
            const String& source_path,
            const String& data_directory,
            const String& source_data_directory,
            FILE* input,
            FILE* memory_resident_data,
            FILE* streaming_data );

        public:
          void log( const char* format, ... ) const;

          const String& path() const;
          const String& source_path() const;

          const String& data_directory() const;
          const String& source_data_directory() const;

          FILE* input() const;
          FILE* memory_resident_data() const;
          FILE* streaming_data() const;

        private:
          String& _log;
          String _path;
          String _src_path;
          const String& _data_dir;
          const String& _src_data_dir;
          FILE* _input;
          FILE* _memory_resident_data;
          FILE* _streaming_data;
      };

    private:
      Compile();

    public:
      static bool a_resource(
        const String& source_path,
        const String& data_directory,
        const String& source_data_directory,
        Compile& compile );

    private:
      Status _status;
      String _path;
      String _log;
  };
} // bt

#endif // _BUTANE_COMPILE_H_