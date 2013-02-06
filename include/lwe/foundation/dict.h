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

#ifndef _LWE_FOUNDATION_DICT_H_
#define _LWE_FOUNDATION_DICT_H_

#include <lwe/foundation/array.h>
#include <lwe/foundation/pair.h>

#include <stdlib.h>

template <typename K, typename V>
struct lwe_dict_t {
  lwe_array_t< lwe_pair_t<K, V> > _sorted_array;
};

template <typename K, typename V>
LWE_INLINE int _lwe_dict_sort_comparator( const void* a, const void* b ) {
  lwe_pair_t<K, V>* ap = (lwe_pair_t<K, V>*)a;
  lwe_pair_t<K, V>* bp = (lwe_pair_t<K, V>*)b;

  if (ap->key > bp->key) return 1;
  if (ap->key < bp->key) return -1;

  return 0;
}

template <typename K, typename V>
LWE_INLINE int _lwe_dict_search_comparator( const void* k, const void* v ) {
  K key = *((K*)k);
  lwe_pair_t<K, V>* value = (lwe_pair_t<K, V>*)v;

  if (key > value->key) return 1;
  if (key < value->key) return -1;
  
  return 0;
}

template <typename K, typename V>
LWE_INLINE void _lwe_dict_sort( lwe_dict_t<K, V>* dict ) {
  qsort(
    (void*)lwe_array_to_ptr(&dict->_sorted_array),
    dict->_sorted_array.size,
    sizeof(lwe_pair_t<K, V>),
    &_lwe_dict_sort_comparator<K, V>
  );
}

template <typename K, typename V>
LWE_INLINE lwe_pair_t<K, V>* _lwe_dict_search( lwe_dict_t<K, V>* dict, K key ) {
  return (lwe_pair_t<K, V>*)bsearch(
    (void*)&key,
    (void*)lwe_array_to_ptr(&dict->_sorted_array),
    dict->_sorted_array.size,
    sizeof(lwe_pair_t<K, V>),
    &_lwe_dict_search_comparator<K, V>
  );
}

template <typename K, typename V>
LWE_INLINE V* lwe_dict_find( lwe_dict_t<K, V>* dict, K key ) {
  lwe_pair_t<K, V>* pair = _lwe_dict_search(dict, key);

  if (!pair)
    return NULL;

  return &pair->value;
}

template <typename K, typename V>
LWE_INLINE void lwe_dict_replace( lwe_dict_t<K, V>* dict, K key, V new_val ) {
  lwe_pair_t<K, V>* pair = _lwe_dict_search(dict, key);

  if (!pair)
    return;

  memcpy((void*)&pair->value, (void*)&new_val, sizeof(V));
}

template <typename K, typename V>
LWE_INLINE void lwe_dict_insert( lwe_dict_t<K, V>* dict, K key, V value ) {
  lwe_pair_t<K, V> pair;
  memcpy((void*)&pair.key, (void*)&key, sizeof(K));
  memcpy((void*)&pair.value, (void*)&value, sizeof(V));
  lwe_array_push(&dict->_sorted_array, &pair);
  _lwe_dict_sort(dict);
}

#endif // _LWE_FOUNDATION_DICT_H_