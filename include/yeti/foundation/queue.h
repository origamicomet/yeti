//===-- yeti/foundation/queue.h -------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_FOUNDATION_QUEUE_H_
#define _YETI_FOUNDATION_QUEUE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

// TODO(mtwilliams): Call destructors for non-POD?
// TODO(mtwilliams): Document `Queue<T>`.
template <typename T>
class Queue {
 // Copying a queue does not make a lot of sense. If you find yourself needing
 // to copy a queue, you probably don't want a queue.
 YETI_DISALLOW_COPYING(Queue)

 public:
  explicit Queue(uintptr_t mem, size_t sz);
  explicit Queue(Allocator *allocator, const size_t n);
  explicit Queue(Allocator &allocator, const size_t n);
  ~Queue();

 public:
  void push(const T &item);

  bool pop();
  bool pop(T *item);

  bool empty() const;
  bool full() const;

 private:
  Allocator *allocator_;
  T *Q;
  size_t N;
  size_t read_;
  size_t write_;
};

template <typename T>
Queue<T>::Queue(uintptr_t mem, size_t sz) {
  // TODO(mtwilliams): Use a `NullAllocator` rather than special casing.
  allocator_ = NULL;
  Q = (T *const)mem;
  N = sz / sizeof(T);
  read_ = write_ = 0;
}

template <typename T>
Queue<T>::Queue(Allocator *allocator, const size_t n) {
  yeti_assert_debug(allocator != NULL);
  allocator_ = allocator;
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  Q = (T *const)allocator->allocate(n * sizeof(T));
  N = n;
  read_ = write_ = 0;
}

template <typename T>
Queue<T>::Queue(Allocator &allocator, const size_t n) {
  allocator_ = &allocator;
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  Q = (T *const)allocator.allocate(n * sizeof(T));
  N = n;
  read_ = write_ = 0;
}

template <typename T>
Queue<T>::~Queue() {
  if (allocator_)
    allocator_->deallocate((uintptr_t)Q);
}

template <typename T>
void Queue<T>::push(const T &item) {
  yeti_assert_debug(!this->full());
  memcpy((void *)&Q[write_ % N], (const void *)&item, sizeof(T));
  write_ += 1;
}

template <typename T>
bool Queue<T>::pop() {
  if (write_ > read_) {
    read_ += 1;
    return true;
  }

  return false;
}

template <typename T>
bool Queue<T>::pop(T *item) {
  if (write_ > read_) {
    memcpy((void *)item, (const void *)&Q[read_ % N], sizeof(T));
    read_ += 1;
    return true;
  }

  return false;
}

template <typename T>
bool Queue<T>::empty() const {
  const size_t depth = write_ - read_;
  return (depth == 0);
}

template <typename T>
bool Queue<T>::full() const {
  const size_t depth = write_ - read_;
  return (depth == N);
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_QUEUE_H_
