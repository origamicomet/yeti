//===-- yeti/foundation/array.h -------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_FOUNDATION_ARRAY_H_
#define _YETI_FOUNDATION_ARRAY_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

#include "yeti/foundation/allocator.h"

namespace yeti {
namespace foundation {

template <typename T>
class Array {
 public:
  Array();

  explicit Array(Allocator *allocator, size_t size = 0);
  explicit Array(Allocator &allocator, size_t size = 0);

  Array(const Array<T> &array);
  Array<T> &operator=(const Array<T> &array);

  ~Array();

 public:
  T &operator[](size_t index);
  const T &operator[](size_t index) const;

 public:
  void push(const T &element);
  T &emplace();

  void pop(T *element);
  void pop();

  T *begin();
  const T *begin() const;

  T *end();
  const T *end() const;

  void resize(size_t size);
  void reserve(size_t additional);

  void clear();

  size_t size() const;
  size_t reserved() const;

  bool empty() const;

 public:
  T *find(const T &value);
  const T *find(const T &value) const;

 public:
  size_t position(const T &value) const;

 private:
  Allocator *allocator_;

  uintptr_t first_;
  uintptr_t last_;
  uintptr_t end_;
};

template <typename T>
Array<T>::Array() {
  allocator_ = NULL;
  first_ = last_ = end_ = NULL;
}

template <typename T>
Array<T>::Array(Allocator *allocator, size_t size) {
  allocator_ = allocator;
  first_     = size ? allocator_->allocate(size * sizeof(T), alignof(T)) : NULL;
  last_      = first_ + size * sizeof(T);
  end_       = last_;
}

template <typename T>
Array<T>::Array(Allocator &allocator, size_t size) {
  allocator_ = &allocator;
  first_     = size ? allocator_->allocate(size * sizeof(T), alignof(T)) : NULL;
  last_      = first_ + size * sizeof(T);
  end_       = last_;
}

template <typename T>
Array<T>::Array(const Array<T> &array) {
  allocator_ = array.allocator_;

  first_     = size ? allocator_->allocate(array.last_ - array.first_, alignof(T)) : NULL;
  last_      = first_ + (array.last_ - array.first_);
  end_       = last_;

  memcpy((void *)first_, (const void *)array.first_, array.last_ - array.first_);
}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &array) {
  if (&array == this)
    return *this;

  if (allocator_)
    allocator_->deallocate(first_);

  allocator_ = array.allocator_;

  first_     = allocator_->allocate(array.last_ - array.first_, alignof(T));
  last_      = first_ + (array.last_ - array.first_);
  end_       = last_;

  memcpy((void *)first_, (const void *)array.first_, array.last_ - array.first_);

  return *this;
}

template <typename T>
Array<T>::~Array() {
  if (allocator_)
    allocator_->deallocate(first_);
}

template <typename T>
T &Array<T>::operator[](size_t index) {
  yeti_assert_debug(index < size());
  return ((T *)first_)[index];
}

template <typename T>
const T &Array<T>::operator[](size_t index) const {
  yeti_assert_debug(index < size());
  return ((const T *)first_)[index];
}

template <typename T>
void Array<T>::push(const T &element) {
  this->emplace() = element;
}

template <typename T>
T &Array<T>::emplace() {
  // TODO(mtwilliams): Grow logarithmically.
  this->resize(this->size() + 1);
  return ((T *)last_)[-1];
}

template <typename T>
void Array<T>::pop(T *element) {
  // TODO(mtwilliams): Shrink logarithmically.
  *element = ((T *)last_)[-1];
  this->resize(this->size() - 1);
}

template <typename T>
void Array<T>::pop() {
  // TODO(mtwilliams): Shrink logarithmically.
  this->resize(this->size() - 1);
}

template <typename T>
T *Array<T>::begin() {
  return (T *)first_;
}

template <typename T>
const T *Array<T>::begin() const {
  return (const T *)first_;
}

template <typename T>
T *Array<T>::end() {
  return ((T *)last_);
}

template <typename T>
const T *Array<T>::end() const {
  return ((const T *)last_);
}

template <typename T>
void Array<T>::resize(size_t size) {
  // Requested size in bytes.
  const size_t size_in_bytes = size * sizeof(T);

  // Current size in bytes.
  const size_t current_in_bytes = (last_ - first_);

  if (size_in_bytes > current_in_bytes) {
    if ((first_ + size_in_bytes) <= end_) {
      // Grow into reserved.
      last_ = first_ + size_in_bytes;
    } else {
      // Grow.
      first_ = allocator_->reallocate(first_, size_in_bytes, alignof(T));
      last_  = first_ + size_in_bytes;
      end_   = last_;
    }
  } else if (size < current_in_bytes) {
    // Shrink.
    last_ = first_ + size_in_bytes;
  }
}

template <typename T>
void Array<T>::reserve(size_t additional) {
  // Current size in bytes.
  const size_t size_in_bytes = (last_ - first_);
  const size_t reserved_in_bytes = (end_ - first_);

  // Reserve in bytes.
  const size_t additional_in_bytes = additional * sizeof(T);

  first_ = allocator_->reallocate(first_, reserved_in_bytes + additional_in_bytes, alignof(T));
  last_  = first_ + size_in_bytes;
  end_   = first_ + reserved_in_bytes + additional_in_bytes;
}

template <typename T>
void Array<T>::clear() {
  allocator_->deallocate(first_);
  first_ = last_ = end_ = NULL;
}

template <typename T>
YETI_INLINE size_t Array<T>::size() const {
  return (last_ - first_) / sizeof(T);
}

template <typename T>
YETI_INLINE size_t Array<T>::reserved() const {
  return (end_ - first_) / sizeof(T);
}

template <typename T>
YETI_INLINE bool Array<T>::empty() const {
  return (first_ == last_);
}

template <typename T>
T *Array<T>::find(const T &value) {
  for (T *I = this->begin(); I <= this->end(); ++I)
    if (*I == value)
      return I;

  return NULL;
}

template <typename T>
const T *Array<T>::find(const T &value) const {
  for (const T *I = this->begin(); I <= this->end(); ++I)
    if (*I == value)
      return I;

  return NULL;
}

template <typename T>
size_t Array<T>::position(const T &value) const {
  const T *I = this->find(value);
  yeti_assert_debug(I != NULL);
  return (I - this->begin());
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_ARRAY_H_
