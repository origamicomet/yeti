//===-- yeti/core/platform/filesystem.cc ----------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/filesystem.h"

// TODO(mtwilliams): Allocate from a pool.
#include "yeti/core/allocators/global_heap_allocator.h"

// TODO(mtwilliams): Support unicode paths.
#include "yeti/core/platform/path.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
  #undef ABSOLUTE
  #undef RELATIVE
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  #include <unistd.h>
  #include <fcntl.h>
  #include <dirent.h>
  #include <sys/types.h>
  #include <sys/stat.h>
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

// REFACTOR(mtwilliams): Use Shell32 API?

namespace yeti {
namespace core {

namespace {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  static u64 file_time_to_secs_since_unix_epoch(u64 file_time) {
    return (file_time / 10000000ull) - 11644473600ull;
  }

  static u64 file_time_to_secs_since_unix_epoch(LARGE_INTEGER file_time) {
    return file_time_to_secs_since_unix_epoch(file_time.QuadPart);
  }

  static u64 file_time_to_secs_since_unix_epoch(FILETIME file_time) {
    LARGE_INTEGER file_time_as_li;
    file_time_as_li.LowPart = file_time.dwLowDateTime;
    file_time_as_li.HighPart = file_time.dwHighDateTime;
    return file_time_to_secs_since_unix_epoch(file_time_as_li);
  }

  static DWORD permissions_to_desired_access(u32 permissions) {
    DWORD desired_access = 0;
    if (permissions & File::READ)
      desired_access |= GENERIC_READ;
    if (permissions & File::WRITE)
      desired_access |= GENERIC_WRITE;
    return desired_access;
  }

  static DWORD permissions_to_share_mode(u32 permissions) {
    if (permissions & File::EXCLUSIVE)
      return 0;
    else
      return FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
  }

  static u64 size_of_file(File *file) {
    u64 size;
    yeti_assert(::GetFileSizeEx((HANDLE)file, (LARGE_INTEGER *)&size) != 0);
    return size;
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  static int fd_from_handle(File *file) {
    yeti_assert_debug(file != NULL);
    return reinterpret_cast<i64>(file);
  }

  static int permissions_to_flags(u32 permissions) {
    const u32 masked = permissions & (File::READ | File::WRITE);
    switch (masked) {
      case (File::READ | File::WRITE):
        return O_WRONLY;
      case File::READ:
        return O_RDONLY;
      case File::WRITE:
        return O_WRONLY;
      default:
        YETI_UNREACHABLE();
    }
  }

  static mode_t permissions_to_mode(u32 permissions) {
    mode_t mode = 0;
    if (permissions & File::READ)
      mode |= (S_IRUSR | S_IRGRP | S_IROTH);
    if (permissions & File::WRITE)
      mode |= S_IWUSR;
    return mode;
  }

  static bool info_from_stat(const struct stat *stat, File::Info *info) {
    const bool is_directory = !!S_ISDIR(stat->st_mode);
    const bool is_file = !!S_ISREG(stat->st_mode);

    if (!is_directory && !is_file)
      return false;

    info->type = is_directory ? File::DIRECTORY : File::FILE;

    const uid_t euid = geteuid();
    const uid_t egid = getegid();

    info->read = false; {
      if (stat->st_mode & S_IROTH) {
        info->read = true;
      } else if (stat->st_mode & S_IRGRP) {
        if (egid == stat->st_gid)
          info->read = true;
      } else if (stat->st_mode & S_IRUSR) {
        if (euid == stat->st_uid)
          info->read = true;
      }
    }

    info->write = false; {
      if (stat->st_mode & S_IWOTH) {
        info->write = true;
      } else if (stat->st_mode & S_IWGRP) {
        if (egid == stat->st_gid)
          info->write = true;
      } else if (stat->st_mode & S_IWUSR) {
        if (euid == stat->st_uid)
          info->write = true;
      }
    }

    info->execute = false; {
      if (stat->st_mode & S_IXOTH) {
        info->execute = true;
      } else if (stat->st_mode & S_IXGRP) {
        if (egid == stat->st_gid)
          info->execute = true;
      } else if (stat->st_mode & S_IXUSR) {
        if (euid == stat->st_uid)
          info->execute = true;
      }
    }

    info->size = stat->st_size;

  #if defined(_DARWIN_FEATURE_64_BIT_INODE)
    info->created_at = (u64)stat->st_birthtimespec.tv_sec +
                       (u64)(stat->st_birthtimespec.tv_nsec / 1000000000);
  #else
    info->created_at = 0;
  #endif

    info->last_accessed_at = (u64)stat->st_atimespec.tv_sec +
                             (u64)(stat->st_atimespec.tv_nsec / 1000000000);
    info->last_modified_at = (u64)stat->st_mtimespec.tv_sec +
                             (u64)(stat->st_mtimespec.tv_nsec / 1000000000);

    return true;
  }

  static u64 size_of_file(File *file) {
    struct stat stat;
    yeti_assert(::fstat(fd_from_handle(file), &stat) == 0);
    return stat->st_size;
  }
#endif
}

bool fs::info(const char *path, File::Info *info) {
  yeti_assert_debug(path != NULL);
  yeti_assert_debug(info != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  if (::GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES)
    // Does not exist?
    return false;

  HANDLE hndl = ::CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hndl == INVALID_HANDLE_VALUE)
    return false;

  const bool succeeded = fs::info((File *)hndl, info);

  ::CloseHandle(hndl);

  return succeeded;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  struct stat stat;

  if (::stat(path, &stat) != 0)
    return false;

  return info_from_stat(&stat, info);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::info(File *file, File::Info *info) {
  yeti_assert_debug(file != NULL);
  yeti_assert_debug(info != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  FILE_BASIC_INFO basic_info;
  FILE_STANDARD_INFO standard_info;

  if (!::GetFileInformationByHandleEx((HANDLE)file, FileBasicInfo, (LPVOID)&basic_info, sizeof(FILE_BASIC_INFO)))
    return false;

  if (!::GetFileInformationByHandleEx((HANDLE)file, FileStandardInfo, (LPVOID)&standard_info, sizeof(FILE_STANDARD_INFO)))
    return false;

  info->type = standard_info.Directory ? File::DIRECTORY : File::FILE;
  info->read = 1;
  info->write = !(basic_info.FileAttributes & FILE_ATTRIBUTE_READONLY);
  info->execute = 0;
  info->size = standard_info.EndOfFile.QuadPart;
  info->created_at = file_time_to_secs_since_unix_epoch(basic_info.CreationTime);
  info->last_accessed_at = file_time_to_secs_since_unix_epoch(basic_info.LastAccessTime);
  info->last_modified_at = file_time_to_secs_since_unix_epoch(basic_info.LastWriteTime);

  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  struct stat stat;

  if (::fstat(fd_from_handle(file), &stat) != 0)
    return false;

  return info_from_stat(&stat, info);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::exists(const char *path) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // BUG(mtwilliams): Does not handle GetFileAttributesA returning
  // INVALID_FILE_ATTRIBUTES when we have insufficient permissions.
   // https://blogs.msdn.microsoft.com/oldnewthing/20071023-00/?p=24713
   // http://mfctips.com/2012/03/26/best-way-to-check-if-file-or-directory-exists/
  return (::GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  return (::access(path, F_OK) == 0);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::create(File::Type type, const char *path) {
  yeti_assert_debug((type == File::FILE) || (type == File::DIRECTORY));
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  switch (type) {
    case File::FILE: {
      if (HANDLE hndl = ::CreateFileA(path, 0, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) {
        ::CloseHandle(hndl);
        return true;
      } else {
        // Already exists?
        return false;
      }
    }
    case File::DIRECTORY: {
      return ::CreateDirectoryA(path, NULL) != 0;
    }
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  switch (type) {
    case File::FILE: {
      const mode_t mode = (S_IRUSR | S_IWUSR) | (S_IRGRP) | (S_IROTH);
      const int fd = ::open(path, O_CREAT | O_EXCL | O_TRUNC | O_WRONLY, mode);
      const bool created = (fd >= 0);
      if (created)
        ::close(fd);
      return created;
    }
    case File::DIRECTORY: {
      const mode_t mode = (S_IRWXU) | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH);
      return (::mkdir(path, mode) == 0);
    }
  }
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
  return false;
}

File *fs::open(const char *path, const u32 permissions) {
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD desired_access = permissions_to_desired_access(permissions);
  const DWORD share_mode = permissions_to_share_mode(permissions);

  HANDLE hndl = ::CreateFileA(path, desired_access, share_mode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hndl == INVALID_HANDLE_VALUE)
    return NULL;

  return (File *)hndl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  const int flags = permissions_to_flags(permissions);
  const int exclusivity = (permissions & File::EXCLUSIVE) ? O_EXLOCK : 0;

  const int fd = ::open(path, flags | exclusivity);

  if (fd < 0)
    return NULL;

  return (File *)fd;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

File *fs::create_or_open(const char *path, const u32 permissions) {
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD desired_access = permissions_to_desired_access(permissions);
  const DWORD share_mode = permissions_to_share_mode(permissions);
  HANDLE hndl = ::CreateFileA(path, desired_access, share_mode, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hndl == INVALID_HANDLE_VALUE)
    return NULL;
  return (File *)hndl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  const mode_t mode = permissions_to_mode(permissions) | (S_IRUSR | S_IRGRP | S_IROTH);
  const int flags = permissions_to_flags(permissions);
  const int exclusivity = (permissions & File::EXCLUSIVE) ? O_EXLOCK : 0;
  const int fd = ::open(path, O_CREAT | O_TRUNC | flags | exclusivity, mode);
  if (fd < 0)
    return NULL;
  return (File *)fd;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

File *fs::open_or_create(const char *path, const u32 permissions) {
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD desired_access = permissions_to_desired_access(permissions);
  const DWORD share_mode = permissions_to_share_mode(permissions);
  HANDLE hndl = ::CreateFileA(path, desired_access, share_mode, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hndl == INVALID_HANDLE_VALUE)
    return NULL;
  return (File *)hndl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::close(File *file) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::CloseHandle((HANDLE)file);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  ::close(fd_from_handle(file));
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::destroy(const char *path) {
  yeti_assert_debug(path != NULL);

File::Info info;
  if (fs::info(path, &info))
    return fs::destroy(info.type, path);

  // Doesn't exist?
  return false;
}

bool fs::destroy(File::Type type, const char *path) {
  yeti_assert_debug((type == File::FILE) || (type == File::DIRECTORY));
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  switch (type) {
    case File::FILE:
      return (::DeleteFileA(path) != 0);

    case File::DIRECTORY:
      // TODO(mtwilliams): Delete recursively, using SHFileOperation?
      return (::RemoveDirectory(path) != 0);
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  switch (type) {
    case File::FILE:
      return (::unlink(path) == 0);

    case File::DIRECTORY:
      // TODO(mtwilliams): Delete recursively?
      return (::rmdir(path) == 0);
  }
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

u64 fs::read(File *file, void *in, u64 length) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  yeti_assert_debug(length <= 0xFFFFFFFFull);
  // Guaranteed to read |length| bytes except in certain circumstances that we
  // won't encounter.
  DWORD read;
  ::ReadFile((HANDLE)file, (LPVOID)in, length, &read, NULL);
  return read;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  const ssize_t read = ::read(fd_from_handle(file), (void *)in, length);
  yeti_assert(read >= 0);
  return read;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

u64 fs::read_into_buffer(File *file, Array<u8> &buffer) {
  yeti_assert_debug(file != NULL);
  const u64 size = size_of_file(file);
  buffer.resize(size);
  return size ? fs::read(file, (void *)&buffer[0], size) : 0;
}

u64 fs::write(File *file, const void *out, u64 length) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  yeti_assert_debug(length <= 0xFFFFFFFFull);
  // Guaranteed to write |length| bytes except in certain circumstances that
  // we won't encounter.
  DWORD wrote;
  ::WriteFile((HANDLE)file, (LPCVOID)out, length, &wrote, NULL);
  return wrote;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  const int fd = fd_from_handle(file);
  u64 remaining = length;
  do {
    const ssize_t wrote = ::write(fd, (const void *)out, length);
    yeti_assert(wrote >= 0);
    remaining -= wrote;
  } while (remaining > 0);
  return length;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

u64 fs::write_to_file(File *file,
                      const Array<u8> &buffer) {
  yeti_assert_debug(file != NULL);
  const u64 size = buffer.size();
  return size ? fs::write(file, (void *)&buffer[0], buffer.size()) : 0;
}

i64 fs::seek(File *file, File::Position pos, i64 offset) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  LARGE_INTEGER move;
  move.QuadPart = offset;

  DWORD method;
  if (pos == File::ABSOLUTE)
    if (offset >= 0)
      method = FILE_BEGIN;
    else
      method = FILE_END;
  else if (pos == File::RELATIVE)
    method = FILE_CURRENT;

  LARGE_INTEGER moved = { 0, };
  ::SetFilePointerEx((HANDLE)file, move, &moved, method);

  // TODO(mtwilliams): Force allocation using `SetEndOfFile`.
#if 0
  ::SetEndOfFile((HANDLE)file);
#endif

  return moved.QuadPart;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  // TODO(mtwilliams): We need to force allocation using `F_PREALLOCATE`.
  int fd = fd_from_handle(file);

  int whence;
  if (pos == File::ABSOLUTE)
    if (offset >= 0)
      whence = SEEK_SET;
    else
      whence = SEEK_END;
  else if (pos == File::RELATIVE)
    whence = SEEK_CUR;

  const off_t moved = ::lseek(fd, offset, whence);
  yeti_assert(moved >= 0);

  return moved;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::flush(File *file) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::FlushFileBuffers((HANDLE)file);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  // TODO(mtwilliams): Set `F_FULLFSYNC`?
  ::fsync(fd_from_handle(file));
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::walk(const char *directory,
              fs::Walker walker,
              void *context) {
  yeti_assert_debug(directory != NULL);
  yeti_assert_debug(walker != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  yeti_assert_debug(strlen(directory) <= 253);
  char pattern[256];
  sprintf(&pattern[0], "%s\\*", directory);

  WIN32_FIND_DATA find_data;
  HANDLE find = ::FindFirstFileA(&pattern[0], &find_data);
  if (find == INVALID_HANDLE_VALUE)
    return false;

  do {
    // Ignore '.' and '..' but not .*
    if (find_data.cFileName[0] == '.') {
      if (find_data.cFileName[1] == '\0')
        continue;
      if (find_data.cFileName[1] == '.')
        if (find_data.cFileName[2] == '\0')
          continue;
    }

    File::Info info;

    info.type = (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? File::DIRECTORY : File::FILE;

    info.read = 1;
    info.write = !(find_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
    info.execute = 0;

    LARGE_INTEGER size;
    size.LowPart = find_data.nFileSizeLow;
    size.HighPart = find_data.nFileSizeHigh;
    info.size = size.QuadPart;

    info.created_at = file_time_to_secs_since_unix_epoch(find_data.ftCreationTime);
    info.last_accessed_at = file_time_to_secs_since_unix_epoch(find_data.ftLastAccessTime);
    info.last_modified_at = file_time_to_secs_since_unix_epoch(find_data.ftLastWriteTime);

    char path[256];
    sprintf(&path[0], "%s\\%s", directory, &find_data.cFileName[0]);

    if (!walker(&path[0], &info, context))
      break;
  } while (::FindNextFile(find, &find_data) != 0);

  ::FindClose(find);

  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  // OPTIMIZE(mtwilliams): Use ftw(3) instead?
  // OPTIMIZE(mtwilliams): Set rapid aging to eliminate writes caused by
  // unnecessary tracking of access times.

  DIR *stream = ::opendir(directory);
  if (stream == NULL)
    return false;

  while (dirent *entry = ::readdir(stream)) {
    // Ignore '.' and '..' but not .*
    if (entry->d_name[0] == '.') {
      if (entry->d_name[1] == '\0')
        continue;
      if (entry->d_name[1] == '.')
        if (entry->d_name[2] == '\0')
          continue;
    }

    // TODO(mtwilliams): Handle symbolic links?
    if ((entry->d_type != DT_DIR) && (entry->d_type != DT_REG))
      // Not a directory or file; skip.
      continue;

    char path[256];
    sprintf(&path[0], "%s/%s", directory, &entry->d_name[0]);

    File::Info info;
    yeti_assert(fs::info(&path[0], &info) != false);

    if (!walker(&path[0], &info, context))
      break;
  }

  ::closedir(stream);

  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
namespace fs {
  struct Watch {
    // TODO(mtwilliams): Determine exactly which filters we need.
    static const DWORD FILTER = FILE_NOTIFY_CHANGE_CREATION |
                                FILE_NOTIFY_CHANGE_FILE_NAME |
                                FILE_NOTIFY_CHANGE_SIZE;

    // Handle to root directory.
    HANDLE hndl;

    // Notifications are begot by Overlapped I/O. I know...
    OVERLAPPED io;

    // Since Overlapped I/O is asynchronous, we need a persistent buffer. We
    // make this buffer large enough to prevent notifications from being
    // silently dropped.
    u8 notifications[65535];

    // Path to the root directory. Used to reconstitute paths.
    char root[256];
    size_t root_len;

    // User's callback.
    fs::Watcher callback;
    void *callback_context;

    // Whether or not we should listen for changes to the entire tree.
    bool recursive;

    // Necessary flag to prevent our callback from queuing another read.
    bool stop;

    // Recovers our `Watch` from @lpOverlapped.
    static Watch *recover(LPOVERLAPPED lpOverlapped) {
      static const size_t offset = offsetof(Watch, io);
      return (Watch *)((uintptr_t)lpOverlapped - offset);
    }
  };

  // This is foward declared because |on_read_directory_changes| calls it.
  static bool wait_for_notfications(Watch *watch);

  static void CALLBACK on_read_directory_changes(DWORD dwErrorCode,
                                                 DWORD dwNumberOfBytesTransfered,
                                                 LPOVERLAPPED lpOverlapped)
  {
    Watch *watch = Watch::recover(lpOverlapped);

    if (dwNumberOfBytesTransfered == 0)
      // Notficiations were silently dropped?
      return;

    if (dwErrorCode == ERROR_OPERATION_ABORTED) {
      // Cancelled; clean up after ourselves.

      ::CloseHandle(watch->io.hEvent);
      ::CloseHandle(watch->hndl);

      YETI_DELETE(Watch, core::global_heap_allocator(), watch);

      return;
    }

    if (dwErrorCode == ERROR_SUCCESS) {
      FILE_NOTIFY_INFORMATION *notification;
      DWORD offset = 0;

      char path[255*4+1];
      memcpy((void *)&path[0], (const void *)&watch->root[0], watch->root_len);
      path[watch->root_len] = '/';

      do {
        notification = (FILE_NOTIFY_INFORMATION *)&watch->notifications[offset];
        offset += notification->NextEntryOffset;

        // Reconstitute path.
        const int relative_path_len = ::WideCharToMultiByte(CP_UTF8, 0, &notification->FileName[0], notification->FileNameLength, NULL, 0, NULL, NULL);
        const size_t maximum_relative_path_len = (255*4+1) - (watch->root_len+1);
        yeti_assert_development(relative_path_len <= maximum_relative_path_len);
        ::WideCharToMultiByte(
          CP_UTF8, 0, &notification->FileName[0], notification->FileNameLength,
          &path[watch->root_len+1], (255*4+1) - (watch->root_len+1), NULL, NULL);

        path::unixify(&path[0]);

        Event event;
        switch (notification->Action) {
          case FILE_ACTION_RENAMED_NEW_NAME:
          case FILE_ACTION_ADDED:
            event = CREATED;
          break;

          case FILE_ACTION_RENAMED_OLD_NAME:
          case FILE_ACTION_REMOVED:
            event = DESTROYED;
          break;

          case FILE_ACTION_MODIFIED:
            event = MODIFIED;
          break;
        }

        watch->callback(event, &path[0], watch->callback_context);
      } while (notification->NextEntryOffset != 0);
    }

    if (!watch->stop)
      // Get more notifications.
      wait_for_notfications(watch);
  }

  // Waits for more notifications.
  static bool wait_for_notfications(Watch *watch) {
    yeti_assert_debug(watch != NULL);
    return !!::ReadDirectoryChangesW(
      watch->hndl, (void *)&watch->notifications, sizeof(Watch::notifications),
      watch->recursive, Watch::FILTER,
      NULL, &watch->io,
      &on_read_directory_changes
    );
  }
}
#endif

fs::Watch *fs::watch(const char *directory,
                     fs::Watcher callback,
                     void *context) {
  yeti_assert_debug(directory != NULL);
  yeti_assert_debug(callback != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  static const DWORD desired_access = FILE_LIST_DIRECTORY;
  static const DWORD share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
  static const DWORD flags_and_attrbitues = FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED;

  // BUG(mtwilliams): Potentially uninitialized |path_w|.
  WCHAR path_w[256];
  ::MultiByteToWideChar(CP_UTF8, 0, directory, -1, &path_w[0], 256);

  HANDLE hndl = ::CreateFileW(path_w, desired_access, share_mode, NULL, OPEN_EXISTING, flags_and_attrbitues, NULL);
  if (hndl == INVALID_HANDLE_VALUE)
    return NULL;

  Watch *watch = YETI_NEW(Watch, core::global_heap_allocator());

  watch->hndl = hndl;
  watch->io.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  memset((void *)watch->notifications, 0, sizeof(Watch::notifications));
  strncpy(watch->root, directory, 255);
  watch->root_len = strlen(directory);
  yeti_assert_development(watch->root_len <= 255);
  watch->callback = callback;
  watch->callback_context = context;
  watch->recursive = true;
  watch->stop = false;

  if (wait_for_notfications(watch))
    return watch;

  ::CloseHandle(watch->io.hEvent);
  ::CloseHandle(watch->hndl);

  YETI_DELETE(Watch, core::global_heap_allocator(), watch);

  return NULL;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  // TODO(mtwilliams): Implement using FSEvents.
  return NULL;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::poll(fs::Watch *watch) {
  yeti_assert_debug(watch != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  YETI_TRAP();
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::unwatch(fs::Watch *watch) {
  yeti_assert_debug(watch != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  watch->stop = true;

  // Eventually calls our completion routine one final time with an error code
  // of `ERROR_OPERATION_ABORTED`. That's when we can safely clean up after
  // ourselves.
  ::CancelIo(watch->hndl);

  // Try to force it.
  ::MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  YETI_TRAP();
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // core
} // yeti
