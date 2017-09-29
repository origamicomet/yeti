//===-- yeti/core/algorithms/digest.h -------------------*- mode: C++11 -*-===//
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
/// \brief Digest functions.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_ALGORITHMS_DIGEST_H_
#define _YETI_CORE_ALGORITHMS_DIGEST_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"

// TODO(mtwilliams): SHA-256.

namespace yeti {
namespace core {

// See `yeti/core/platform/filesystem.h`.
struct File;

namespace sha1 {
  /// Computes the SHA-1 digest of @buf.
  extern YETI_PUBLIC void compute(const void *buf, u32 buf_len, u8 digest[20]);

  /// Computes the SHA-1 digest of @file.
  extern YETI_PUBLIC void compute(File *file, u8 digest[20]);

  /// Converts @digest to a 40-character hexadecimal number in @pretty.
  extern YETI_PUBLIC void present(const u8 digest[20], char pretty[40]);

  /// Computes digest of @file and presents it as a 40-character hexadecimal
  /// number.
  extern YETI_PUBLIC void fingerprint(File *file, char fingerprint[40]);
}
} // core
} // yeti

#endif // _YETI_CORE_ALGORITHMS_DIGEST_H_
