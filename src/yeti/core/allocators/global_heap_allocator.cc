//===-- yeti/core/allocators/global_heap_allocator.cc ---*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/allocators/global_heap_allocator.h"

// For sanity checks.
#include "yeti/core/debug/assert.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS || \
    YETI_PLATFORM == YETI_PLATFORM_MAC || \
    YETI_PLATFORM == YETI_PLATFORM_LINUX
  #include <windows.h>
  #include <malloc.h>
#endif

namespace yeti {
namespace core {

class GlobalHeapAllocator : public Allocator {
 YETI_DISALLOW_COPYING(GlobalHeapAllocator)

 public:
  GlobalHeapAllocator();
  ~GlobalHeapAllocator();

 public:
  void *allocate(size_t size, size_t alignment = 16);
  void *reallocate(void *ptr, size_t size, size_t alignment = 16);
  void deallocate(void *ptr);
};

GlobalHeapAllocator::GlobalHeapAllocator()
  : Allocator()
{
}

GlobalHeapAllocator::~GlobalHeapAllocator() {
}

void *GlobalHeapAllocator::allocate(size_t size, size_t alignment) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return _aligned_malloc(size, alignment);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void *GlobalHeapAllocator::reallocate(void *ptr, size_t size, size_t alignment) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  return _aligned_realloc(ptr, size, alignment);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void GlobalHeapAllocator::deallocate(void *ptr) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  _aligned_free(ptr);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

static GlobalHeapAllocator global_heap_allocator_;

Allocator &global_heap_allocator() {
  return global_heap_allocator_;
}

} // core
} // yeti
