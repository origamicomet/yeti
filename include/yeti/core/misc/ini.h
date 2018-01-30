//===-- yeti/core/misc/ini.h ----------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_MISC_INI_H_
#define _YETI_CORE_MISC_INI_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

// See `yeti/core/platform/filesystem.h`.
struct File;

namespace ini {
  typedef bool (*Callback)(const char *section,
                           const char *key,
                           const char *value,
                           void *context);

  enum Options {
    // Skip malformed entries and rather thant stopping parsing.
    CONTINUE_ON_ERROR = (1 << 0)
  };

  /// \brief Parses a @buffer, calling @callback for each entry.
  extern YETI_PUBLIC bool parse(const char *buffer,
                                unsigned length,
                                Callback callback,
                                void *context = NULL,
                                u32 options = 0x00000000);

  /// \brief Parses a @file, calling @callback for each entry.
  extern YETI_PUBLIC bool parse(File *file,
                                Callback callback,
                                void *context = NULL,
                                u32 options = 0x00000000);
}

} // core
} // yeti

#endif // _YETI_CORE_MISC_INI_H_
