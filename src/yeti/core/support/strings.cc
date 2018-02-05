//===-- yeti/core/support/strings.h ---------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/support/strings.h"

#undef vsnprintf
#undef snprintf

namespace yeti {
namespace core {

int kludge::vsnprintf(char *buffer, size_t size, const char *format, va_list ap) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  int count = -1;
  if (size != 0)
    count = _vsnprintf_s(buffer, size, _TRUNCATE, format, ap);
  if (count == -1)
    count = _vscprintf(format, ap);
  return count;
#else
  return ::vsnprintf(buffer, size, format, ap);
#endif
}

int kludge::snprintf(char *buffer, size_t size, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  const int count = vsnprintf(buffer, size, format, ap);
  va_end(ap);
  return count;
}

bool string::compare(const char *a, const char *b) {
  return (strcmp(a, b) == 0);
}

bool string::compare(const char *a, const char *b, size_t length) {
  return (strncmp(a, b, length) == 0);
}

} // core
} // yeti
