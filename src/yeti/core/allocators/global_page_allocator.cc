//===-- yeti/core/allocators/global_page_allocator.cc ---*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/allocators/global_page_allocator.h"

// For sanity checks.
#include "yeti/core/utilities.h"
#include "yeti/core/debug/assert.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
      YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace core {

// NOTE(mtwilliams): Unfortunately, Microsoft aligns virtual memory allocations
// to 64KiB boundaries for maddening historical reasons. If we allocate
// assuming page granularity we can end up wasting 60KiB each allocation.
// Rather than deal with consequences of that (which are especially noticeable
// in 32 bit) we actually allocate from a seperate unbounded heap. This
// introduces some (minor) overhead, but also provides a lot of functionality
// out of the box.

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  static HANDLE global_page_heap_ = INVALID_HANDLE_VALUE;
#endif

namespace {
  static size_t size_of_pages() {
  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    SYSTEM_INFO system_info = { 0 };
    ::GetSystemInfo(&system_info);
    return system_info.dwPageSize;
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC || \
        YETI_PLATFORM == YETI_PLATFORM_LINUX
  #endif
  }
}

class GlobalPageAllocator : public PageAllocator {
 YETI_DISALLOW_COPYING(GlobalPageAllocator)

 public:
  GlobalPageAllocator();
  ~GlobalPageAllocator();

 public:
  void *allocate(size_t size, size_t alignment = 16);
  void *reallocate(void *ptr, size_t size, size_t alignment = 16);
  void deallocate(void *ptr);
};

GlobalPageAllocator::GlobalPageAllocator()
  : PageAllocator(size_of_pages())
{
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  global_page_heap_ = ::HeapCreate(0, 65535, 0);
#endif
}

GlobalPageAllocator::~GlobalPageAllocator() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::HeapDestroy(global_page_heap_);
#endif
}

void *GlobalPageAllocator::allocate(size_t size, size_t alignment) {
  yeti_assert_debug(alignment <= granularity());
  yeti_assert_debug(YETI_IS_POWER_OF_TWO(alignment));

  // Round up to nearest page.
  size += size % granularity();

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  void *ptr = ::HeapAlloc(global_page_heap_,
                          HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
                          size);
#endif

  return ptr;
}

void *GlobalPageAllocator::reallocate(void *ptr, size_t size, size_t alignment) {
  // Reallocation not supported.
  YETI_TRAP();

  return NULL;
}

void GlobalPageAllocator::deallocate(void *ptr) {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::HeapFree(global_page_heap_, 0, ptr);
#endif
}


PageAllocator &global_page_allocator() {
  // HACK(mtwilliams): Force initialization on first call in case static
  // constructors need to allocate from the global page allocator.

  // BUG(mtwilliams): May initialize more than once.
  static GlobalPageAllocator global_page_allocator_;

  return global_page_allocator_;
}

} // core
} // yeti
