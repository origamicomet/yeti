//===-- yeti/core/platform/path.h -----------------------*- mode: C++11 -*-===//
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
/// \file
/// \brief Path manipulation utilities.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_PATH_H_
#define _YETI_CORE_PLATFORM_PATH_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

namespace path {

/// Preferred path seperator for the current platform.
extern YETI_PUBLIC char seperator();

/// Modifies @path to use the current platforms preferred seperators, among
/// other things.
extern YETI_PUBLIC void canonicalize(char *path);

/// Modifies @path to use forward slashes as path seperators.
extern YETI_PUBLIC void unixify(char *path);

/// Returns the file component of @path.
extern YETI_PUBLIC const char *file(const char *path);

/// Returns the extension of @path.
extern YETI_PUBLIC const char *extension(const char *path);

/// Copies the current working directory to @cwd.
extern YETI_PUBLIC void cwd(char *cwd, size_t size);

/// Performs a wildcard match of @path against @pattern.
extern YETI_PUBLIC bool match(const char *pattern, const char *path);

/// \copydoc yeti::core::path::match
extern YETI_PUBLIC bool glob(const char *pattern, const char *path);

} // path

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_PATH_H_
