//===-- yeti/foundation/mirrored_ring_buffer.cc -----------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/mirrored_ring_buffer.h"

#include "yeti/foundation/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif

namespace yeti {
namespace foundation {

struct MirroredRingBuffer::Implementation {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  struct {
    HANDLE mapping;
    uintptr_t lower_half;
    uintptr_t upper_half;
  } vm;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
};

MirroredRingBuffer *MirroredRingBuffer::create(u32 sz) {
  yeti_assertf_debug(sz >= 0xffff, "Must be at least 64KiB.");
  yeti_assertf_debug((sz & 0xffff) == 0, "Must be a multiple of 64KiB.");

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  HANDLE mapping = ::CreateFileMappingA(INVALID_HANDLE_VALUE, NULL,
                                        PAGE_READWRITE | SEC_COMMIT,
                                        0, sz, NULL);
  yeti_assert(mapping != NULL);

  // Retry until we obtain a contiguous virtual map.
  for (u32 attempts = 0; attempts < 3; ++attempts) {
    void *base_virtual_addr = ::VirtualAlloc(NULL, 2 * sz, MEM_RESERVE, PAGE_NOACCESS);
    yeti_assert(base_virtual_addr != NULL);
    ::VirtualFree(base_virtual_addr, 0, MEM_RELEASE);

    void *lower_half = ::MapViewOfFileEx(mapping,
                                         FILE_MAP_ALL_ACCESS, 0, 0, sz,
                                         base_virtual_addr);
    if (lower_half == NULL) {
      // Another thread took the virtual address space.
      continue;
    }

    void *upper_half = ::MapViewOfFileEx(mapping,
                                         FILE_MAP_ALL_ACCESS, 0, 0, sz,
                                         (void *)((uintptr_t)base_virtual_addr + sz));
    if (upper_half == NULL) {
      // Another thread took the virtual address space.
      ::UnmapViewOfFile(lower_half);
      continue;
    }

    // We got a contiguous mapping!
    Implementation *impl = new (foundation::heap()) Implementation();
    impl->vm.mapping = mapping;
    impl->vm.lower_half = (uintptr_t)lower_half;
    impl->vm.upper_half = (uintptr_t)upper_half;
    return (MirroredRingBuffer *)impl;
  }

  yeti_assertf(0, "Unable to obtain a contiguous virtual memory map!");
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

void MirroredRingBuffer::destroy() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  Implementation *impl = (Implementation *)this;

  ::UnmapViewOfFile((void *)impl->vm.upper_half);
  ::UnmapViewOfFile((void *)impl->vm.lower_half);
  ::CloseHandle(impl->vm.mapping);

  delete impl;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
#endif
}

} // foundation
} // yeti
