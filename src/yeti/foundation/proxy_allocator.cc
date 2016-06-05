//===-- yeti/foundation/proxy_allocator.cc ----------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/proxy_allocator.h"

#include <stdlib.h>

namespace yeti {
namespace foundation {

ProxyAllocator::ProxyAllocator(Allocator &allocator, const char *name) {
  yeti_assert_debug(name != NULL);
  this->allocator_ = &allocator;
  this->set_name(name);
}

ProxyAllocator::ProxyAllocator(Allocator *allocator, const char *name) {
  yeti_assert_debug(name != NULL);
  this->allocator_ = allocator;
  this->set_name(name);
}

ProxyAllocator::~ProxyAllocator() {
}

uintptr_t ProxyAllocator::allocate(size_t sz, size_t alignment) {
  return allocator_->allocate(sz, alignment);
}

uintptr_t ProxyAllocator::reallocate(uintptr_t ptr, size_t sz, size_t alignment) {
  return allocator_->reallocate(ptr, sz, alignment);
}

void ProxyAllocator::deallocate(uintptr_t ptr) {
  allocator_->deallocate(ptr);
}

} // foundation
} // yeti
