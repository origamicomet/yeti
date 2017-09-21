//===-- yeti/core/allocators/thread_safe/scratch_allocator.cc -------------===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/allocators/thread_safe/scratch_allocator.h"

#include "yeti/core/atomics.h"

// For sanity checks.
#include "yeti/core/debug/assert.h"

// TODO(mtwilliams): Audit synchronization points.

namespace yeti {
namespace core {
namespace thread_safe {

ScratchAllocator::ScratchAllocator(Allocator *allocator, size_t size)
  : Allocator()
  , backing_(allocator)
  , lower_((uintptr_t)allocator->allocate(size))
  , upper_(lower_ + size)
  , unallocated_(lower_)
  , unreclaimed_lower_(lower_)
  , unreclaimed_upper_(lower_)
  , reclaiming_(0)
{
}

ScratchAllocator::ScratchAllocator(Allocator &allocator, size_t size)
  : Allocator()
  , backing_(&allocator)
  , lower_((uintptr_t)allocator.allocate(size))
  , upper_(lower_ + size)
  , unallocated_(lower_)
  , unreclaimed_lower_(lower_)
  , unreclaimed_upper_(lower_)
  , reclaiming_(0)
{
}

ScratchAllocator::ScratchAllocator(void *memory, size_t size)
  : Allocator()
  , backing_(NULL)
  , lower_((uintptr_t)memory)
  , upper_(lower_ + size)
  , unallocated_(lower_)
  , unreclaimed_lower_(lower_)
  , unreclaimed_upper_(lower_)
  , reclaiming_(0)
{
}

ScratchAllocator::~ScratchAllocator() {
  if (backing_)
    backing_->deallocate((void *)lower_);
}

void *ScratchAllocator::allocate(size_t size, size_t alignment) {
  while (true) {
    const uintptr_t unallocated = atomic::load(&unallocated_);

    const uintptr_t mem_after_header = unallocated + sizeof(Allocation);
    const size_t padding_after_header = memory::align(mem_after_header, alignment);

    const uintptr_t allocation = mem_after_header + padding_after_header;

    const uintptr_t mem_after_allocation = allocation + size;
    const size_t padding_after_allocation = memory::align(mem_after_allocation, 4);

    const uintptr_t mem_after_everything = mem_after_allocation + padding_after_allocation;

    if (mem_after_everything >= upper_)
      // We don't have enough memory left to fufill the requested allocation.
      return NULL;

    if (atomic::cmp_and_xchg(&unallocated_, unallocated, mem_after_everything) != unallocated)
      // Lost to another thread.
      continue;

    Allocation *header = (Allocation *)unallocated;
    header->end = mem_after_everything;
    memset((void *)&header->padding[0], Allocation::PAD, padding_after_header);

    return (void *)allocation;
  }
}

void *ScratchAllocator::reallocate(void *ptr, size_t size, size_t alignment) {
  // Not supported.
  YETI_TRAP();

  return NULL;
}

void ScratchAllocator::deallocate(void *ptr) {
  yeti_assert_debug(((uintptr_t)ptr) > lower_);
  yeti_assert_debug(((uintptr_t)ptr) < upper_);

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

void ScratchAllocator::reclaim() {
  if (atomic::cmp_and_xchg(&reclaiming_, 0, 1) != 0)
    // Another thread is already reclaiming.
    return;

  while (true) {
    uintptr_t unallocated = atomic::load(&unallocated_);

    uintptr_t unreclaimed_lower = atomic::load(&unreclaimed_lower_);
    uintptr_t unreclaimed_upper = atomic::load(&unreclaimed_upper_);

    if (unreclaimed_upper != unallocated)
      // Not a contiguous block.
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
} // core
} // yeti
