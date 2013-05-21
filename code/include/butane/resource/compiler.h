// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_COMPILER_H_
#define _BUTANE_RESOURCE_COMPILER_H_

class Compiler final {
  __foundation_trait(Compiler, non_copyable);

  public:
    enum Status {
      Successful   = 0,
      Unsuccessful = 1,
      Skipped      = 2
    };

  public:
    typedef void (*Logger)(
      void* closure,
      const char* format, ... );

  public:
    class Stream final {
      __foundation_trait(Stream, non_copyable);

      public:
        Stream(
          FILE* source_data,
          FILE* memory_resident_data,
          FILE* streaming_data );

      public:
        FOUNDATION_INLINE FILE* source_data() const
        { return _source_data; }

        FOUNDATION_INLINE FILE* memory_resident_data() const
        { return _memory_resident_data; }

        FOUNDATION_INLINE FILE* streaming_data() const
        { return _streaming_data; }
      
      private:
        FILE* _source_data;
        FILE* _memory_resident_data;
        FILE* _streaming_data;
    };

  public:
    static Status compile(
      const char* data_dir,
      const char* source_data_dir,
      const char* source_path,
      Logger logger,
      void* closure = nullptr );
};

#endif // _BUTANE_RESOURCE_COMPILER_H_
