//===-- yeti/foundation/array.h ---------------------------*- mode: C++ -*-===//
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

namespace yeti {
namespace foundation {

// TODO(mtwilliams): Call destructors for non-POD?
// TODO(mtwilliams): Specialized implementation for T = T'[N], i.e arrays.

/// ...
template <typename T>
class Array {
 public:
  explicit Array(Allocator *allocator, const u32 sz = 0);
  explicit Array(Allocator &allocator, const u32 sz = 0);
  Array(const Array<T> &ary);
  Array<T> &operator=(const Array<T> &ary);
  ~Array();

 public:
  void unshift(const T &element);
  void shift();
  void shift(T *element);

  void push(const T &element);
  void pop();
  void pop(T *element);

  void clear();
  void reserve(const u32 additional);
  void resize(const u32 new_sz);

  T *first();
  const T *first() const;

  T *last();
  const T *last() const;

  u32 reserved() const;
  u32 size() const;
  u32 length() const;
  bool empty() const;

 public:
  typedef bool Matcher(const T &, void *);

  /// \brief Returns true if @matcher returns true for every element.
  bool all(Matcher matcher, void *matcher_ctx = NULL) const;

  /// \brief Returns true if @matcher returns true for any element.
  bool any(Matcher matcher, void *matcher_ctx = NULL) const;

  /// \brief Returns true if @matcher returns true for only one element.
  bool one(Matcher matcher, void *matcher_ctx = NULL) const;

  /// \brief Returns true if @matcher returns false for every element.
  bool none(Matcher matcher, void *matcher_ctx = NULL) const;

 public:
  void reduce(void reducer(const T &, T &, void *), const T &initial, void *fn_ctx = NULL) const;
  void apply(void fn(T &, void *), void *fn_ctx = NULL);

 public:
  // TODO(mtwilliams): Implement sorting and searching.
  // void sort(...);
  // T search(...);

 private:
  Allocator *allocator_;
  T *first_, *last_, *reserved_;
};

template <typename T>
Array<T>::Array(Allocator *allocator, const u32 sz) {
  yeti_assert_debug(allocator != NULL);
  allocator_ = allocator;
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)allocator.allocate(sz * sizeof(T));
  last_ = first_ + sz * sizeof(T);
  reserved_ = last_;
}

template <typename T>
Array<T>::Array(Allocator &allocator, const u32 sz) {
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
  reserved_ first + ary.reserved() * sizeof(T);
}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &ary) {
  allocator_->deallocate((uintptr_t)first_);
  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)ary.allocator_->allocate(ary.reserved() * sizeof(T));
  memcpy((void *)first_, (const void *)ary.first(), ary.size() * sizeof(T));
  last_ = first_ + ary.size() * sizeof(T);
  reserved_ first + ary.reserved() * sizeof(T);
  return *this;
}

template <typename T>
Array<T>::~Array() {
  allocator_->deallocate((uintptr_t)last_);
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
void Array<T>::push(const T &element) {
  // TODO(mtwilliams): Grow logarithmically?
  this->resize(this->size() + 1);
  this->last_[-1] = element;
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
void Array<T>::reserve(const u32 additional) {
  const u32 size = this->size();
  const u32 reserved = this->reserved();

  // TODO(mtwilliams): Respect minimum alignment requirements of |T|.
  first_ = (T *)allocator_->reallocate((uintptr_t)first_, (reserved + additional) * sizeof(T));
  last_ = first_ + size;
  reserved_ = first_ + reserved + additional;
}

template <typename T>
void Array<T>::resize(const u32 new_sz) {
  const u32 size = this->size();
  const u32 reserved = this->reserved();
  const u32 remaining = reserved - size;

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
  return last_;
}

template <typename T>
const T *Array<T>::last() const {
  return last_;
}

template <typename T>
YETI_INLINE u32 Array<T>::reserved() const {
  return (reserved_ - first_) / sizeof(T);
}

template <typename T>
YETI_INLINE u32 Array<T>::size() const {
  return (last_ - first_) / sizeof(T);
}

template <typename T>
YETI_INLINE u32 Array<T>::length() const {
  return this->reserved();
}

template <typename T>
YETI_INLINE bool Array<T>::empty() const {
  return (first_ == last_);
}

template <typename T>
bool Array<T>::all(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  for (const T *E = this->first(); E != this->last(); E += sizeof(T))
    if (!matcher(E, matcher_ctx))
      return false;
  return true;
}

template <typename T>
bool Array<T>::any(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  for (const T *E = this->first(); E != this->last(); E += sizeof(T))
    if (matcher(E, matcher_ctx))
      return true;
  return false;
}

template <typename T>
bool Array<T>::one(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  u32 matches = 0;
  for (const T *E = this->first(); E != this->last(); E += sizeof(T))
    matches += matcher(E, matcher_ctx) ? 1 : 0;
  return (matches == 1);
}

template <typename T>
bool Array<T>::none(Matcher matcher, void *matcher_ctx) const {
  yeti_assert_debug(matcher != NULL);
  for (const T *E = this->first(); E != this->last(); E += sizeof(T))
    if (matcher(E, matcher_ctx))
      return false;
  return true;
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_ARRAY_H_
