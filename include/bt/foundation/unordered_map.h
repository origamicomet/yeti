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
/*! @file bt/foundation/unordered_map.h
      Provides a typed unordered key-to-value mapping data structure.         */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_UNORDERED_MAP_H_
#define _BT_FOUNDATION_UNORDERED_MAP_H_

#include <bt/foundation/compat.h>
#include <bt/foundation/allocator.h>
#include <bt/foundation/hash.h>

/* ========================================================================== */
/*  Unordered Map:                                                            */
/* ========================================================================== */

/*! Allows mappings from `_Key` to a hash type and hashing function. This is
  required for accepting differing supplied key types, to allow operations when
  the originally supplied key type cannot be used.

  @para For example: Strings are mapped to unsigned-integers with
  the bt_murmur_hash_str() function. This allows operations to be performed
  with just the hash! */
template <typename _Key>
struct bt_unordered_map_key_to_hash_ {
  typedef _Key _;
  static _ hash(const _Key key) {
    return key;
  }
};

template <>
struct bt_unordered_map_key_to_hash_<const char *> {
  typedef uint32_t _;
  static _ hash(const char *key) {
    return bt_murmur_hash_str(key, 0);
  }
};

/*! Provides special values for a `_Hash` type hash, like invalid values for
  denoting empty key-value pairs. */
template <typename _Hash>
struct bt_unordered_map_hash_ {};

template <>
struct bt_unordered_map_hash_<uint32_t> {
  static uint32_t invalid() {
    return UINT32_C(0);
  }
};

/*! A typed unordered key-to-value mapping data structure. */
template <typename _Key, typename _Value>
struct bt_unordered_map_t {
  /* ======================================================================== */
  typedef _Key Key; typedef _Value Value;
  typedef struct _KeyValuePair {
    typedef _Key Key; typedef _Value Value;
    typedef typename bt_unordered_map_key_to_hash_<_Key>::_ _Hash;
    _Hash hash;
    _Value value;
  } _KeyValuePair;
  /* ======================================================================== */
  bt_allocator_t *allocator;
  size_t kvps_sz_;
  size_t kvps_load_;
  _KeyValuePair *kvps_;
};

/* ========================================================================== */

/*! Initializes `unordered_map` to use `allocator`. */
template <typename _Key, typename _Value>
static void bt_unordered_map_init(
  bt_unordered_map_t<_Key, _Value> *unordered_map,
  bt_allocator_t *allocator);

/*! Releases any resources associated with `unordered_map`, and resets all fields. */
template <typename _Key, typename _Value>
static void bt_unordered_map_destroy(
  bt_unordered_map_t<_Key, _Value> *unordered_map);

/* ========================================================================== */

/*! Associates a value with `key` in near O(1) time.
  @returns A pointer to the value that is associated with `key` if a value is
           not already associated with `key`, or NULL. */
template <typename _Key, typename _Value, typename _SuppliedKey>
static _Value *bt_unordered_map_insert(
  bt_unordered_map_t<_Key, _Value> *unordered_map,
  _SuppliedKey key);

/*! Finds the associated value for `key` in near O(1) time.
  @returns A pointer to the value associated with `key` if existent, or NULL. */
template <typename _Key, typename _Value, typename _SuppliedKey>
static _Value *bt_unordered_map_find(
  const bt_unordered_map_t<_Key, _Value> *unordered_map,
  _SuppliedKey key);

/*! Deassociates a value from `key` in near O(1) time.
  @returns A pointer to the value that is now deassociated from `key` if a
  value was associated with `key`, or NULL. */
template <typename _Key, typename _Value, typename _SuppliedKey>
static _Value *bt_unordered_map_remove(
  bt_unordered_map_t<_Key, _Value> *unordered_map,
  _SuppliedKey key);

/* ========================================================================== */

#include <bt/foundation/unordered_map.inl>

#endif /* _BT_FOUNDATION_UNORDERED_MAP_H_ */
