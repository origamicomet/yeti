//===-- yeti/core/allocators/global_page_allocator.h ----*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_ALLOCATORS_GLOBAL_PAGE_ALLOCATOR_H_
#define _YETI_CORE_ALLOCATORS_GLOBAL_PAGE_ALLOCATOR_H_

#include "yeti/core/allocator.h"

namespace yeti {
namespace core {

/// Allocates whole pages from system.
class YETI_PUBLIC PageAllocator : public Allocator {
 YETI_DISALLOW_COPYING(PageAllocator)

 protected:
  PageAllocator(size_t granularity) : granularity_(granularity) {}
  ~PageAllocator() {}

 public:
  virtual void *allocate(size_t size, size_t alignment = 16) = 0;
  virtual void *reallocate(void *ptr, size_t size, size_t alignment = 16) = 0;
  virtual void deallocate(void *ptr) = 0;

 public:
  size_t granularity() const { return granularity_; }

 protected:
  const size_t granularity_;
};

/// brief Returns the global page allocator.
extern YETI_PUBLIC PageAllocator &global_page_allocator();

} // core
} // yeti

#endif // _YETI_CORE_ALLOCATORS_GLOBAL_PAGE_ALLOCATOR_H_
