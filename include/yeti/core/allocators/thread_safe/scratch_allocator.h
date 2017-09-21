//===-- yeti/core/allocators/thread_safe/scratch_allocator.h --------------===//
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

#ifndef _YETI_CORE_ALLOCATORS_THREAD_SAFE_SCRATCH_ALLOCATOR_H_
#define _YETI_CORE_ALLOCATORS_THREAD_SAFE_SCRATCH_ALLOCATOR_H_

#include "yeti/core/allocator.h"

namespace yeti {
namespace core {
namespace thread_safe {

class YETI_PUBLIC ScratchAllocator : public Allocator {
 YETI_DISALLOW_COPYING(ScratchAllocator)

 private:
  struct Allocation {
    // This deserves some explanation. We know that `end` will be aligned on a
    // 4-byte boundary. In fact, we pad `end` such that the lowest two bits of
    // its highest byte are never set. This ensures that we will never confuse
    // the highest byte of `end` for a padding byte.
    static const u8 PAD = 0x3;

    union {
      // The address to the end of this allocation.
      uintptr_t end;

      // We use the least significant bit to indicate deallocation.
      struct {
      #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
        u32 deallocated : 1;
        u32             : 31;
      #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
        u64 deallocated : 1;
        u64             : 63;
      #endif
      };
    };

    // We may have one or more padding bytes. In practice this is very rare, as
    // the user requested alignment is normally 4 or 8 bytes, which marries up
    // with the end of this header perfectly.
    u8 padding[0];

    static YETI_INLINE Allocation *recover(void *ptr) {
      // Refer to the comments above to understand why this works.
      u8 *end_of_header = (u8 *)ptr;
      while (*--end_of_header == Allocation::PAD);
      return (Allocation *)(end_of_header - sizeof(uintptr_t) + 1);
    }
  };

 public:
  ScratchAllocator(Allocator *allocator, size_t size);
  ScratchAllocator(Allocator &allocator, size_t size);
  ScratchAllocator(void *memory, size_t size);
  ~ScratchAllocator();

 public:
  void *allocate(size_t size, size_t alignment = 16);
  void *reallocate(void *ptr, size_t size, size_t alignment = 16);
  void deallocate(void *ptr);

 private:
  void reclaim();

 private:
  Allocator *backing_;

  const uintptr_t lower_;
  const uintptr_t upper_;

  volatile uintptr_t unallocated_;

  volatile uintptr_t unreclaimed_lower_;
  volatile uintptr_t unreclaimed_upper_;

  volatile u32 reclaiming_;
};

} // thread_safe
} // core
} // yeti

#endif // _YETI_CORE_ALLOCATORS_THREAD_SAFE_SCRATCH_ALLOCATOR_H_
