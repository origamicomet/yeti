//===-- yeti/foundation/allocator.cc --------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/allocator.h"

#include <stdlib.h>
#include <string.h>

#include "yeti/foundation/atomics.h"

namespace yeti {
namespace foundation {

Allocator::Allocator() {
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  ::memset(&name_[0], 0, sizeof(name_));
  allocations_ = NULL;
#endif
}

Allocator::~Allocator() {
}

void Allocator::allocated(Allocation *allocation) {
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  Allocation *head; do {
    head = (Allocation *)atomic::load((void ** const volatile)&allocations_);
    allocation->next = head;
  } while(atomic::cmp_and_xchg((void ** volatile)&allocations_, (const void *)head, (const void *)allocation) != (void *)head);
#else
  YETI_UNUSED(allocation);
#endif
}

void Allocator::reallocated(Allocation *allocation) {
  YETI_UNUSED(allocation);
}

void Allocator::deallocated(Allocation *allocation) {
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  // TODO(mtwilliams): Make this thread-safe.
  Allocation **I = &allocations_;
  while (*I) {
    if (*I == allocation) {
      *I = allocation->next;
      break;
    }
    I = &((*I)->next);
  }
#endif
}

const char *Allocator::name() const {
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  return &name_[0];
#else
  return (const char *)NULL;
#endif
}

void Allocator::set_name(const char *name) {
  yeti_assert_debug(name != NULL);
  yeti_assert_debug(strlen(name) <= 255);
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  const size_t name_len = strlen(name);
  memcpy(&name_[0], name, name_len + 1);
#else
  YETI_UNUSED(name);
#endif
}

const Allocation *Allocator::allocations() const {
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  return allocations_;
#else
  return (const Allocation *)NULL;
#endif
}

void Allocator::for_each_allocation(void (*vistor)(const Allocation *)) {
  yeti_assert_debug(vistor != NULL);
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  const Allocation *allocation = allocations_;
  while (allocation) {
    vistor(allocation);
    allocation = allocation->next;
  }
#else
  YETI_UNUSED(vistor);
#endif
}

void Allocator::for_each_allocation(void (*vistor)(const Allocation *, void *), void *ctx) {
#if (YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG) || \
    (YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT)
  const Allocation *allocation = allocations_;
  while (allocation) {
    vistor(allocation, ctx);
    allocation = allocation->next;
  }
#else
  YETI_UNUSED(vistor);
  YETI_UNUSED(ctx);
#endif
}

} // foundation
} // yeti
