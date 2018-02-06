//===-- yeti/core/misc/uuid.h ---------------------------*- mode: C++11 -*-===//
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
/// \brief Universally unique identifiers.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_MISC_UUID_H_
#define _YETI_CORE_MISC_UUID_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"

namespace yeti {
namespace core {

namespace uuid {
  /// Generates a UUID v4.
  extern YETI_PUBLIC void generate(u8 uuid[16]);

  /// Parses @string into a @uuid.
  extern YETI_PUBLIC bool parse(const char *string, u8 uuid[16]);

  /// Converts @uuid to a human-readable representation in @pretty.
  extern YETI_PUBLIC void present(const u8 uuid[20], char pretty[36+1]);

  /// Determines if @string contains a UUID.
  extern YETI_PUBLIC bool validate(const char *string);
}

} // core
} // yeti

#endif // _YETI_CORE_MISC_UUID_H_


