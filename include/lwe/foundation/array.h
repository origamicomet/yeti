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

#ifndef _LWE_FOUNDATION_ARRAY_H_
#define _LWE_FOUNDATION_ARRAY_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>
#include <lwe/foundation/allocator.h>

template <typename T>
struct lwe_array_t {
  lwe_array_t() { size = reserved = 0; _internal = NULL; }
  ~lwe_array_t() { size = reserved = 0; lwe_realloc((void*)_internal, 0); }

  lwe_size_t size;
  lwe_size_t reserved;
  T* _internal;
};

template <typename T>
LWE_INLINE void lwe_array_reserve(
  lwe_array_t<T>* array,
  lwe_size_t to_reserve )
{
  lwe_assert(array != NULL);

  array->reserved += to_reserve;
  array->_internal =
    (T*)lwe_realloc((void*)array->_internal, array->reserved * sizeof(T));
}

template <typename T>
LWE_INLINE void lwe_array_resize(
  lwe_array_t<T>* array,
  lwe_size_t new_size )
{
  lwe_assert(array != NULL);

  array->size = array->reserved = new_size;
  array->_internal =
    (T*)lwe_realloc((void*)array->_internal, new_size * sizeof(T));
}

template <typename T>
LWE_INLINE T* lwe_array_index(
  lwe_array_t<T>* array,
  lwe_size_t index )
{
  lwe_assert(array != NULL);
  lwe_assert(index <= array->size);
  return &array->_internal[index];
}

template <typename T>
LWE_INLINE lwe_size_t lwe_array_push(
  lwe_array_t<T>* array,
  T* value )
{
  lwe_assert(array != NULL);

  if (array->size == array->reserved)
    lwe_array_reserve(array, 16);

  lwe_size_t index = array->size = array->size + 1;
  memcpy((void*)&array->_internal[index - 1], (void*)value, sizeof(T));
  return index - 1;
}

template <typename T>
LWE_INLINE void lwe_array_pop(
  lwe_array_t<T>* array )
{
  lwe_assert(array != NULL);

  if ((array->reserved - array->size) >= 16)
    lwe_array_resize(array, array->size - 1);

  array->size -= 1;
}

template <typename T>
LWE_INLINE T* lwe_array_to_ptr(
  lwe_array_t<T>* array )
{
  lwe_assert(array != NULL);
  return array->_internal;
}

#endif // _LWE_FOUNDATION_ARRAY_H_