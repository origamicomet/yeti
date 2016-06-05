//===-- yeti/foundation/path.h ----------------------------*- mode: C++ -*-===//
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

#include "yeti/foundation/path.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

// TODO(mtwilliams): Handle UTF-8.

namespace yeti {
namespace foundation {

char path::seperator() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return '\\';
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X \
      YETI_PLATFORM == YETI_PLATFORM_LINUX \
      YETI_PLATFORM == YETI_PLATFORM_IOS \
      YETI_PLATFORM == YETI_PLATFORM_ANDROID
  return '/';
#endif
}

void path::canonicalize(char *path) {
  yeti_assert_debug(path != NULL);
  for (char *ch = path; *ch; ++ch) {
    switch (*ch) {
    #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
      case '/': *ch = '\\'; break;
    #elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X \
          YETI_PLATFORM == YETI_PLATFORM_LINUX \
          YETI_PLATFORM == YETI_PLATFORM_IOS \
          YETI_PLATFORM == YETI_PLATFORM_ANDROID
      case '\\': *ch = '/'; break;
    #endif
    }
  }
}

const char *path::file(const char *path) {
  yeti_assert_debug(path != NULL);

  const char *start = path;
  const char *end = path + strlen(path);

  for (const char *ch = end; ch >= start; --ch) {
    switch (*ch) {
      case '/':
      case '\\': return &ch[1];
    }
  }

  return path;
}

const char *path::extension(const char *path) {
  yeti_assert_debug(path != NULL);

  const char *start = path;
  const char *end = path + strlen(path);

  for (const char *ch = end; ch >= start; --ch) {
    switch (*ch) {
      case '.': return (ch != start) ? &ch[1] : NULL;
      case '/':
      case '\\': return NULL;
    }
  }

  return NULL;
}

void path::cwd(char *cwd, size_t cwd_len) {
  // TODO(mtwilliams): Allow users to determine the size of buffer required?
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD wrote = ::GetCurrentDirectoryA((DWORD)cwd_len, (LPTSTR)&cwd[0]);
  yeti_assert_debug((wrote != 0) && (wrote <= cwd_len - 1));
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

} // foundation
} // yeti
