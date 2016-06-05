//===-- yeti/foundation/uuid.h ----------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_UUID_H_
#define _YETI_FOUNDATION_UUID_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

class YETI_PUBLIC UUID {
 public:
  UUID();
  UUID(const UUID &uuid);
  UUID &operator=(const UUID &uuid);

 public:
  static void generate(UUID *uuid);

 public:
  u8 raw[16];
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_UUID_H_
