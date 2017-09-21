//===-- yeti/core/allocators/thread_safe/bump_allocator.cc ----------------===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/allocators/thread_safe/bump_allocator.h"

#include "yeti/core/atomics.h"

// For sanity checks.
#include "yeti/core/debug/assert.h"

namespace yeti {
namespace core {
namespace thread_safe {

BumpAllocator::BumpAllocator(Allocator *allocator, size_t size)
  : Allocator()
  , backing_(allocator)
  , lower_((uintptr_t)allocator->allocate(size))
  , upper_(lower_ + size)
  , unallocated_(lower_)
{
}

BumpAllocator::BumpAllocator(Allocator &allocator, size_t size)
  : Allocator()
  , backing_(&allocator)
  , lower_((uintptr_t)allocator.allocate(size))
  , upper_(lower_ + size)
  , unallocated_(lower_)
{
}

BumpAllocator::BumpAllocator(void *memory, size_t size)
  : Allocator()
  , backing_(NULL)
  , lower_((uintptr_t)memory)
  , upper_(lower_ + size)
  , unallocated_(lower_)
{
}

BumpAllocator::~BumpAllocator() {
  if (backing_)
    backing_->deallocate((void *)lower_);
}

void *BumpAllocator::allocate(size_t size, size_t alignment) {
  while (true) {
    const uintptr_t unallocated = atomic::load(&unallocated_);

    const size_t padding = unallocated % alignment;
    const size_t length = size + padding;

    if (length >= upper_)
      // We don't have enough memory left to fufill the requested allocation.
      return NULL;

    if (atomic::cmp_and_xchg(&unallocated_, unallocated, unallocated + length) != unallocated)
      // Lost to another thread.
      continue;

    return (void *)(unallocated + padding);
  }
}

void *BumpAllocator::reallocate(void *ptr, size_t size, size_t alignment) {
  // Not supported.
  YETI_TRAP();

  return NULL;
}

void BumpAllocator::deallocate(void *ptr) {
  yeti_assert_debug(((uintptr_t)ptr) > lower_);
  yeti_assert_debug(((uintptr_t)ptr) < upper_);
}

void BumpAllocator::reset() {
  while (atomic::cmp_and_xchg(&unallocated_, unallocated_, lower_) != unallocated_);
}

} // thread_safe
} // core
} // yeti
