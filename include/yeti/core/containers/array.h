//===-- yeti/core/containers/array.h --------------------*- mode: C++11 -*-===//
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
/// \brief Dynamically resizeable arrays.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_CONTAINERS_ARRAY_H_
#define _YETI_CORE_CONTAINERS_ARRAY_H_

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

/// \brief A dynamically resizeable array.
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
  /// Returns the position of the value at @pointer in the array.
  size_t index_from_pointer(const T *pointer) const;

  /// Returns a pointer to the element at @index in the array.
  T *pointer_from_index(size_t index);
  const T *pointer_from_index(size_t index) const;

 public:
  /// Reserves space for an element at the back of the array.
  T &emplace();

  /// Pushes @element to the back of the array.
  ///
  /// \return Index of newly appended element.
  ///
  size_t push(const T &element);

  /// Pops an element from the back of the array and copies it to @element.
  void pop(T *element);

  /// Pops an element from the back of the array.
  void pop();

  /// Returns a pointer to the first element in the array.
  /// @{
  T *begin();
  const T *begin() const;
  /// @}

  /// Returns a pointer past the last element in the array.
  /// @{
  T *end();
  const T *end() const;
  /// @}

  /// Resizes the array to store @size elements.
  void resize(size_t size);

  /// Grows the array to store @amount additional elements.
  void grow(size_t amount);

  /// Reserves spaces to hold @additional elements.
  void reserve(size_t additional);

  /// Empties the array.
  void clear();

  /// Returns the number of elements in the array.
  size_t size() const;

  /// Returns the amount of space allocated for elements.
  size_t reserved() const;

  /// Determines if the array contains no elements.
  bool empty() const;

 public:
  /// Returns a pointer to the first occurrence of @value in the the array.
  /// @{
  T *find(const T &value);
  const T *find(const T &value) const;
  /// @}

 public:
  /// Returns the position of the first occurance of @value in the array.
  size_t position(const T &value) const;

 public:
  T *raw() { return (T *)first_; }
  const T *raw() const { return (const T *)first_; }

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
  first_     = size ? (uintptr_t)allocator_->allocate(size * sizeof(T), alignof(T)) : NULL;
  last_      = first_ + size * sizeof(T);
  end_       = last_;
}

template <typename T>
Array<T>::Array(Allocator &allocator, size_t size) {
  allocator_ = &allocator;
  first_     = size ? (uintptr_t)allocator_->allocate(size * sizeof(T), alignof(T)) : NULL;
  last_      = first_ + size * sizeof(T);
  end_       = last_;
}

template <typename T>
Array<T>::Array(const Array<T> &array) {
  allocator_ = array.allocator_;

  first_     = size ? (uintptr_t)allocator_->allocate(array.last_ - array.first_, alignof(T)) : NULL;
  last_      = first_ + (array.last_ - array.first_);
  end_       = last_;

  memcpy((void *)first_, (const void *)array.first_, array.last_ - array.first_);
}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &array) {
  if (&array == this)
    return *this;

  if (allocator_)
    allocator_->deallocate((void *)first_);

  allocator_ = array.allocator_;

  first_     = (uintptr_t)allocator_->allocate(array.last_ - array.first_, alignof(T));
  last_      = first_ + (array.last_ - array.first_);
  end_       = last_;

  memcpy((void *)first_, (const void *)array.first_, array.last_ - array.first_);

  return *this;
}

template <typename T>
Array<T>::~Array() {
  if (allocator_)
    allocator_->deallocate((void *)first_);
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
size_t Array<T>::index_from_pointer(const T *pointer) const {
  yeti_assert_debug(uintptr_t(pointer) >= first_ && uintptr_t(pointer) < last_);
  return (size_t)(pointer - ((const T *)first_));
}

template <typename T>
T *Array<T>::pointer_from_index(size_t index) {
  return &(*this)[index];
}

template <typename T>
const T *Array<T>::pointer_from_index(size_t index) const {
  return &(*this)[index];
}

template <typename T>
T &Array<T>::emplace() {
  // TODO(mtwilliams): Grow logarithmically.
  this->resize(this->size() + 1);
  return ((T *)last_)[-1];
}

template <typename T>
size_t Array<T>::push(const T &element) {
  this->emplace() = element;
  return (this->size() - 1);
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
      first_ = (uintptr_t)allocator_->reallocate((void *)first_, size_in_bytes, alignof(T));
      last_  = first_ + size_in_bytes;
      end_   = last_;
    }
  } else if (size < current_in_bytes) {
    // TODO(mtwilliams): Shrink.
    last_ = first_ + size_in_bytes;
  }
}

template <typename T>
void Array<T>::grow(size_t amount) {
  // Requested amount in bytes.
  const size_t amount_in_bytes = amount * sizeof(T);

  if ((last_ + amount_in_bytes) <= end_) {
    // Grow into reserved.
    last_ = first_ + amount_in_bytes;
  } else {
    // Calculate new size.
    const size_t size_in_bytes = last_ - first_ + amount_in_bytes;

    // Grow.
    first_ = (uintptr_t)allocator_->reallocate((void *)first_, size_in_bytes, alignof(T));
    last_  = first_ + size_in_bytes;
    end_   = last_;
  }
}

template <typename T>
void Array<T>::reserve(size_t additional) {
  // Current size in bytes.
  const size_t size_in_bytes = (last_ - first_);
  const size_t reserved_in_bytes = (end_ - first_);

  // Reserve in bytes.
  const size_t additional_in_bytes = additional * sizeof(T);

  first_ = (uintptr_t)allocator_->reallocate((void *)first_, reserved_in_bytes + additional_in_bytes, alignof(T));
  last_  = first_ + size_in_bytes;
  end_   = first_ + reserved_in_bytes + additional_in_bytes;
}

template <typename T>
void Array<T>::clear() {
  allocator_->deallocate((void *)first_);
  first_ = last_ = end_ = NULL;
}

template <typename T>
size_t Array<T>::size() const {
  return (last_ - first_) / sizeof(T);
}

template <typename T>
size_t Array<T>::reserved() const {
  return (end_ - first_) / sizeof(T);
}

template <typename T>
bool Array<T>::empty() const {
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
  if (const T *I = this->find(value))
    return (I - this->begin());
  return ~(size_t)0;
}

} // core
} // yeti

#endif // _YETI_CORE_CONTAINERS_ARRAY_H_
