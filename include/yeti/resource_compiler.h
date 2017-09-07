//===-- yeti/resource_compiler.h ------------------------*- mode: C++11 -*-===//
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

    /// Path to file of ignore patterns.
    Path ignore;

    /// The amount of time, in milliseconds, to collect changes prior to processing.
    /// \note Only applies if daemonized.
    u32 debounce;
  };

  namespace Results {
    /// Possible results of compilation.
    enum _ {
      /// Matched an ignore rule.
      IGNORED = 0,

      /// Bad or reserved path.
      FORBIDDEN = 1,

      /// Don't know how to compile.
      UNCOMPILABLE = 2,

      /// Already up to date.
      SKIPPED = 3,

      /// Successfully compiled.
      SUCCEEDED = 4,

      /// Compilation failed.
      FAILED = 5
    };
  }

  /// \copydoc yeti::resource_compiler::Results::_
  typedef Results::_ Result;

  struct Environment {
    void (*info)(const Environment *env, const char *format, ...);
    void (*warning)(const Environment *env, const char *format, ...);
    void (*error)(const Environment *env, const char *format, ...);
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
 YETI_DISALLOW_COPYING(ResourceCompiler)

 private:
  ResourceCompiler();
  ~ResourceCompiler();

 public:
  /// \copydoc yeti::resource_compiler::Path
  typedef resource_compiler::Path Path;

  /// \copydoc yeti::resource_compiler::Options
  typedef resource_compiler::Options Options;

  /// \copydoc yeti::resource_compiler::Result
  typedef resource_compiler::Result Result;

  /// \copydoc yeti::resource_compiler::Environment
  typedef resource_compiler::Environment Environment;

  /// \copydoc yeti::resource_compiler::Input
  typedef resource_compiler::Input Input;

  /// \copydoc yeti::resource_compiler::Output
  typedef resource_compiler::Output Output;

 public:
  static ResourceCompiler *create(const ResourceCompiler::Options &opts);
  void destroy();

 private:
  void add_ignore_patterns(const char *path);

 public:
  /// Walks source data directory for changes and compiles as necessary.
  ///
  /// \param @force Forces compilation, even if already up to date.
  ///
  void run(bool force = false);

  /// Watches source data for changes and compiles as necessary.
  ///
  /// \warning This does not return. If you want to gracefully stop, you will
  /// need install signal handler(s) that call `stop`.
  ///
  void daemon();

  /// Signals to daemonized resource compiler to stop.
  void stop();

 public:
  /// Compiles a resource.
  ///
  /// \param @path Relative path of source data.
  /// \param @force Forces compilation, even if already up to date.
  ///
  /// \return Result of compilation.
  Result compile(const char *path, bool force = false);

 private:
  void canonicalize(char *path) const;

 private:
  bool ignorable(const char *path) const;
  bool allowable(const char *path) const;
  bool compilable(const char *path) const;

 private:
  bool walk(const char *path, const foundation::fs::Info *info);

  static bool walker(const char *path,
                     const foundation::fs::Info *info,
                     ResourceCompiler *resource_compiler);

 private:
  void watch(foundation::fs::Event event, const char *path);

  static void watcher(foundation::fs::Event event,
                      const char *path,
                      ResourceCompiler *resource_compiler);

 private:
  static void info(const Environment *env, const char *format, ...);
  static void warning(const Environment *env, const char *format, ...);
  static void error(const Environment *env, const char *format, ...);

 private:
  ResourceDatabase *db_;

  Path data_;
  size_t data_len_;

  Path data_src_;
  size_t data_src_len_;

  // Patterns of source filenames to ignore.
  foundation::Array<const char *> ignore_;

  // Milliseconds to collect changes prior to processing.
  u32 debounce_;

  // Paths collected by `walker`.
  foundation::Array<const char *> backlog_;

  // Ture if daemonized.
  bool daemonized_;

  // True when daemonized resource compiler should gracefully stop.
  bool stop_;
};

} // yeti

#endif
