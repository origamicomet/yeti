//===-- yeti/foundation/filesystem.h ----------------------*- mode: C++ -*-===//
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
  DIRECTORY = 2,
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
  /// \warning Is not available on some platforms, namely Linux and OS X!
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

  /// Don't allow others to fiddle.
  EXCLUSIVE = 0x80000000,
};

enum Position {
  /// Move from the start or end of a file.
  ABSOLUTE = 1,

  /// Move from current position in the file.
  RELATIVE = 2
};

/// Fills out @info for the file or directory at @path.
extern YETI_PUBLIC bool info(const char *path, fs::Info *info);

/// Returns true if a file (or directory) exists at @path.
extern YETI_PUBLIC bool exists(const char *path);

/// Creates a new file or directory at @path.
/// \note Fails if a file or directory already exists at @path.
///
extern YETI_PUBLIC bool create(fs::Type type, const char *path);

/// Destroys a file or directory at @path.
/// \note Auto-detects the type from @path.
///
extern YETI_PUBLIC bool destroy(const char *path);

/// Destroys a file or directory at @path.
/// \note Fails if @type does not match the entry at @path.
///
extern YETI_PUBLIC bool destroy(fs::Type type, const char *path);

/// User-specified callback for yeti::foundation::fs::walk.
typedef bool (*Walker)(const char *path, const Info *info, void *ctx);

/// Non-recursively walks @directory with @walker until there are no more
/// entires or @walker returns |false|.
extern YETI_PUBLIC bool walk(const char *directory, fs::Walker walker, void *walker_ctx = NULL);

/// Opens an existing file @path with @permissions.
/// \note Fails if there is no file @path.
///
extern YETI_PUBLIC uintptr_t open(const char *path, const fs::Permissions permissions);

/// Closes a handle of a file opened by yeti::foundation::fs::open.
extern YETI_PUBLIC void close(uintptr_t hndl);

/// Synchronously reads up to @in_len bytes from the file @hndl into @in.
/// \returns The number of bytes actually read. Zero indicates failure.
///
extern YETI_PUBLIC u64 read(uintptr_t hndl, uintptr_t in, u64 in_len);

/// Synchronously writes up to @out_len bytes to the file @hndl from @out.
/// \returns The number of bytes actually written. Zero indicates failure.
///
extern YETI_PUBLIC u64 write(uintptr_t hndl, const uintptr_t out, u64 out_len);

/// Moves the read/write position inside the file @hndl. If @pos is
/// fs::ABSOLUTE, then @offset is taken as from the start of the file if
/// positive, or from the end of the file if negative. If @pos is fs::RELATIVE,
/// the @offset is taken from the current position in the file.
/// \returns The number of bytes seeked.
///
extern YETI_PUBLIC i64 seek(uintptr_t hndl, fs::Position pos, i64 offset);

/// Forces all buffered data to be written to the file @hndl.
extern YETI_PUBLIC void flush(uintptr_t hndl);

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
