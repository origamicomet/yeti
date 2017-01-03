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

#include "yeti/foundation/filesystem.h"

#include <stdlib.h>
#include <stdio.h>

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
  #undef ABSOLUTE
  #undef RELATIVE
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

// TODO(mtwilliams): Make pretty.
 // https://github.com/origamicomet/yeti/tree/bcb14a305ea603b610a7501707a354fd6ae72c2d
// TODO(mtwilliams): Support unicode paths.
// TODO(mtwilliams): Use Shell32 API?

namespace yeti {
namespace foundation {

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
namespace {
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
    if (permissions & fs::READ)
      desired_access |= GENERIC_READ;
    if (permissions & fs::WRITE)
      desired_access |= GENERIC_WRITE;
    return desired_access;
  }

  static DWORD permissions_to_share_mode(u32 permissions) {
    if (permissions & fs::EXCLUSIVE)
      return 0;
    else
      return FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
  }
}
#endif

bool fs::info(const char *path, fs::Info *info) {
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
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::info(fs::File *file, fs::Info *info) {
  yeti_assert_debug(file != NULL);
  yeti_assert_debug(info != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  FILE_BASIC_INFO basic_info;
  FILE_STANDARD_INFO standard_info;

  if (!::GetFileInformationByHandleEx((HANDLE)file, FileBasicInfo, (LPVOID)&basic_info, sizeof(FILE_BASIC_INFO)))
    return false;

  if (!::GetFileInformationByHandleEx((HANDLE)file, FileStandardInfo, (LPVOID)&standard_info, sizeof(FILE_STANDARD_INFO)))
    return false;

  info->type = standard_info.Directory ? fs::DIRECTORY : fs::FILE;
  info->read = 1;
  info->write = !(basic_info.FileAttributes & FILE_ATTRIBUTE_READONLY);
  // TODO(mtwilliams): Determine if executable without resorting to ::GetBinaryType.
  info->execute = 0;
  info->size = standard_info.EndOfFile.QuadPart;
  info->created_at = file_time_to_secs_since_unix_epoch(basic_info.CreationTime);
  info->last_accessed_at = file_time_to_secs_since_unix_epoch(basic_info.LastAccessTime);
  info->last_modified_at = file_time_to_secs_since_unix_epoch(basic_info.LastWriteTime);

  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::exists(const char *path) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  // BUG(mtwilliams): Does not handle GetFileAttributesA returning
  // INVALID_FILE_ATTRIBUTES when we have insufficient permissions.
   // https://blogs.msdn.microsoft.com/oldnewthing/20071023-00/?p=24713
   // http://mfctips.com/2012/03/26/best-way-to-check-if-file-or-directory-exists/
  return ::GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::create(fs::Type type, const char *path) {
  yeti_assert_debug((type == fs::FILE) || (type == fs::DIRECTORY));
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  switch (type) {
    case fs::FILE: {
      if (HANDLE hndl = ::CreateFileA(path, 0, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) {
        ::CloseHandle(hndl);
        return true;
      } else {
        // Already exists?
        return false;
      }
    }
    case fs::DIRECTORY: {
      return ::CreateDirectoryA(path, NULL) != 0;
    }
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
  return false;
}

bool fs::destroy(const char *path) {
  yeti_assert_debug(path != NULL);

  fs::Info info;
  if (fs::info(path, &info))
    return fs::destroy(info.type, path);

  // Doesn't exist?
  return false;
}

bool fs::destroy(fs::Type type, const char *path) {
  yeti_assert_debug((type == fs::FILE) || (type == fs::DIRECTORY));
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  switch (type) {
    case fs::FILE:
      return ::DeleteFileA(path) != 0;
    case fs::DIRECTORY:
      // TODO(mtwilliams): Should we delete recursively, using SHFileOperation?
      return ::RemoveDirectory(path) != 0;
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

bool fs::walk(const char *directory, fs::Walker walker, void *walker_ctx) {
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

    fs::Info info;
    info.type = (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? fs::DIRECTORY : fs::FILE;
    info.read = 1;
    info.write = !(find_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY);
    /* info.execute = */ {
      DWORD binary_type;
      info.execute = !!::GetBinaryTypeA(&find_data.cFileName[0], &binary_type); }
    /* info.size = */ {
      LARGE_INTEGER size;
      size.LowPart = find_data.nFileSizeLow;
      size.HighPart = find_data.nFileSizeHigh;
      info.size = size.QuadPart; }
    info.created_at = file_time_to_secs_since_unix_epoch(find_data.ftCreationTime);
    info.last_accessed_at = file_time_to_secs_since_unix_epoch(find_data.ftLastAccessTime);
    info.last_modified_at = file_time_to_secs_since_unix_epoch(find_data.ftLastWriteTime);

    char path[256];
    sprintf(&path[0], "%s\\%s", directory, &find_data.cFileName[0]);

    if (!walker(&path[0], &info, walker_ctx))
      break;
  } while (::FindNextFile(find, &find_data) != 0);

  ::FindClose(find);

  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

fs::File *fs::create_or_open(const char *path, const u32 permissions) {
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD desired_access = permissions_to_desired_access(permissions);
  const DWORD share_mode = permissions_to_share_mode(permissions);
  HANDLE hndl = ::CreateFileA(path, desired_access, share_mode, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hndl == INVALID_HANDLE_VALUE)
    return NULL;
  return (fs::File *)hndl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

fs::File *fs::open(const char *path, const u32 permissions) {
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD desired_access = permissions_to_desired_access(permissions);
  const DWORD share_mode = permissions_to_share_mode(permissions);
  HANDLE hndl = ::CreateFileA(path, desired_access, share_mode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hndl == INVALID_HANDLE_VALUE)
    return NULL;
  return (fs::File *)hndl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::close(fs::File *file) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::CloseHandle((HANDLE)file);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

u64 fs::read(fs::File *file, uintptr_t in, u64 in_len) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  yeti_assert_debug(in_len <= 0xFFFFFFFFull);
  // Guaranteed to read |in_len| bytes except in certain circumstances that we
  // won't encounter.
  DWORD read;
  ::ReadFile((HANDLE)file, (LPVOID)in, in_len, &read, NULL);
  return read;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

u64 fs::write(fs::File *file, const uintptr_t out, u64 out_len) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  yeti_assert_debug(out_len <= 0xFFFFFFFFull);
  // Guaranteed to write |out_len| bytes except in certain circumstances that
  // we won't encounter.
  DWORD wrote;
  ::WriteFile((HANDLE)file, (LPCVOID)out, out_len, &wrote, NULL);
  return wrote;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

i64 fs::seek(fs::File *file, fs::Position pos, i64 offset) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  LARGE_INTEGER move; move.QuadPart = offset;
  LARGE_INTEGER moved = { 0, };
  DWORD method;
  if (pos == fs::ABSOLUTE)
    if (offset >= 0)
      method = FILE_BEGIN;
    else
      method = FILE_END;
  else if (pos == fs::RELATIVE)
    method = FILE_CURRENT;
  ::SetFilePointerEx((HANDLE)file, move, &moved, method);
  return moved.QuadPart;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::flush(fs::File *file) {
  yeti_assert_debug(file != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::FlushFileBuffers((HANDLE)file);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

// TODO(mtwilliams): Asynchronous I/O on files.
void fs::async::read() {}
void fs::async::write() {}
void fs::async::wait_for_one() {}
void fs::async::wait_for_all() {}

// TODO(mtwilliams): Memory-mapped I/O on files.
void fs::map() {}
void fs::unmap() {}

} // foundation
} // yeti
