//===-- yeti/core/allocator.h ---------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_ALLOCATOR_H_
#define _YETI_CORE_ALLOCATOR_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

#include "yeti/core/memory.h"

namespace yeti {
namespace core {

/// \brief Common interface that encapsulates strategies for allocation,
/// deallocation, and tracking of memory.
class YETI_PUBLIC Allocator {
 YETI_DISALLOW_COPYING(Allocator)

 protected:
  Allocator();
  virtual ~Allocator();

 public:
  virtual void *allocate(size_t size, size_t alignment = 16) = 0;
  virtual void *reallocate(void *ptr, size_t size, size_t alignment = 16) = 0;
  virtual void deallocate(void *ptr) = 0;
};

} // core
} // yeti

// Need to include to pickup placement new.
#include <new>

/// \def YETI_NEW
///
/// \brief Allocates memory from @Allocator for the given type @T, then calls
/// its constructor.
///
#define YETI_NEW(T, Allocator) \
  new (Allocator.allocate(sizeof(T), alignof(T))) T

/// \def YETI_DELETE
///
/// \brief Destructs @Instance and deallocates memory from @Allocator.
///
#define YETI_DELETE(T, Allocator, Instance) \
  { (Instance)->~T(); Allocator.deallocate((void *)Instance); }

#endif // _YETI_CORE_ALLOCATOR_H_
