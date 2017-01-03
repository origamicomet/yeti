//===-- yeti/resource_compiler.h --------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_RESOURCE_COMPILER_H_
#define _YETI_RESOURCE_COMPILER_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/foundation.h"

#include "yeti/resource.h"
#include "yeti/resource_database.h"
#include "yeti/resource_manager.h"

namespace yeti {

namespace resource_compiler {
  typedef char Path[256];

  struct Options {
    ResourceDatabase *db;

    /// Root directory of source data.
    Path data_src;

    /// Root directory of compiled data.
    Path data;

    // TODO(mtwilliams): Expose the time between scans?

    // For when we switch to `yeti::foundation::fs::watch`.
    // u32 debounce;
  };

  struct Input {
    /// Root directory of source data.
    const char *root;

    /// Canonical path of source data.
    const char *path;

    /// Handle to read source data.
    foundation::fs::File *source;
  };

  struct Output {
    /// Root directory of compiled data.
    const char *root;

    /// Handle to write memory-resident data.
    foundation::fs::File *memory_resident_data;

    /// Handle to write streaming data.
    foundation::fs::File *streaming_data;
  };
}

/// ...
class YETI_PUBLIC ResourceCompiler {
 YETI_DISALLOW_COPYING(ResourceCompiler);

 private:
  ResourceCompiler();
  ~ResourceCompiler();

 public:
  /// \copydoc resource_compiler::Path
  typedef resource_compiler::Path Path;

  /// \copydoc resource_compiler::Options
  typedef resource_compiler::Options Options;

  /// \copydoc resource_compiler::Input
  typedef resource_compiler::Input Input;

  /// \copydoc resource_compiler::Output
  typedef resource_compiler::Output Output;

 public:
  static ResourceCompiler *start(const ResourceCompiler::Options &opts);
  void shutdown();

  bool compilable(const char *path) const;
  bool ignorable(const char *path) const;

  void compile();
  void compile(const char *path);

 private:
  void canonicalize(char *path) const;

 private:
  bool walk(const char *path, const foundation::fs::Info *info);

  static bool walker(const char *path,
                     const foundation::fs::Info *info,
                     ResourceCompiler *resource_compiler);

 private:
  ResourceDatabase *db_;

  Path data_;
  size_t data_len_;
  Path data_src_;
  size_t data_src_len_;

  foundation::Array<const char *> backlog_;
};

} // yeti

#endif
