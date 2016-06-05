//===-- yeti/foundation/thread_safe/scratch_allocator.h ---*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_THREAD_SAFE_SCRATCH_ALLOCATOR_H_
#define _YETI_FOUNDATION_THREAD_SAFE_SCRATCH_ALLOCATOR_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

#include "yeti/foundation/allocator.h"

namespace yeti {
namespace foundation {
namespace thread_safe {

// https://bitbucket.org/bitsquid/foundation/src/7f896236dbafd2cb842655004b1c7bf6e76dcef9/memory.cpp?fileviewer=file-view-default

class YETI_PUBLIC ScratchAllocator : public Allocator {
 YETI_DISALLOW_COPYING(ScratchAllocator);

 private:
  struct Allocation {
    // This deserves some explanation. We know that |end| will be aligned on a
    // 4-byte boundary. In fact, we pad |end| such that the lowest two bits of
    // its highest byte are never set. This ensures that we will never confuse
    // a the highest byte of |end| for a padding byte.
    static const u8 PAD = 0x3;

    union {
      // The address to the end of this allocation.
      uintptr_t end;

      // We use the least significant bit to indicate deallocation.
      struct {
      #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
        u32 deallocated : 1;
        u32 : 31;
      #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
        u64 deallocated : 1;
        u64 : 63;
      #endif
      };
    };

    // We may have one or more padding bytes. In practice this is very rare, as
    // the user requested alignment is normally 4 or 8 bytes, which marries up
    // with the end of this header perfectly.
    u8 padding[0];

    static Allocation *recover(uintptr_t ptr) {
      // Refer to the comments above to understand why this works.
      u8 *end_of_header = (u8 *)ptr;
      while (*--end_of_header == Allocation::PAD);
      return (Allocation *)(end_of_header - sizeof(Allocation::end) + 1);
    }
  };

 public:
  ScratchAllocator(uintptr_t mem, size_t sz);
  ~ScratchAllocator();

 public:
  uintptr_t allocate(size_t sz, size_t alignment = 8);
  uintptr_t reallocate(uintptr_t ptr, size_t sz, size_t alignment = 8);
  void deallocate(uintptr_t ptr);

 private:
  void reclaim();

 private:
  const uintptr_t mem_lower_;
  const uintptr_t mem_upper_;
  uintptr_t unallocated_;
  uintptr_t unreclaimed_lower_;
  uintptr_t unreclaimed_upper_;
  u32 reclaiming_;
};

} // thread_safe
} // foundation
} // yeti

#endif // _YETI_FOUNDATION_THREAD_SAFE_SCRATCH_ALLOCATOR_H_
