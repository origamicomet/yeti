//===-- yeti/core/allocators/proxy_allocator.cc ---------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/allocators/proxy_allocator.h"

namespace yeti {
namespace core {

ProxyAllocator::ProxyAllocator(Allocator *allocator,
                               const char *name)
  : forwardee_(allocator)
{
}

ProxyAllocator::ProxyAllocator(Allocator &allocator,
                               const char *name)
  : forwardee_(&allocator)
{
}

ProxyAllocator::~ProxyAllocator() {
}

void *ProxyAllocator::allocate(size_t size, size_t alignment) {
  return forwardee_->allocate(size, alignment);
}

void *ProxyAllocator::reallocate(void *ptr, size_t size, size_t alignment) {
  return forwardee_->reallocate(ptr, size, alignment);
}

void ProxyAllocator::deallocate(void *ptr) {
  forwardee_->deallocate(ptr);
}

} // core
} // yeti
