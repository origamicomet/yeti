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
    Status compile(
      const char* source_path,
      const char* data_dir,
      const char* source_data_dir,
      Logger logger,
      void* closure );
};

#endif // _BUTANE_RESOURCE_COMPILER_H_
