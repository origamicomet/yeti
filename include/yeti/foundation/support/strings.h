//===-- yeti/foundation/support/strings.h -----------------*- mode: C++ -*-===//
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
/// \brief Normalizes support for strings.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_STRINGS_H_
#define _YETI_FOUNDATION_SUPPORT_STRINGS_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// Of course Microsoft has to do things their own, stupid way...
#if defined(_MSC_VER)
  #define vsnprintf _yeti_vsnprintf_kludge
  static __forceinline int _yeti_vsnprintf_kludge(
    char *buf, size_t buf_sz, const char *fmt, va_list ap)
  {
    int count = -1;
    if (buf_sz != 0)
      count = _vsnprintf_s(buf, buf_sz, _TRUNCATE, fmt, ap);
    if (count == -1)
      count = _vscprintf(fmt, ap);
    return count;
  }

  #define snprintf _yeti_snprintf_kludge
  static __forceinline int _yeti_snprintf_kludge(
    char *buf, size_t buf_sz, const char *fmt, ...)
  {
    va_list ap;
    va_start(ap, fmt);
    const int count = _yeti_vsnprintf_kludge(buf, buf_sz, fmt, ap);
    va_end(ap);
    return count;
  }
#endif

#endif // _YETI_FOUNDATION_SUPPORT_STRINGS_H_
