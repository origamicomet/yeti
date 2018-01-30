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
///
/// \file
/// \brief Normalizes support for strings.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_STRINGS_H_
#define _YETI_FOUNDATION_SUPPORT_STRINGS_H_

#include "yeti/linkage.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

namespace yeti {
namespace core {

namespace kludge {
  /// \internal
  /// @{
  extern YETI_PUBLIC int vsnprintf(char *buffer, size_t size, const char *format, va_list ap);
  extern YETI_PUBLIC int snprintf(char *buffer, size_t size, const char *format, ...);
  /// @}
}

namespace string {
  /// \brief Compares @a to @b.
  /// \return True if @a and @b match.
  extern YETI_PUBLIC bool compare(const char *a, const char *b);
}

} // core
} // yeti

// All because Microsoft has to do things their own stupid way...
#define vsnprintf ::yeti::core::kludge::vsnprintf
#define snprintf ::yeti::core::kludge::snprintf

#endif // _YETI_FOUNDATION_SUPPORT_STRINGS_H_
