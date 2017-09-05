//===-- yeti/foundation/atomics.h -----------------------*- mode: C++11 -*-===//
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

#include <stdlib.h>
#include <string.h>

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"

namespace yeti {
namespace foundation {

// TODO(mtwilliam): Document `memory` utilities.
// TODO(mtwilliams): Restrict `memory::copy` buffers.
// TODO(mtwilliams): Assert `memory::copy` buffers don't overlap.

/// \namespace ::yeti::foundation::memory
/// \brief
namespace memory {
  /// Forward aligns @base to the closest @alignment byte boundry.
  uintptr_t align(uintptr_t base, size_t alignment);

  /// Zeros @count bytes starting from @ptr.
  void zero(uintptr_t ptr, size_t count);

  /// Copies @count bytes from @src to @dst.
  void copy(uintptr_t src, uintptr_t dst, size_t count);
}

YETI_INLINE uintptr_t memory::align(uintptr_t base, size_t alignment) {
  return (alignment - (base % alignment)) % alignment;
}

YETI_INLINE void memory::zero(uintptr_t ptr, size_t count) {
  ::memset((void *)ptr, 0, count);
}

YETI_INLINE void memory::copy(uintptr_t src, uintptr_t dst, size_t count) {
  ::memcpy((void *)dst, (const void *)src, count);
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_MEMORY_H_
