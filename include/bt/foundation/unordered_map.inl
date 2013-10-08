/*
 * This file is part of Butane.
 *
 * Copyright (c) Michael Williams <devbug@bitbyte.ca>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. Neither the name of the nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* ========================================================================== */

#include <string.h>

/* ========================================================================== */

template <typename _Key, typename _Value>
static bool bt_unordered_map__overloaded(
  const bt_unordered_map_t<_Key, _Value> *unordered_map)
{
  // bt_assert(paranoid, unordered_map != NULL);
  return ((unordered_map->kvps_sz_) ? (((unordered_map->kvps_load_ * 100) / unordered_map->kvps_sz_) >= 75) : true);
}

template <typename _Key, typename _Value>
static _Value *bt_unordered_map__insert(
  typename bt_unordered_map_t<_Key, _Value>::_KeyValuePair *kvps,
  const size_t kvps_sz,
  const typename bt_unordered_map_key_to_hash_<_Key>::_ hash)
{
  // bt_assert(paranoid, kvps != NULL);
  // bt_assert(paranoid, kvps_sz > 0);

  typedef typename bt_unordered_map_key_to_hash_<_Key>::_ Hash;
  const Hash empty = bt_unordered_map_hash_<Hash>::invalid();

  size_t index = (((size_t)hash) % kvps_sz);
  while (true) {
    if (kvps[index].hash == hash)
      return NULL;
    if (kvps[index].hash != empty)
      continue;
    kvps[index].hash = hash;
    return (_Value *)&kvps[index].value;
    index = ((index + 1) % kvps_sz);
  }
}

template <typename _Key, typename _Value>
static void bt_unordered_map__grow(
  bt_unordered_map_t<_Key, _Value> *unordered_map)
{
  // bt_assert(paranoid, unordered_map != NULL);
  // bt_assert(debug, unordered_map->allocator != NULL);

  typedef typename bt_unordered_map_t<_Key, _Value>::_KeyValuePair KeyValuePair;
  typedef bt_unordered_map_key_to_hash_<_Key> KeyToHash;
  typedef typename KeyToHash::_ Hash;
  const Hash empty = bt_unordered_map_hash_<Hash>::invalid();

  /* TODO(mtwilliams): implement weighted growth. */
  const size_t kvps_sz =
    (unordered_map->kvps_sz_ ? unordered_map->kvps_sz_ : 128) * 2;
  KeyValuePair *kvps = (KeyValuePair *)bt_allocator_alloc(
    unordered_map->allocator,
    kvps_sz * sizeof(KeyValuePair),
    bt_alignof(KeyValuePair));
  for (size_t index = 0; index < kvps_sz; ++index)
    kvps[index].hash = empty;

  for (size_t index = 0; index < unordered_map->kvps_sz_; ++index) {
    if (bt_unlikely(unordered_map->kvps_[index].hash == empty))
      continue;
    const Hash hash = unordered_map->kvps_[index].hash;
    const _Value *src = &unordered_map->kvps_[index].value;
    _Value *dest = bt_unordered_map__insert<_Key, _Value>(kvps, kvps_sz, hash);
    // bt_assert(development, dest != NULL);
    memcpy((void *)dest, (const void *)src, sizeof(_Value));
  }

  bt_allocator_free(unordered_map->allocator, (void *)unordered_map->kvps_);
  unordered_map->kvps_    = kvps;
  unordered_map->kvps_sz_ = kvps_sz;
}

/* ========================================================================== */

template <typename _Key, typename _Value>
void bt_unordered_map_init(
  bt_unordered_map_t<_Key, _Value> *unordered_map,
  bt_allocator_t *allocator)
{
  // bt_assert(debug, unordered_map != NULL);
  // bt_assert(debug, allocator != NULL);

  unordered_map->allocator  = allocator;
  unordered_map->kvps_sz_   = 0;
  unordered_map->kvps_load_ = 0;
  unordered_map->kvps_      = NULL;
}

template <typename _Key, typename _Value>
void bt_unordered_map_destroy(
  bt_unordered_map_t<_Key, _Value> *unordered_map)
{
  // bt_assert(debug, unordered_map != NULL);

  if (bt_likely(unordered_map->kvps_))
    bt_allocator_free(unordered_map->allocator, (void *)unordered_map->kvps_);

  unordered_map->allocator  = NULL;
  unordered_map->kvps_sz_   = 0;
  unordered_map->kvps_load_ = 0;
  unordered_map->kvps_      = NULL;
}

/* ========================================================================== */

template <typename _Key, typename _Value, typename _SuppliedKey>
_Value *bt_unordered_map_insert(
  bt_unordered_map_t<_Key, _Value> *unordered_map,
  _SuppliedKey key)
{
  // bt_assert(debug, unordered_map != NULL);
  if (bt_unlikely(bt_unordered_map__overloaded(unordered_map)))
    bt_unordered_map__grow(unordered_map);
  typedef bt_unordered_map_key_to_hash_<_SuppliedKey> KeyToHash;
  typedef typename KeyToHash::_ Hash;
  return bt_unordered_map__insert<_Key, _Value>(
    unordered_map->kvps_,
    unordered_map->kvps_sz_,
    KeyToHash::hash(key));
}

/* ========================================================================== */

template <typename _Key, typename _Value, typename _SuppliedKey>
_Value *bt_unordered_map_find(
  const bt_unordered_map_t<_Key, _Value> *unordered_map,
  _SuppliedKey key)
{
  // bt_assert(debug, unordered_map != NULL);

  if (bt_unlikely(unordered_map->kvps_sz_ == 0))
    return NULL;

  typedef bt_unordered_map_key_to_hash_<_SuppliedKey> KeyToHash;
  typedef typename KeyToHash::_ Hash;
  const Hash hash = KeyToHash::hash(key);
  const Hash empty = bt_unordered_map_hash_<Hash>::invalid();

  size_t index = (((size_t)hash) % unordered_map->kvps_sz_);
  while (true) {
    if (unordered_map->kvps_[index].hash == hash)
      return (_Value *)&unordered_map->kvps_[index].value;
    if (unordered_map->kvps_[index].hash == empty)
      return NULL;
    index = ((index + 1) % unordered_map->kvps_sz_);
  }
}

/* ========================================================================== */

template <typename _Key, typename _Value, typename _SuppliedKey>
_Value *bt_unordered_map_remove(
  bt_unordered_map_t<_Key, _Value> *unordered_map,
  _SuppliedKey key)
{
  // bt_assert(debug, unordered_map != NULL);

  /* TODO(mtwilliams): shrink if (load * 2) < 75. */
  // if (bt_unlikely(bt_unordered_map__underloaded(unordered_map)))
  //   bt_unordered_map__shirnk(unordered_map);

  typedef bt_unordered_map_key_to_hash_<_SuppliedKey> KeyToHash;
  typedef typename KeyToHash::_ Hash;
  const Hash hash = KeyToHash::hash(key);
  const Hash empty = bt_unordered_map_hash_<Hash>::invalid();

  size_t index = (((size_t)hash) % unordered_map->kvps_sz_);
  while (true) {
    if (unordered_map->kvps_[index].hash == hash) {
      unordered_map->kvps_[index].hash = empty;
      return (_Value *)&unordered_map->kvps_[index].value; }
    if (unordered_map->kvps_[index].hash == empty)
      return NULL;
    index = ((index + 1) % unordered_map->kvps_sz_);
  }
}
