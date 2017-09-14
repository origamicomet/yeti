//===-- yeti/foundation/global_heap_allocator.cc --------*- mode: C++11 -*-===//
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

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <malloc.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
  #include <malloc.h>
#endif

namespace yeti {
namespace foundation {

namespace {
  class GlobalHeapAllocator : public Allocator {
   YETI_DISALLOW_COPYING(GlobalHeapAllocator)

   public:
    GlobalHeapAllocator();
    ~GlobalHeapAllocator();

   public:
    uintptr_t allocate(size_t size, size_t alignment);
    uintptr_t reallocate(uintptr_t ptr, size_t size, size_t alignment);
    void deallocate(uintptr_t ptr);
  };

  GlobalHeapAllocator::GlobalHeapAllocator()
    : Allocator()
  {
    this->set_name("heap");
  }

  GlobalHeapAllocator::~GlobalHeapAllocator() {
  }

  uintptr_t GlobalHeapAllocator::allocate(size_t size, size_t alignment) {
  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    return (uintptr_t)_aligned_malloc(size, alignment);
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
        YETI_PLATFORM == YETI_PLATFORM_LINUX
  #endif
  }

  uintptr_t GlobalHeapAllocator::reallocate(uintptr_t ptr, size_t size, size_t alignment) {
  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    if (ptr)
      return (uintptr_t)_aligned_realloc((void *)ptr, size, alignment);
    else
      return this->allocate(size, alignment);
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
        YETI_PLATFORM == YETI_PLATFORM_LINUX
  #endif
  }

  void GlobalHeapAllocator::deallocate(uintptr_t ptr) {
  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    _aligned_free((void *)ptr);
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
        YETI_PLATFORM == YETI_PLATFORM_LINUX
  #endif
  }
}

static GlobalHeapAllocator global_heap_allocator_;

Allocator &heap() {
  return global_heap_allocator_;
}

} // foundation
} // yeti
