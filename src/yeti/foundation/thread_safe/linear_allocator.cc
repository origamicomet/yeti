//===-- yeti/foundation/thread_safe/linear_allocator.cc ---*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/thread_safe/linear_allocator.h"

#include "yeti/foundation/atomics.h"
#include "yeti/foundation/memory.h"

namespace yeti {
namespace foundation {
namespace thread_safe {

LinearAllocator::LinearAllocator(uintptr_t mem, size_t sz)
  : Allocator()
  , mem_lower_(mem)
  , mem_upper_(mem + sz)
  , unallocated_(mem)
  , unreclaimed_lower_(mem)
  , unreclaimed_upper_(mem)
  , reclaiming_(0)
{
  this->set_name("thread_safe_linear_allocator");
}

LinearAllocator::~LinearAllocator() {
}

// TODO(mtwilliams): Add barriers.
uintptr_t LinearAllocator::allocate(size_t sz, size_t alignment) {
  while (true) {
    const uintptr_t unallocated = atomic::load(&unallocated_);

    const uintptr_t mem_after_header = unallocated + sizeof(Allocation);
    const size_t padding_after_header = memory::align(mem_after_header, alignment);

    const uintptr_t allocation = mem_after_header + padding_after_header;

    const uintptr_t mem_after_allocation = allocation + sz;
    const size_t padding_after_allocation = memory::align(mem_after_allocation, 4);

    const uintptr_t mem_after_everything = mem_after_allocation + padding_after_allocation;

    if (mem_after_everything >= mem_upper_)
      // We don't have enough memory left to fufill the requested allocation.
      // In most cases this is non-fatal, at least in our use cases. Typically
      // we trade memory for contention when using this allocator. Hence us
      // returning `NULL` rather than crashing (to let us retry).
      return NULL;

    if (atomic::cmp_and_xchg(&unallocated_, unallocated, mem_after_everything) != unallocated)
      continue;

    Allocation *header = (Allocation *)unallocated;
    header->end = mem_after_everything;
    memset((void *)&header->padding[0], Allocation::PAD, padding_after_header);

    return allocation;
  }
}

// TODO(mtwilliams): Add barriers.
uintptr_t LinearAllocator::reallocate(uintptr_t ptr, size_t sz, size_t alignment) {
  Allocation *header = Allocation::recover(ptr);

  // OPTIMIZATION(mtwilliams): Grow if possible.
  const uintptr_t new_ptr = this->allocate(sz, alignment);
  yeti_assert_debug(new_ptr != NULL);
  memcpy((void *)new_ptr, (const void *)ptr, header->end - ptr);
  this->deallocate(ptr);

  return new_ptr;
}

// TODO(mtwilliams): Add barriers.
void LinearAllocator::deallocate(uintptr_t ptr) {
  yeti_assert_debug(ptr > mem_lower_);
  yeti_assert_debug(ptr < mem_upper_);

  Allocation *header = Allocation::recover(ptr);

  // Catch double deallocations.
  const bool deallocated = !!(atomic::load(&header->end) & 1);
  yeti_assert_debug(!deallocated);

  const uintptr_t lower = (uintptr_t)header;
  const uintptr_t upper = atomic::load(&header->end);

  // Mark as deallocated.
  atomic::store(&header->end, upper | 1);

  // Add to unreclaimed region.
  atomic::min(&unreclaimed_lower_, lower);
  atomic::max(&unreclaimed_upper_, upper);

  // Reclaim as much of the partially unallocated region as possible.
  this->reclaim();
}

void LinearAllocator::reclaim() {
  if (atomic::cmp_and_xchg(&reclaiming_, 0, 1) != 0)
    // Another thread is already reclaiming.
    return;

  while (true) {
    uintptr_t unallocated = atomic::load(&unallocated_);

    uintptr_t unreclaimed_lower = atomic::load(&unreclaimed_lower_);
    uintptr_t unreclaimed_upper = atomic::load(&unreclaimed_upper_);

    if (unreclaimed_upper != unallocated)
      // Not a contiguous block, unfortunately.
      break;

    if (unreclaimed_lower == unreclaimed_upper)
      // Nothing to reclaim.
      break;

    uintptr_t reclaimable = unreclaimed_lower;
    for (uintptr_t cursor = unreclaimed_lower; cursor < unreclaimed_upper;) {
      const Allocation *header = (const Allocation *)cursor;
      cursor = header->end & ~((uintptr_t)1);
      reclaimable = header->deallocated ? reclaimable : cursor;
    }

    if (atomic::cmp_and_xchg(&unallocated_, unallocated, reclaimable) == unallocated) {
      atomic::store(&unreclaimed_upper_, reclaimable);
      break;
    }
  }

  atomic::store(&reclaiming_, 0);
}

} // thread_safe
} // foundation
} // yeti
