//===-- yeti/foundation/thread_safe/pool.h ----------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_THREAD_SAFE_POOL_H_
#define _YETI_FOUNDATION_THREAD_SAFE_POOL_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {
namespace thread_safe {

// TODO(mtwilliams): Make the following code more clear.

template <typename T>
class Pool {
 YETI_DISALLOW_COPYING(Pool)

 private:
  struct Element {
    union {
      Element *next_available_element;
      T storage;
    };
  };

 public:
  Pool(Allocator &allocator, size_t n);
  Pool(uintptr_t mem, size_t sz);
  ~Pool();

 private:
  void initialize();

 public:
  T *acquire();
  void relinquish(T *borrowed);

 private:
  Allocator *allocator_;
  uintptr_t lower_, upper_;
  Element *free_;
};

template <typename T>
Pool<T>::Pool(Allocator &allocator, size_t n) {
  allocator_ = &allocator;

  lower_ = allocator.allocate(n * sizeof(Element));
  upper_ = lower_ + (n * sizeof(Element));

  this->initialize();
}

template <typename T>
Pool<T>::Pool(uintptr_t mem, size_t sz) {
  allocator_ = NULL;

  lower_ = mem;
  upper_ = mem + sz;

  this->initialize();
}

template <typename T>
Pool<T>::~Pool() {
  if (allocator_)
    allocator_->deallocate(lower_);
}

template <typename T>
void Pool<T>::initialize() {
  const size_t N = (upper_ - lower_) / sizeof(Element);
  free_ = (Element *)lower_;
  for (Element *E = free_; E < &free_[N-1]; ++E)
    E->next_available_element = &E[1];
  free_[N-1].next_available_element = NULL;
}

template <typename T>
T *Pool<T>::acquire() {
try_acquire:
  Element *element = (Element *)atomic::load((void **volatile const)&free_);

  if (element == NULL)
    // No more space.
    return NULL;

  Element *next = (Element *)atomic::load((void **volatile const)&element->next_available_element);

  if (atomic::cmp_and_xchg((void ** volatile)&free_, (void *)element, (void *)next) != (void *)element)
    goto try_acquire;

  // TODO(mtwilliams): Use type traits to call `T`'s default constructor.
  memset((void *)&element->storage, 0, sizeof(T));

  return &element->storage;
}

template <typename T>
void Pool<T>::relinquish(T *borrowed) {
  yeti_assert_debug((uintptr_t)borrowed >= lower_);
  yeti_assert_debug((uintptr_t)borrowed < upper_);

  Element *element = (Element *)borrowed;

try_relinquish:
  Element *head = (Element *)atomic::load((void **volatile const)&free_);
  element->next_available_element = head;

  if (atomic::cmp_and_xchg((void ** volatile)&free_, (void *)head, (void *)element) != (void *)head)
    goto try_relinquish;
}

} // thread_safe
} // foundation
} // yeti

#endif // _YETI_FOUNDATION_THREAD_SAFE_POOL_H_
