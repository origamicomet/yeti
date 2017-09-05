//===-- yeti/foundation/filesystem.h --------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_FILESYTEM_H_
#define _YETI_FOUNDATION_FILESYTEM_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

namespace fs {

enum Type {
  /// A regular ol' file.
  FILE = 1,

  /// A directory.
  DIRECTORY = 2
};

struct Info {
  Type type;

  union {
    struct {
      /// (We) can read.
      u32 read : 1;

      /// (We) can write.
      u32 write : 1;

      /// (We) can execute.
      u32 execute : 1;

      /// Reserved for future use.
      u32 reserved : 29;
    };

    u32 mode;
  };

  /// The (logical) size, in bytes.
  u64 size;

  /// Created at, in seconds from Unix epoch.
  /// \warning Is not available on some platforms, namely Mac and Linux!
  u64 created_at;

  /// Most recently accessed at, in seconds from Unix epoch.
  u64 last_accessed_at;

  /// Most recently modified at, in seconds from Unix epoch.
  u64 last_modified_at;
};

enum Permissions {
  /// Read access.
  READ = 0x00000001,

  /// Write access.
  WRITE = 0x00000002,

  /// Don't allow others to read, write, or destroy.
  EXCLUSIVE = 0x80000000
};

enum Position {
  /// Move from the beginning or end of the file.
  ABSOLUTE = 1,

  /// Move from current position in the file.
  RELATIVE = 2
};

enum Event {
  /// A file was created.
  CREATED = 1,

  /// A file was modified.
  MODIFIED = 2,

  /// A file was destroyed.
  DESTROYED = 3
};

/// An opaque handle that represents an open file.
struct File;

/// Fills out @info for the file or directory at @path.
extern YETI_PUBLIC bool info(const char *path, fs::Info *info);

/// Fills out @info for the @file.
extern YETI_PUBLIC bool info(fs::File *file, fs::Info *info);

/// Returns true if a file (or directory) exists at @path.
extern YETI_PUBLIC bool exists(const char *path);

/// Creates a new file or directory at @path.
///
/// \note Fails if a file or directory already exists at @path.
///
extern YETI_PUBLIC bool create(fs::Type type, const char *path);

/// Destroys a file or directory at @path.
///
/// \note Auto-detects the type from @path.
///
extern YETI_PUBLIC bool destroy(const char *path);

/// Destroys a file or directory at @path.
///
/// \note Fails if @type does not match the entry at @path.
///
extern YETI_PUBLIC bool destroy(fs::Type type, const char *path);

/// User-specified callback for yeti::foundation::fs::walk.
typedef bool (*Walker)(const char *path, const Info *info, void *ctx);

/// Non-recursively walks @directory with @walker until there are no more
/// entires or @walker returns `false`.
extern YETI_PUBLIC bool walk(const char *directory,
                             fs::Walker walker,
                             void *walker_ctx = NULL);

/// An opaque handle to a directory being watched.
struct Watch;

/// User-specified callback for `yeti::foundation::fs::watch`.
typedef void (*Watcher)(Event event, const char *path, void *ctx);

/// Watches @directory's tree for changes, indicating to @callback what changes
/// occur.
extern YETI_PUBLIC Watch *watch(const char *directory,
                                fs::Watcher callback,
                                void *callback_ctx = NULL);

// TODO(mtwilliams): Document this.
extern YETI_PUBLIC void poll(Watch *watch);

/// Stops watching a directory.
extern YETI_PUBLIC void unwatch(Watch *watch);

/// Creates or opens a file at @path with @permissions.
extern YETI_PUBLIC fs::File *create_or_open(const char *path, const u32 permissions);

/// Opens an existing file @path with @permissions.
///
/// \note Fails if there is no file @path.
///
extern YETI_PUBLIC fs::File *open(const char *path, const u32 permissions);

/// Closes a @file opened by yeti::foundation::fs::open.
extern YETI_PUBLIC void close(fs::File *file);

/// Synchronously reads up to @in_len bytes from the @file into @in.
///
/// \returns The number of bytes actually read. Zero indicates failure.
///
extern YETI_PUBLIC u64 read(fs::File *file, uintptr_t in, u64 in_len);

/// Synchronously writes up to @out_len bytes to the @file from @out.
///
/// \returns The number of bytes actually written. Zero indicates failure.
///
extern YETI_PUBLIC u64 write(fs::File *file, const uintptr_t out, u64 out_len);

/// Moves the read/write position inside the @file. If @pos is `fs::ABSOLUTE`,
/// then @offset is taken as from the start of the file if positive, or from
/// the end of the file if negative. If @pos is `fs::RELATIVE`, the @offset is
/// taken from the current position in the file.
///
/// \returns The number of bytes seeked.
///
extern YETI_PUBLIC i64 seek(fs::File *file, fs::Position pos, i64 offset);

/// Forces all buffered data to be written to the @file.
extern YETI_PUBLIC void flush(fs::File *file);

namespace async {
  extern YETI_PUBLIC void read();
  extern YETI_PUBLIC void write();
  extern YETI_PUBLIC void wait_for_one();
  extern YETI_PUBLIC void wait_for_all();
}

extern YETI_PUBLIC void map();
extern YETI_PUBLIC void unmap();

} // fs

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_FILESYTEM_H_
