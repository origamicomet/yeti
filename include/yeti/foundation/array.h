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

// TODO(mtwilliams): Call destructors for non-POD?
// TODO(mtwilliams): Specialized implementation for T = T'[N], i.e arrays.
// TODO(mtwilliams): Document `Array<T>`.

template <typename T>
class Array {
 public:
  Array();
  explicit Array(Allocator *allocator, const size_t sz = 0);
  explicit Array(Allocator &allocator, const size_t sz = 0);
  Array(const Array<T> &ary);
  Array<T> &operator=(const Array<T> &ary);
  ~Array();

 public:
  T &operator[](const size_t idx);
  const T &operator[](const size_t idx) const;

 public:
  void unshift(const T &element);
  void shift();
  void shift(T *element);

  T &emplace();

  void push(const T &element);

  void pop();
  void pop(T *element);

  void clear();
  void reserve(const size_t additional);
  void resize(const size_t new_sz);

  T *first();
  const T *first() const;

  T *last();
  const T *last() const;

  size_t reserved() const;
  size_t size() const;
  size_t length() const;
  bool empty() const;

 public:
  typedef bool Matcher(const T &, void *);

  /// \brief Returns true if @matcher returns true for only one element.
  bool one(Matcher matcher, void *matcher_ctx = NULL) const;

  /// \brief Returns true if @matcher returns false for every element.
  bool none(Matcher matcher, void *matcher_ctx = NULL) const;

  /// \brief Returns true if @matcher returns true for every element.
  bool all(Matcher matcher, void *matcher_ctx = NULL) const;

  /// \brief Returns true if @matcher returns true for any element.
  bool any(Matcher matcher, void *matcher_ctx = NULL) const;

 public:
  void reduce(void reducer(const T &, T &, void *), const T &initial, void *fn_ctx = NULL) const;
  void apply(void fn(T &, void *), void *fn_ctx = NULL);

 public:
  T *find(const T &value);
  const T *find(const T &value) const;

 public:
  size_t position(const T &value) const;

 public:
  // TODO(mtwilliams): Implement sorting and searching.
  // void sort(...);
  // T search(...);

 private:
  Allocator *allocator_;
  T *first_, *last_, *reserved_;
};

template <typename T>
Array<T>::Array() {
  allocator_ = NULL;
  first_ = last_ = reserved_ = NULL;
}

template <typename T>
Array<T>::Array(Allocator *allocator, const size_t sz) {
  yeti_assert_debug(allocator != NULL);
  allocator_ = allocator;
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)allocator->allocate(sz * sizeof(T));
  last_ = first_ + sz * sizeof(T);
  reserved_ = last_;
}

template <typename T>
Array<T>::Array(Allocator &allocator, const size_t sz) {
  allocator_ = &allocator;
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)allocator.allocate(sz * sizeof(T));
  last_ = first_ + sz * sizeof(T);
  reserved_ = last_;
}

template <typename T>
Array<T>::Array(const Array<T> &ary) {
  allocator_ = ary.allocator_;
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)ary.allocator_->allocate(ary.reserved() * sizeof(T));
  memcpy((void *)first_, (const void *)ary.first(), ary.size() * sizeof(T));
  last_ = first_ + ary.size() * sizeof(T);
  reserved_ = first_ + ary.reserved() * sizeof(T);
}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &ary) {
  allocator_->deallocate((uintptr_t)first_);
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)ary.allocator_->allocate(ary.reserved() * sizeof(T));
  memcpy((void *)first_, (const void *)ary.first(), ary.size() * sizeof(T));
  last_ = first_ + ary.size() * sizeof(T);
  reserved_ = first_ + ary.reserved() * sizeof(T);
  return *this;
}

template <typename T>
Array<T>::~Array() {
  if (allocator_)
    allocator_->deallocate((uintptr_t)first_);
}

template <typename T>
T &Array<T>::operator[](const size_t idx) {
  yeti_assert_debug(idx < size());
  return first_[idx];
}

template <typename T>
const T &Array<T>::operator[](const size_t idx) const {
  yeti_assert_debug(idx < size());
  return first_[idx];
}

// TODO(mtwilliams): Implement shifting and unshifting.

// template <typename T>
// void Array<T>::unshift(T *element) {
// }

// template <typename T>
// void Array<T>::shift() {
// }

// template <typename T>
// void Array<T>::shift(T &element) {
// }

template <typename T>
T &Array<T>::emplace() {
  // TODO(mtwilliams): Grow logarithmically?
  this->resize(this->size() + 1);
  return this->last_[-1];
}

template <typename T>
void Array<T>::push(const T &element) {
  this->emplace() = element;
}

template <typename T>
void Array<T>::pop() {
  // TODO(mtwilliams): Shrink logarithmically?
  this->resize(this->size() - 1);
}

template <typename T>
void Array<T>::pop(T *element) {
  // TODO(mtwilliams): Shrink logarithmically?
  *element = this->last_[-1];
  this->resize(this->size() - 1);
}

template <typename T>
void Array<T>::clear() {
  return this->resize(0);
}

template <typename T>
void Array<T>::reserve(const size_t additional) {
  const size_t size = this->size();
  const size_t reserved = this->reserved();

  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)allocator_->reallocate((uintptr_t)first_, (reserved + additional) * sizeof(T));
  last_ = first_ + size;
  reserved_ = first_ + reserved + additional;
}

template <typename T>
void Array<T>::resize(const size_t new_sz) {
  const size_t size = this->size();
  const size_t reserved = this->reserved();
  const size_t remaining = reserved - size;

  if (new_sz > size) {
    if ((new_sz - size) <= remaining) {
      last_ = first_ + (new_sz - size) * sizeof(T);
      return;
    }
  }

  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)allocator_->reallocate((uintptr_t)first_, new_sz * sizeof(T));
  last_ = first_ + new_sz;
  reserved_ = last_;
}

template <typename T>
T *Array<T>::first() {
  return first_;
}

template <typename T>
const T *Array<T>::first() const {
  return first_;
}

template <typename T>
T *Array<T>::last() {
  // TODO(mtwilliams): Figure out nicer way of doing this.
  return &last_[-1];
}

template <typename T>
const T *Array<T>::last() const {
  // TODO(mtwilliams): Figure out nicer way of doing this.
  return &last_[-1];
}

template <typename T>
YETI_INLINE size_t Array<T>::reserved() const {
  return (reserved_ - first_);
}

template <typename T>
YETI_INLINE size_t Array<T>::size() const {
  return (last_ - first_);
}

template <typename T>
YETI_INLINE size_t Array<T>::length() const {
  return this->size();
}

template <typename T>
YETI_INLINE bool Array<T>::empty() const {
  return (first_ == last_);
}

template <typename T>
bool Array<T>::one(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  // OPTIMIZE(mtwilliams): Early out on the second match.
  size_t matches = 0;
  for (const T *E = this->first(); E <= this->last(); E += sizeof(T))
    matches += matcher(E, matcher_ctx) ? 1 : 0;
  return (matches == 1);
}

template <typename T>
bool Array<T>::none(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  for (const T *E = this->first(); E <= this->last(); E += sizeof(T))
    if (matcher(E, matcher_ctx))
      return false;
  return true;
}

template <typename T>
bool Array<T>::all(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  for (const T *E = this->first(); E <= this->last(); E += sizeof(T))
    if (!matcher(E, matcher_ctx))
      return false;
  return true;
}

template <typename T>
bool Array<T>::any(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  for (const T *E = this->first(); E <= this->last(); E += sizeof(T))
    if (matcher(E, matcher_ctx))
      return true;
  return false;
}

template <typename T>
T *Array<T>::find(const T &value) {
  for (T *I = this->first(); I <= this->last(); ++I)
    if (*I == value)
      return I;

  return NULL;
}

template <typename T>
const T *Array<T>::find(const T &value) const {
  for (const T *I = this->first(); I <= this->last(); ++I)
    if (*I == value)
      return I;

  return NULL;
}

template <typename T>
size_t Array<T>::position(const T &value) const {
  const T *I = this->find(value);
  yeti_assert_debug(I != NULL);
  return (I - first_);
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_ARRAY_H_
