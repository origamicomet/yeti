//===-- yeti/core/platform/path.cc ----------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/path.h"

// For sanity checks.
#include "yeti/core/debug/assert.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  #include <unistd.h>
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif

// TODO(mtwilliams): Handle unicode.

namespace yeti {
namespace core {

char path::seperator() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return '\\';
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX || \
      YETI_PLATFORM == YETI_PLATFORM_IOS || \
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
    #elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
          YETI_PLATFORM == YETI_PLATFORM_LINUX || \
          YETI_PLATFORM == YETI_PLATFORM_IOS || \
          YETI_PLATFORM == YETI_PLATFORM_ANDROID
      case '\\': *ch = '/'; break;
    #endif
    }
  }
}

void path::unixify(char *path) {
  yeti_assert_debug(path != NULL);
  for (char *ch = path; *ch; ++ch) {
    switch (*ch) {
      case '\\': *ch = '/'; break;
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

void path::cwd(char *cwd, size_t length) {
  // TODO(mtwilliams): Allow users to determine the size of buffer required?
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  const DWORD wrote = ::GetCurrentDirectoryA((DWORD)length, (LPTSTR)&cwd[0]);
  yeti_assert_debug((wrote != 0) && (wrote <= length - 1));
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  const char *path = ::getcwd(cwd, length);
  yeti_assert_debug(path != NULL);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#elif YETI_PLATFORM == YETI_PLATFORM_IOS
#elif YETI_PLATFORM == YETI_PLATFORM_ANDROID
#endif
}

// TODO(mtwilliams): Double wildcards should greedy match.
// TODO(mtwilliams): One of any characters, e.g. `[abc]`.
// TODO(mtwilliams): One of any range of characters, e.g. `[a-z]`.

bool path::match(const char *pattern, const char *path) {
  while (*path) {
    if (*pattern == '*') {
      // Coalesce wildcards.
      while (*pattern++ == '*');

      // Match any until we match next class in pattern.
      while (*path) {
        if (*pattern == '?')
          goto matched;
        else if ((*pattern == '/'))
          if (pattern[1] == '\0')
            // Trailing path separators are ignored.
            return true;
          else if (*path == '/' || *path == '\\')
            // Matches Windows and Unix style path separators.
            goto matched;
          else
            return false;
        else if (*path == *pattern)
          goto matched;

        path++;
      }

      return (*pattern == '\0');
    } else if (*pattern == '?') {
      // Matches any.
      goto matched;
    } else if ((*pattern == '/')) {
      if (pattern[1] == '\0')
        // Trailing path separators are ignored.
        return true;
      else if (*path == '/' || *path == '\\')
        // Matches Windows and Unix style path separators.
        goto matched;
      else
        return false;
    }

    if (*path != *pattern)
      return false;

  matched:
    path++;
    pattern++;
  }

  return (*pattern == '\0');
}

bool path::glob(const char *pattern, const char *path) {
  return path::match(pattern, path);
}

} // core
} // yeti
