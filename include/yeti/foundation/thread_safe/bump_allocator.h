//===-- yeti/foundation/thread_safe/bump_allocator.h ------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_THREAD_SAFE_BUMP_ALLOCATOR_H_
#define _YETI_FOUNDATION_THREAD_SAFE_BUMP_ALLOCATOR_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

#include "yeti/foundation/allocator.h"

namespace yeti {
namespace foundation {
namespace thread_safe {

class YETI_PUBLIC BumpAllocator : public Allocator {
 YETI_DISALLOW_COPYING(BumpAllocator)

 public:
  BumpAllocator(uintptr_t memory, size_t size);
  ~BumpAllocator();

 public:
  uintptr_t allocate(size_t size, size_t alignment = 16);
  uintptr_t reallocate(uintptr_t ptr, size_t size, size_t alignment = 16);
  void deallocate(uintptr_t ptr);

 private:
  const uintptr_t lower_;
  const uintptr_t upper_;
  volatile uintptr_t unallocated_;
};

} // thread_safe
} // foundation
} // yeti

#endif // _YETI_FOUNDATION_THREAD_SAFE_BUMP_ALLOCATOR_H_
