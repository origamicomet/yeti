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
 YETI_DISALLOW_COPYING(Pool);

 private:
  struct Element {
    union {
      Element *next_available_element;
      T storage;
    };
  };

 public:
  Pool(uintptr_t mem, size_t sz);
  ~Pool();

 public:
  T *acquire();
  void relinquish(T *borrowed);

 private:
  Element *head_;
};

template <typename T>
Pool<T>::Pool(uintptr_t mem, size_t sz) {
  static const size_t N = sz / sizeof(Element);
  head_ = (Element *)mem;
  for (Element *E = head_; E < &head_[N-1]; ++E)
    E->next_available_element = &E[1];
  head_[N-1].next_available_element = NULL;
}

template <typename T>
Pool<T>::~Pool() {
}

template <typename T>
T *Pool<T>::acquire() {
try_acquire:
  Element *element = (Element *)atomic::load((void **volatile const)&head_);

  if (element == NULL)
    // No more space.
    return NULL;

  Element *next = (Element *)atomic::load((void **volatile const)&element->next_available_element);

  if (atomic::cmp_and_xchg((void ** volatile)&head_, (void *)element, (void *)next) != (void *)element)
    goto try_acquire;

  return &element->storage;
}

template <typename T>
void Pool<T>::relinquish(T *borrowed) {
  Element *element = (Element *)borrowed;

try_relinquish:
  Element *head = (Element *)atomic::load((void **volatile const)&head_);
  element->next_available_element = head;

  if (atomic::cmp_and_xchg((void ** volatile)&head_, (void *)head, (void *)element) != (void *)head)
    goto try_relinquish;
}

} // thread_safe
} // foundation
} // yeti

#endif // _YETI_FOUNDATION_THREAD_SAFE_POOL_H_
