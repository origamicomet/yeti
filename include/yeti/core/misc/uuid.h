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
  /// \brief Determines if @string contains a UUID.
  extern YETI_PUBLIC bool validate(const char *string);

  /// \brief Parses @string into a @uuid.
  extern YETI_PUBLIC bool parse(const char *string, u8 uuid[16]);
}

} // core
} // yeti

#endif // _YETI_CORE_MISC_UUID_H_


