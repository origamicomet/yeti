//===-- yeti/core/containers/queue.h --------------------*- mode: C++11 -*-===//
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
/// \file
/// \brief Fixed size queues.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_CONTAINERS_QUEUE_H_
#define _YETI_CORE_CONTAINERS_QUEUE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

#include "yeti/core/memory.h"
#include "yeti/core/allocator.h"

// For bounds checking in debug builds.
#include "yeti/core/debug/assert.h"

namespace yeti {
namespace core {

/// \brief A fixed size queue.
template <typename T>
class Queue {
 public:
  Queue();

  explicit Queue(Allocator *allocator, size_t size = 0);
  explicit Queue(Allocator &allocator, size_t size = 0);

  Queue(const Queue<T> &queue);
  Queue<T> &operator=(const Queue<T> &queue);

  ~Queue();

 public:
  /// Reserves space at the back of the queue for an element.
  T &emplace();

  /// Pushes @element to the back of the queue.
  void push(const T &element);

  /// Pops an element from the front of the queue and copies it to @element.
  /// \return If an element was popped.
  bool pop(T *element);

  /// Pops an element from the queue.
  /// \return If an element was popped.
  bool pop();

  /// Returns the maximum number of elements that can be stored in the queue.
  size_t size() const;

  /// Computes the number of elements in the queue.
  size_t depth() const;

  /// Determines if the queue can't hold any more elements.
  bool full() const;

  /// Determines if the queue doesn't hold any elements.
  bool empty() const;

 private:
  Allocator *allocator_;

  T *elements_;

  size_t size_;

  size_t read_;
  size_t write_;
};

template <typename T>
Queue<T>::Queue() {
  allocator_ = NULL;
  elements_  = NULL;
  size_      = 0;
  read_      = 0;
  write_     = 0;
}

template <typename T>
Queue<T>::Queue(Allocator *allocator, size_t size) {
  allocator_ = allocator;
  elements_  = (T *)allocator_->allocate(size * sizeof(T), alignof(T));
  size_      = size;
  read_      = 0;
  write_     = 0;
}

template <typename T>
Queue<T>::Queue(Allocator &allocator, size_t size) {
  allocator_ = &allocator;
  elements_  = (T *)allocator_->allocate(size * sizeof(T), alignof(T));
  size_      = size;
  read_      = 0;
  write_     = 0;
}

template <typename T>
Queue<T>::~Queue() {
  if (allocator_)
    allocator_->deallocate((void *)elements_);
}

template <typename T>
T &Queue<T>::emplace() {
  yeti_assert_debug(!this->full());
  return elements_[write_++ % size_];
}

template <typename T>
void Queue<T>::push(const T &element) {
  this->emplace() = element;
}

template <typename T>
bool Queue<T>::pop(T *element) {
  if (read_ == write_)
    return false;

  *element = elements_[read_++ % size_];

  return true;
}

template <typename T>
bool Queue<T>::pop() {
  if (read_ == write_)
    return false;

  read_++;

  return true;
}

template <typename T>
size_t Queue<T>::size() const {
  return size_;
}

template <typename T>
size_t Queue<T>::depth() const {
  if (write_ >= read_)
    return (write_ - read_);
  else
    // Wrapped around.
    return ((UINT32_MAX - read_) + write_);
}

template <typename T>
bool Queue<T>::full() const {
  return (depth() == size());
}

template <typename T>
bool Queue<T>::empty() const {
  return (read_ == write_);
}

} // core
} // yeti

#endif // _YETI_CORE_CONTAINERS_QUEUE_H_
