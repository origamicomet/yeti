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
#include "yeti/core.h"

#include "yeti/resource.h"
#include "yeti/resource_manager.h"
#include "yeti/resource_database.h"

namespace yeti {

// Forward declared for `resource_compiler::Environment`.
class ResourceCompiler;

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
    /// Resource compiler driving compilation.
    ResourceCompiler *compiler;

    /// Unique identifier of the resource being compiled.
    Resource::Id id;

    /// Unique identifier for the build.
    Resource::Build::Id build;

    /// Logs an informational message.
    void (*info)(const Environment *env, const char *format, ...);

    /// Logs a warning.
    void (*warning)(const Environment *env, const char *format, ...);

    /// Logs an error message.
    void (*error)(const Environment *env, const char *format, ...);
  };

  struct Input {
    /// Root directory of source data.
    const char *root;

    /// Canonical path of source data.
    const char *path;

    /// Handle to read source data.
    core::File *source;

    /// Digest of source data.
    char fingerprint[41];
  };

  struct Output {
    /// Root directory of compiled data.
    const char *root;

    /// Handle to write memory-resident data.
    core::File *memory_resident_data;

    /// Handle to write streaming data.
    core::File *streaming_data;
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
  static ResourceCompiler *create(const ResourceCompiler::Options &options);
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
  ///
  ///
  Result compile(const char *path, bool force = false);

 private:
  void canonicalize(char *path) const;

 private:
  bool ignorable(const char *path) const;
  bool allowable(const char *path) const;
  bool compilable(const char *path) const;

 private:
  /// \internal Computes fingerprint for @file.
  void fingerprint_for_file(core::File *file, char fingerprint[41]) const;

 private:
  bool walk(const char *path, const core::File::Info *info);

  static bool walker(const char *path,
                     const core::File::Info *info,
                     ResourceCompiler *resource_compiler);

 private:
  void watch(core::fs::Event event, const char *path);

  static void watcher(core::fs::Event event,
                      const char *path,
                      ResourceCompiler *resource_compiler);

 private:
  /// \internal Shims used to bind `forward_to_log`.
  /// @{
  static void info(const Environment *env, const char *format, ...);
  static void warning(const Environment *env, const char *format, ...);
  static void error(const Environment *env, const char *format, ...);
  /// @}

  /// \internal Logs a message and copies to the build log.
  void forward_to_log(const Environment *env,
                      Resource::Build::Log::Level level,
                      const char *format,
                      va_list ap);

 private:
  ResourceDatabase *db_;

  Path data_;
  size_t data_len_;

  Path data_src_;
  size_t data_src_len_;

  // Patterns of source filenames to ignore.
  core::Array<const char *> ignore_;

  // Milliseconds to collect changes prior to processing.
  u32 debounce_;

  // Paths collected by `walker`.
  core::Array<const char *> backlog_;

  // Ture if daemonized.
  bool daemonized_;

  // True when daemonized resource compiler should gracefully stop.
  bool stop_;
};

} // yeti

#endif
