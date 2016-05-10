//===-- yeti/foundation/atomics.h -------------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_MEMORY_H_
#define _YETI_FOUNDATION_MEMORY_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"

namespace yeti {
namespace foundation {

/// \namespace ::yeti::foundation::memory
/// \brief TODO(mtwilliam): Document this.
namespace memory {
  uintptr_t align(uintptr_t base, size_t alignment);
}

YETI_INLINE uintptr_t memory::align(uintptr_t base, size_t alignment) {
  return (alignment - (base % alignment)) % alignment;
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_MEMORY_H_
