//===-- yeti/core/allocators/thread_safe/bump_allocator.h -----------------===//
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

#ifndef _YETI_CORE_ALLOCATORS_THREAD_SAFE_BUMP_ALLOCATOR_H_
#define _YETI_CORE_ALLOCATORS_THREAD_SAFE_BUMP_ALLOCATOR_H_

#include "yeti/core/allocator.h"

namespace yeti {
namespace core {
namespace thread_safe {

class YETI_PUBLIC BumpAllocator : public Allocator {
 YETI_DISALLOW_COPYING(BumpAllocator)

 public:
  BumpAllocator(Allocator *allocator, size_t size);
  BumpAllocator(Allocator &allocator, size_t size);
  BumpAllocator(void *memory, size_t size);

  ~BumpAllocator();

 public:
  void *allocate(size_t size, size_t alignment = 16);
  void *reallocate(void *ptr, size_t size, size_t alignment = 16);
  void deallocate(void *ptr);

 public:
  void reset();

 private:
  Allocator *backing_;
  const uintptr_t lower_;
  const uintptr_t upper_;
  volatile uintptr_t unallocated_;
};

} // thread_safe
} // core
} // yeti

#endif // _YETI_CORE_ALLOCATORS_THREAD_SAFE_BUMP_ALLOCATOR_H_
