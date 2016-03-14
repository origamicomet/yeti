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

  static LARGE_INTEGER FileTimeToUnixTime(LARGE_INTEGER ftime) {
    LARGE_INTEGER unix_time;
    unix_time.QuadPart = (ftime.QuadPart / 10000000LL) - 11644473600LL;
    return unix_time;
  }

  static LARGE_INTEGER FileTimeToUnixTime(FILETIME ftime) {
    LARGE_INTEGER ftime_as_li;
    ftime_as_li.LowPart = ftime.dwLowDateTime;
    ftime_as_li.HighPart = ftime.dwHighDateTime;
    return ::FileTimeToUnixTime(ftime_as_li);
  }
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

// TODO(mtwilliams): Support unicode paths.
// TODO(mtwilliams): Use Shell32 API?

namespace yeti {
namespace foundation {

bool fs::info(const char *path, fs::Info *info) {
  yeti_assert_debug(path != NULL);
  yeti_assert_debug(info != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  DWORD attributes = ::GetFileAttributesA(path);
  if (attributes == INVALID_FILE_ATTRIBUTES)
    return false;

  DWORD binary_type;
  BOOL is_executable = ::GetBinaryTypeA(path, &binary_type);

  HANDLE hndl = ::CreateFileA(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hndl == INVALID_HANDLE_VALUE)
    return false;

  FILE_BASIC_INFO basic_info;
  if (!::GetFileInformationByHandleEx(hndl, FileBasicInfo, (LPVOID)&basic_info, sizeof(FILE_BASIC_INFO)))
    goto failed;

  FILE_STANDARD_INFO standard_info;
  if (!::GetFileInformationByHandleEx(hndl, FileStandardInfo, (LPVOID)&standard_info, sizeof(FILE_STANDARD_INFO)))
    goto failed;

  info->type = standard_info.Directory ? fs::DIRECTORY : fs::FILE;
  info->read = 1;
  info->write = !(attributes & FILE_ATTRIBUTE_READONLY);
  info->execute = is_executable ? 1 : 0;
  info->size = standard_info.EndOfFile.LowPart;
  info->created_at = ::FileTimeToUnixTime(basic_info.CreationTime).QuadPart;
  info->last_accessed_at = ::FileTimeToUnixTime(basic_info.LastAccessTime).QuadPart;
  info->last_modified_at = ::FileTimeToUnixTime(basic_info.LastWriteTime).QuadPart;

succeeded:
  return true;

failed:
  ::CloseHandle(hndl);
  return false;
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
    info.created_at = ::FileTimeToUnixTime(find_data.ftCreationTime).QuadPart;
    info.last_accessed_at = ::FileTimeToUnixTime(find_data.ftLastAccessTime).QuadPart;
    info.last_modified_at = ::FileTimeToUnixTime(find_data.ftLastWriteTime).QuadPart;

    if (!walker(&find_data.cFileName[0], &info, walker_ctx))
      break;
  } while (::FindNextFile(find, &find_data) != 0);

  ::FindClose(find);

  return true;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

uintptr_t fs::open(const char *path, const fs::Permissions permissions) {
  yeti_assert_debug(path != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  DWORD desired_access = 0;
  if (permissions & fs::READ)
    desired_access |= GENERIC_READ;
  if (permissions & fs::WRITE)
    desired_access |= GENERIC_WRITE;
  DWORD share_mode = 0;
  if (!(permissions & fs::EXCLUSIVE))
   share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
  return (uintptr_t)::CreateFileA(path, desired_access, share_mode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::close(uintptr_t hndl) {
  yeti_assert_debug(hndl != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::CloseHandle((HANDLE)hndl);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

u64 fs::read(uintptr_t hndl, uintptr_t in, u64 in_len) {
  yeti_assert_debug(hndl != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  DWORD read;
  ::ReadFile((HANDLE)hndl, (LPVOID)in, in_len, &read, NULL);
  return read;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

u64 fs::write(uintptr_t hndl, const uintptr_t out, u64 out_len) {
  yeti_assert_debug(hndl != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  DWORD wrote;
  ::WriteFile((HANDLE)hndl, (LPCVOID)out, out_len, &wrote, NULL);
  return wrote;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

i64 fs::seek(uintptr_t hndl, fs::Position pos, i64 offset) {
  yeti_assert_debug(hndl != NULL);
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
  ::SetFilePointerEx((HANDLE)hndl, move, &moved, method);
  return moved.QuadPart;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void fs::flush(uintptr_t hndl) {
  yeti_assert_debug(hndl != NULL);
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::FlushFileBuffers((HANDLE)hndl);
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
