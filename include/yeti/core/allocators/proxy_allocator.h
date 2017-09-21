//===-- yeti/core/allocators/proxy_allocator.h ----------*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_PROXY_ALLOCATOR_H_
#define _YETI_CORE_PROXY_ALLOCATOR_H_

#include "yeti/core/allocator.h"

namespace yeti {
namespace core {

/// Proxies allocations to another allocator.
class YETI_PUBLIC ProxyAllocator : public Allocator {
 YETI_DISALLOW_COPYING(ProxyAllocator)

 public:
  explicit ProxyAllocator(Allocator *allocator, const char *name);
  explicit ProxyAllocator(Allocator &allocator, const char *name);

  ~ProxyAllocator();

 public:
  void *allocate(size_t size, size_t alignment = 16);
  void *reallocate(void *ptr, size_t size, size_t alignment = 16);
  void deallocate(void *ptr);

 private:
  Allocator *forwardee_;
};

} // core
} // yeti

#endif // _YETI_CORE_PROXY_ALLOCATOR_H_
