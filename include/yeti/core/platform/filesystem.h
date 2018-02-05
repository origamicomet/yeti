//===-- yeti/core/platform/filesystem.h -----------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Low-level filesystem access.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_FILESYSTEM_H_
#define _YETI_CORE_PLATFORM_FILESYSTEM_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

#include "yeti/core/containers/array.h"

// TODO(mtwilliams): Memory mapping.
// TODO(mtwilliams): Asynchronous I/O.

namespace yeti {
namespace core {

/// An opaque handle that represents an open file.
struct File {
  /// Type of file.
  enum Type {
    /// A regular ol' file.
    FILE = 1,

    /// A directory.
    DIRECTORY = 2
  };

  /// Basic file attributes.
  struct Info {
    Type type;

    union {
      struct {
        /// Readable.
        u32 read : 1;

        /// Writeable.
        u32 write : 1;

        /// Executable.
        u32 execute : 1;

        /// Reserved for future use.
        u32 reserved : 29;
      };

      u32 mode;
    };

    /// The (logical) size in bytes.
    u64 size;

    /// Created at, in seconds from Unix epoch.
    /// \warning This is not available on some platforms, namely Mac and Linux!
    u64 created_at;

    /// Most recently accessed at, in seconds from Unix epoch.
    u64 last_accessed_at;

    /// Most recently modified at, in seconds from Unix epoch.
    u64 last_modified_at;
  };

  /// File (and directory) permissions.
  enum Permissions {
    /// Read access.
    READ = 0x00000001,

    /// Write access.
    WRITE = 0x00000002,

    /// Don't allow others to read, write, or destroy.
    EXCLUSIVE = 0x80000000
  };

  /// Basis for seeking.
  enum Position {
    /// Move from the beginning or end of the file.
    ABSOLUTE = 1,

    /// Move from current position in the file.
    RELATIVE = 2
  };
};

namespace fs {

/// Fills out @info for the file or directory at @path.
extern YETI_PUBLIC bool info(const char *path, File::Info *info);

/// Fills out @info for the @file.
extern YETI_PUBLIC bool info(File *file, File::Info *info);

/// Returns true if a file (or directory) exists at @path.
extern YETI_PUBLIC bool exists(const char *path);

/// Returns the logical size of @file.
extern YETI_PUBLIC u64 size(File *file);

/// Creates a new file or directory at @path.
///
/// \note Fails if a file or directory already exists at @path.
///
extern YETI_PUBLIC bool create(File::Type type, const char *path);

/// Opens an existing file @path with @permissions.
///
/// \note Fails if there is no file @path.
///
extern YETI_PUBLIC File *open(const char *path, const u32 permissions);

/// Creates or opens (and truncates) a file at @path with @permissions.
extern YETI_PUBLIC File *create_or_open(const char *path, const u32 permissions);

/// Opens (and appends) or creates a file at @path with @permissions.
extern YETI_PUBLIC File *open_or_create(const char *path, const u32 permissions);

/// Closes a @file opened by yeti::core::fs::open.
extern YETI_PUBLIC void close(File *file);

/// Destroys a file or directory at @path.
///
/// \note Auto-detects the type from @path.
///
extern YETI_PUBLIC bool destroy(const char *path);

/// Destroys a file or directory at @path.
///
/// \note Fails if @type does not match the entry at @path.
///
extern YETI_PUBLIC bool destroy(File::Type type, const char *path);

/// Synchronously reads up to @length bytes from the @file into @in.
///
/// \returns The number of bytes actually read. Zero indicates failure.
///
extern YETI_PUBLIC u64 read(File *file, void *in, u64 length);

/// Synchronously reads @file into @buffer.
///
/// \returns The number of bytes read. Zero indicates failure.
///
extern YETI_PUBLIC u64 read_into_buffer(File *file,
                                        Array<u8> &buffer);

/// Synchronously writes up to @length bytes to the @file from @out.
///
/// \returns The number of bytes actually written. Zero indicates failure.
///
extern YETI_PUBLIC u64 write(File *file, const void *out, u64 length);

/// Synchronously writes @buffer to @file.
///
/// \returns The number of bytes written. Zero indicates failure.
///
extern YETI_PUBLIC u64 write_to_file(File *file,
                                     const Array<u8> &buffer);

/// Moves the read/write position inside the @file. If @pos is `File::ABSOLUTE`
/// then @offset is taken as from the start of the file if positive, or from
/// the end of the file if negative. If @pos is `File::RELATIVE` the @offset is
/// taken from the current position in the file.
///
/// \returns The number of bytes seeked.
///
extern YETI_PUBLIC i64 seek(File *file, File::Position pos, i64 offset);

/// Forces all buffered data to be written to the @file.
extern YETI_PUBLIC void flush(File *file);

/// User-specified callback for `yeti::core::fs::walk`.
typedef bool (*Walker)(const char *path, const File::Info *info, void *context);

/// Non-recursively walks @directory with @walker until there are no more
/// entires or @walker returns `false`.
extern YETI_PUBLIC bool walk(const char *directory,
                             fs::Walker walker,
                             void *context = NULL);

/// Type of filesystem event.
enum Event {
  /// A file was created.
  CREATED = 1,

  /// A file was modified.
  MODIFIED = 2,

  /// A file was destroyed.
  DESTROYED = 3
};

/// An opaque handle to a directory being watched.
struct Watch;

/// User-specified callback for `yeti::core::fs::watch`.
typedef void (*Watcher)(fs::Event event, const char *path, void *context);

/// Watches @directory's tree for changes, indicating to @callback what changes
/// occur.
extern YETI_PUBLIC Watch *watch(const char *directory,
                                fs::Watcher callback,
                                void *context = NULL);

/// Must be called periodically for watchers to deliver notifications.
extern YETI_PUBLIC void poll(Watch *watch);

/// Stops watching a directory.
extern YETI_PUBLIC void unwatch(Watch *watch);

} // fs

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_FILESYSTEM_H_
