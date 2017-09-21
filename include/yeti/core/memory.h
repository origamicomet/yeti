//===-- yeti/core/memory.h ------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_MEMORY_H_
#define _YETI_CORE_MEMORY_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

#include <string.h>

namespace yeti {
namespace core {

// TODO(mtwilliam): Document `memory` utilities.
// TODO(mtwilliams): Assert `memory::copy` buffers don't overlap.
// PERF(mtwilliams): Restrict `memory::copy` pointers.

/// \namespace ::yeti::core::memory
/// \brief
namespace memory {
  /// Forward aligns @base pointer to the closest @alignment byte boundry.
  static YETI_INLINE uintptr_t align(uintptr_t base, size_t alignment) {
    return (alignment - (base % alignment)) % alignment;
  }

  /// Forward aligns @base pointer to the closest @alignment byte boundry.
  static YETI_INLINE void *align(void *base, size_t alignment) {
    return (void *)memory::align((uintptr_t)base, alignment);
  }

  /// Zeros @count bytes starting from @ptr.
  static YETI_INLINE void zero(void *ptr, size_t count) {
    ::memset(ptr, 0, count);
  }

  /// Copies @count bytes from @source to @destination.
  static YETI_INLINE void copy(const void *source, void *destination, size_t count) {
    ::memcpy(destination, source, count);
  }
}

} // core
} // yeti

#endif // _YETI_CORE_MEMORY_H_
