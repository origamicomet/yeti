// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RESOURCE_COMPILER_H_
#define _BUTANE_RESOURCE_COMPILER_H_

class BUTANE_EXPORT Compiler final {
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
      const char* format,
      va_list ap );

  public:
    class Log final {
      private:
        friend class Compiler;

      private:
        Log()
          : _logger(nullptr)
          , _closure(nullptr)
        {}

        Log(
          const Logger logger,
          void* closure = nullptr
        ) : _logger(logger)
          , _closure(closure)
        {}

      public:
        FOUNDATION_INLINE void operator() (
          const char* format, ... ) const
        {
          va_list ap;
          va_start(ap, format);
          _logger(_closure, format, ap);
          va_end(ap);
        }

      private:
        Logger _logger;
        void* _closure;
    };

  public:
    struct Input {
      const char* root;
      const char* path;
      FILE* data;
    };

    struct Output {
      Log log;
      const char* path;
      FILE* memory_resident_data;
      FILE* streaming_data;
    };

  public:
    static Status compile(
      const Input& input,
      const Output& output );

  private:
    static void extract_properties_from_path(
      const char* path,
      Array<Resource::Property>& properties );

  public:
    static Status compile(
      const char* source_data_dir,
      const char* data_dir,
      const char* path,
      Resource::Database::Record& record,
      const Log& log );

  public:
    static Status compile_and_reflect_changes_onto_database(
      const char* source_data_dir,
      const char* data_dir,
      const char* path,
      Resource::Database* db,
      const Log& log );

  private:
    static void reflect_filesystem_changes_onto_database(
      const char* source_data_dir,
      const char* data_dir,
      Resource::Database* db );

    static bool is_out_of_date(
      const char* source_data_dir,
      const char* data_dir,
      const char* path,
      uint64_t time,
      Resource::Database* db );

  public:
    static void run(
      const char* source_data_dir,
      const char* data_dir,
      const bool daemon = false );
};

#endif // _BUTANE_RESOURCE_COMPILER_H_
