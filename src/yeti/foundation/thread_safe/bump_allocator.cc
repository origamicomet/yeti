//===-- yeti/foundation/thread_safe/bump_allocator.cc -----*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/thread_safe/bump_allocator.h"

#include "yeti/foundation/atomics.h"
#include "yeti/foundation/memory.h"

namespace yeti {
namespace foundation {
namespace thread_safe {

BumpAllocator::BumpAllocator(uintptr_t memory, size_t size)
  : Allocator()
  , lower_(memory)
  , upper_(memory + size)
  , unallocated_(memory)
{
  this->set_name("ts_bump_allocator");
}

BumpAllocator::~BumpAllocator() {
}

uintptr_t BumpAllocator::allocate(size_t size, size_t alignment) {
  while (true) {
    const uintptr_t unallocated = atomic::load(&unallocated_);

    const size_t padding = unallocated % alignment;
    const size_t length = size + padding;

    if (length >= upper_)
      // We don't have enough memory left to fufill the requested allocation.
      return NULL;

    if (atomic::cmp_and_xchg(&unallocated_, unallocated, unallocated + length) != unallocated)
      continue;

    return unallocated + padding;
  }
}

uintptr_t BumpAllocator::reallocate(uintptr_t ptr, size_t size, size_t alignment) {
  // Not supported.
  return NULL;
}

void BumpAllocator::deallocate(uintptr_t ptr) {
  yeti_assert_debug(ptr > lower_);
  yeti_assert_debug(ptr < upper_);
}

} // thread_safe
} // foundation
} // yeti
