// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#ifndef _LWE_FOUNDATION_QUEUE_H_
#define _LWE_FOUNDATION_QUEUE_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>
#include <lwe/foundation/allocator.h>
#include <lwe/foundation/util.h>

template <typename T>
struct lwe_queue_t {
  lwe_queue_t() { _size = _read = _write = 0; _internal = NULL; }
  ~lwe_queue_t() { _internal = lwe_realloc((void*)_internal, 0); }

  lwe_size_t _size;
  lwe_size_t _read;
  lwe_size_t _write;
  T* _internal;
};

template <typename T>
LWE_INLINE bool lwe_queue_empty(
  lwe_queue_t<T>* queue )
{
  lwe_assert(queue != NULL);
  return (queue->_read == queue->_write);
}

template <typename T>
LWE_INLINE bool lwe_queue_full(
  lwe_queue_t<T>* queue )
{
  lwe_assert(queue != NULL);
  return ((queue->_read + queue->_size) == queue->_write);
}

template <typename T>
void lwe_queue_resize(
  lwe_queue_t<T>* queue,
  lwe_size_t size )
{
  lwe_assert(queue != NULL);
  
  queue->_size = size;
  queue->_read = queue->_write = 0;
  queue->_internal = (T*)lwe_realloc((void*)queue->_internal, size * sizeof(T));
}

template <typename T>
LWE_INLINE void lwe_queue_enqueue(
  lwe_queue_t<T>* queue,
  T* item )
{
  lwe_assert(queue != NULL);
  lwe_assert(!lwe_queue_full(queue));

  queue->_internal[(queue->_write++) % queue->_size] = *item;
}

template <typename T>
LWE_INLINE T* lwe_queue_queued(
  lwe_queue_t<T>* queue )
{
  lwe_assert(queue != NULL);
  return &queue->_internal[(queue->_read) % queue->_size];
}

template <typename T>
LWE_INLINE void lwe_queue_dequeue(
  lwe_queue_t<T>* queue )
{
  lwe_assert(queue != NULL);
  queue->_read++;
}

template <typename T>
LWE_INLINE void lwe_queue_fast_forward(
  lwe_queue_t<T>* queue )
{
  lwe_assert(queue != NULL);
  queue->_read = queue->_write;
}

#endif // _LWE_FOUNDATION_QUEUE_H_