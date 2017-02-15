//===-- yeti/foundation/allocator.h -----------------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_ALLOCATOR_H_
#define _YETI_FOUNDATION_ALLOCATOR_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

#include <stdlib.h>
#include <string.h>

#include <new>

namespace yeti {
namespace foundation {

// TODO(mtwilliams): Track if allocations need to be ignored in sumation.

/// ...
struct Allocation {
  /// ...
  class Allocator *allocator;
  /// ...
  Allocation *next;
  /// ...
  uintptr_t start;
  /// ...
  uintptr_t end;
  /// Describes where the allocation was made in the codebase.
  struct {
    const char *file;
    size_t line;
  } location;
};

/// ...
class YETI_PUBLIC Allocator {
 YETI_DISALLOW_COPYING(Allocator)

 protected:
  Allocator();
  virtual ~Allocator();

 public:
  // TODO(mtwilliams): Document this interface.
  virtual uintptr_t allocate(size_t sz, size_t alignment = 16) = 0;
  virtual uintptr_t reallocate(uintptr_t ptr, size_t sz, size_t alignment = 16) = 0;
  virtual void deallocate(uintptr_t ptr) = 0;

 protected:
  // TODO(mtwilliams): Document this interface.
  void allocated(Allocation *allocation);
  void reallocated(Allocation *allocation);
  void deallocated(Allocation *allocation);

 public:
  // TODO(mtwilliams): Document this function.
  const char *name() const;
  // TODO(mtwilliams): Document this function.
  void set_name(const char *name);

  // TODO(mtwilliams): Document this function.
  const Allocation *allocations() const;
  // TODO(mtwilliams): Document these functions.
  void for_each_allocation(void (*vistor)(const Allocation *));
  void for_each_allocation(void (*vistor)(const Allocation *, void *), void *ctx);

 private:
  #if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
      (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
    char name_[256];
    Allocation *allocations_;
  #endif
};

} // foundation
} // yeti

// BUG(mtwilliams): The pointer variant of this is likely a placement new. A
// fix is to throw a tag (and provide a default value.)
extern YETI_PUBLIC void *operator new(size_t sz, yeti::foundation::Allocator &allocator);
extern YETI_PUBLIC void *operator new(size_t sz, yeti::foundation::Allocator *allocator);
extern YETI_PUBLIC void *operator new[](size_t sz, yeti::foundation::Allocator &allocator);
extern YETI_PUBLIC void *operator new[](size_t sz, yeti::foundation::Allocator *allocator);

extern YETI_PUBLIC void operator delete(void *ptr, yeti::foundation::Allocator &allocator);
extern YETI_PUBLIC void operator delete(void *ptr, yeti::foundation::Allocator *allocator);

/// \def YETI_NEW
/// \brief Allocates memory with @Allocator for the given type @T, then calls
/// its constructor.
#define YETI_NEW(T, Allocator) \
  new ((void *)(Allocator).allocate(sizeof(T), alignof(T))) T

/// \def YETI_DELETE
/// \brief Destructs @Instance and deallocates memory using @Allocator.
#define YETI_DELETE(T, Allocator, Instance) \
  { (Instance)->~T(); (Allocator).deallocate((uintptr_t)Instance); }

#endif // _YETI_FOUNDATION_ALLOCATOR_H_
