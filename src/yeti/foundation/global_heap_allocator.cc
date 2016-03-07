//===-- yeti/foundation/global_heap_allocator.cc ----------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/global_heap_allocator.h"

#include <stdlib.h>

namespace yeti {
namespace foundation {

namespace {
  class GlobalHeapAllocator : public Allocator {
   YETI_DISALLOW_COPYING(GlobalHeapAllocator);

   public:
    GlobalHeapAllocator();
    ~GlobalHeapAllocator();

   public:
    uintptr_t allocate(size_t sz, size_t alignment);
    uintptr_t reallocate(uintptr_t ptr, size_t sz, size_t alignment);
    void deallocate(uintptr_t ptr);
  };

  GlobalHeapAllocator::GlobalHeapAllocator()
    : Allocator()
  {
    this->set_name("heap");
  }

  GlobalHeapAllocator::~GlobalHeapAllocator() {
  }

  uintptr_t GlobalHeapAllocator::allocate(size_t sz, size_t alignment) {
    // FIXME(mtwilliams): Respond to |alignment|.
    return (uintptr_t)::malloc(sz);
  }

  uintptr_t GlobalHeapAllocator::reallocate(uintptr_t ptr, size_t sz, size_t alignment) {
    // FIXME(mtwilliams): Respond to |alignment|.
    return (uintptr_t)::realloc((void *)ptr, sz);
  }

  void GlobalHeapAllocator::deallocate(uintptr_t ptr) {
    ::free((void *)ptr);
  }
}

static GlobalHeapAllocator global_heap_allocator_;

Allocator &heap() {
  return global_heap_allocator_;
}

} // foundation
} // yeti
