//===-- yeti/foundation/proxy_allocator.h ---------------*- mode: C++11 -*-===//
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

#ifndef _YETI_FOUNDATION_PROXY_ALLOCATOR_H_
#define _YETI_FOUNDATION_PROXY_ALLOCATOR_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

#include "yeti/foundation/allocator.h"

namespace yeti {
namespace foundation {

class YETI_PUBLIC ProxyAllocator : public Allocator {
 YETI_DISALLOW_COPYING(ProxyAllocator)

 public:
  explicit ProxyAllocator(Allocator &allocator, const char *name);
  explicit ProxyAllocator(Allocator *allocator, const char *name);
  ~ProxyAllocator();

 public:
  uintptr_t allocate(size_t sz, size_t alignment = 16);
  uintptr_t reallocate(uintptr_t ptr, size_t sz, size_t alignment = 16);
  void deallocate(uintptr_t ptr);

 private:
  Allocator *allocator_;
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_PROXY_ALLOCATOR_H_
