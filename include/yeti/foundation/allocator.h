//===-- yeti/foundation/atomics.h -------------------------*- mode: C++ -*-===//
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

namespace yeti {
namespace foundation {

/// ...
struct Allocation {
  /// ...
  class Allocator *allocator;
  /// ...
  Allocation *next;
  /// ...
  Pointer start;
  /// ...
  Pointer end;
  /// Describes where the allocation was made in the codebase.
  struct {
    const char *file;
    Size line;
  } location;
};

/// ...
class Allocator {
 YETI_DISALLOW_COPYING(Allocator);

 protected:
  Allocator();
  virtual ~Allocator();

 public:
  // TODO(mtwilliams): Document this interface.
  virtual Pointer allocate(Size sz, Size alignment = 16) = 0;
  virtual Pointer reallocate(Pointer ptr, Size sz, Size alignment = 16) = 0;
  virtual void deallocate(Pointer ptr) = 0;

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

#endif // _YETI_FOUNDATION_ALLOCATOR_H_
