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

const char *path::extension(const char *path) {
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

} // foundation
} // yeti
