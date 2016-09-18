//===-- yeti/foundation/thread_safe/sp_mc_deque.h ---------*- mode: C++ -*-===//
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
/// \brief Single-producer, multiple-consumer, double-ended queue.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_THREAD_SAFE_SP_MC_DEQUE_H_
#define _YETI_FOUNDATION_THREAD_SAFE_SP_MC_DEQUE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {
namespace thread_safe {

// TODO(mtwilliams): Use type-traits to only allow pointers, or specialize in
// the case of integers, i.e. indexes.

// OPTIMIZE(mtwilliams): Force queue size to a multiple of two, thus allowing
// us to use a simple bitwise AND to dereference an index.

/// \brief A lock-free, single-producer, multiple-consumer, doubly-ended queue.
///
/// \details Provides a private interface to @push and @pop elements in LIFO
/// order for the single producer and consumer thread. Also provides a public
/// interface to let other consumer threads @steal elements in FIFO order.
///
/// This is based on the work of D. Chase and Y. Lev as per their paper
/// "Dynamic Circular Work-Stealing Deque." A more approachable description is
/// provided by S. Reinalter on his blog.
///
/// \warning Do not use @pop in any thread other than the producer thread.
///
template <typename T>
class SpMcDeque {
 // Copying a SpMcDequeue does not make any sense, whatsoever.
 YETI_DISALLOW_COPYING(SpMcDeque);

 public:
  SpMcDeque(uintptr_t mem, size_t sz);
  ~SpMcDeque();

 public:
  /// \brief Pushes @E to the private end of the queue.
  u64 push(const T &E);

  /// \brief Pops an element from the private end of the queue.
  /// \returns An element, or NULL when the queue is empty.
  T pop();

  /// \briefs Steals an element from the public end of the queue.
  /// \returns
  T steal();

 private:
  T *const Q;
  const u64 N;
  u64 top_;
  u64 bottom_;
};


template <typename T>
SpMcDeque<T>::SpMcDeque(uintptr_t mem, size_t sz)
  : Q((T *)mem)
  , N(sz / sizeof(T))
  , top_(0), bottom_(0)
{
}

template <typename T>
SpMcDeque<T>::~SpMcDeque() {
}

// TODO(mtwilliams): Insert compiler and memory barriers.

template <typename T>
u64 SpMcDeque<T>::push(const T& E) {
  const u64 bottom = atomic::load(&bottom_);
  const u64 top = atomic::load(&top_);

  // Make sure we won't overflow the queue.
  yeti_assert_debug(top - bottom <= N);

  atomic::store((volatile void **)&Q[bottom % N], (const void *)E);
  atomic::store(&bottom_, bottom + 1);

  return (top - bottom - 1);
}

template <typename T>
T SpMcDeque<T>::pop() {
  const u64 bottom = atomic::load(&bottom_) - 1;
  atomic::store(&bottom_, bottom);

  const u64 top = atomic::load(&top_);

  if (top <= bottom) {
    // Non-empty.
    T element = (T)atomic::load((const volatile void **)&Q[bottom % N]);

    if (top != bottom)
      // Still more than one element left in the queue.
      return element;

    // This is the last element in the queue; potential race against @steal.
    if (atomic::cmp_and_xchg(&top_, top, top + 1) != top)
      element = NULL;

    atomic::store(&bottom_, top + 1);
    return element;
  } else {
    // Empty.
    atomic::store(&bottom_, top);
    return NULL;
  }
}

template <typename T>
T SpMcDeque<T>::steal() {
  const u64 top = atomic::load(&top_);
  const u64 bottom = atomic::load(&bottom_);

  if (top < bottom) {
    // Non-empty.
    T element = (T)atomic::load((const volatile void **)&Q[top % N]);

    if (atomic::cmp_and_xchg(&top_, top, top + 1) != top)
      // Lost race against @pop or another @steal.
      return NULL;

    return element;
  } else {
    // Empty.
    return NULL;
  }
}

} // thread_safe
} // foundation
} // yeti

#endif // _YETI_FOUNDATION_THREAD_SAFE_SP_MC_DEQUE_H_
