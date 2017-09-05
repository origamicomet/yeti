//===-- yeti/foundation/hash_map.h ----------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_FOUNDATION_HASH_MAP_H_
#define _YETI_FOUNDATION_HASH_MAP_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

// OPTIMIZE(mtwilliams): Steal a few things from Dale's implementation.
 // https://github.com/graphitemaster/neothyne/blob/master/u_hash.cpp
// TODO(mtwilliams): Move to type-safe `yeti::foundation::Buffer`.
// TODO(mtwilliams): Call destructors for non-POD?
// TODO(mtwilliams): Document `HashMap<K,V>`.

namespace hash_map {
  namespace Types {
    typedef uintptr_t Hash;
    enum {Bits = sizeof(uintptr_t) * 8};
  }

  template <typename K, size_t Bits>
  struct Hasher {};

  template <typename K>
  struct Hasher<K, 32> {
    static u32 hash(const K &key) {
      return fnv1a_hash_32((const void *)&key, sizeof(K)); }};

  template <typename K>
  struct Hasher<K, 64> {
    static u64 hash(const K &key) {
      return fnv1a_hash_64((const void *)&key, sizeof(K)); }};

  template <>
  struct Hasher<const char *, 32> {
    static u32 hash(const char *key) {
      return murmur_hash_32(key); }};

  template <>
  struct Hasher<const char *, 64> {
    static u64 hash(const char *key) {
      return murmur_hash_64(key); }};

  template <typename K>
  static Types::Hash hash(const K &key) {
    return Hasher<K, Types::Bits>::hash(key); }
}

template <typename K, typename V>
class HashMap {
 // Copying a hash-map does not make a lot of sense. If you find yourself
 // needing to copy a hash-map, you probably want to dynamically resize it. If
 // that's the case, see the comment in `HashMap<K,V>::insert(...)`.
 YETI_DISALLOW_COPYING(HashMap)

 private:
  typedef hash_map::Types::Hash Hash;
  struct Entry {
    Hash hash_of_key;
    V value;
  };

 public:
  explicit HashMap(Allocator *allocator, const size_t sz);
  explicit HashMap(Allocator &allocator, const size_t sz);
  ~HashMap();

 public:
  V *find(const K &key);
  void insert(const K &key, const V &value);
  V *reserve(const K &key);
  void remove(const K &key);

 private:
  static Hash hash(const K &key);

 private:
  Allocator *allocator_;
  Entry *slots_;
  size_t num_slots_;
  size_t occupants_;
};

template <typename K, typename V>
HashMap<K,V>::HashMap(Allocator *allocator, const size_t sz) {
  yeti_assert_debug(allocator != NULL);
  allocator_ = allocator;
  // TODO(mtwilliams): Respect minimum alignment requirements of |Entry|.
  slots_ = (Entry *)allocator->allocate(sz * sizeof(Entry));
  memset((void *)slots_, 0, sz * sizeof(Entry));
  num_slots_ = sz;
  occupants_ = 0;
}

template <typename K, typename V>
HashMap<K,V>::HashMap(Allocator &allocator, const size_t sz) {
  allocator_ = &allocator;
  // TODO(mtwilliams): Respect minimum alignment requirements of |Entry|.
  slots_ = (Entry *)allocator.allocate(sz * sizeof(Entry));
  memset((void *)slots_, 0, sz * sizeof(Entry));
  num_slots_ = sz;
  occupants_ = 0;
}

template <typename K, typename V>
HashMap<K,V>::~HashMap() {
  allocator_->deallocate((uintptr_t)slots_);
}

template <typename K, typename V>
V *HashMap<K,V>::find(const K &key) {
  const Hash hash_of_key = hash(key);
  for (size_t slot = hash_of_key % num_slots_;
       slots_[slot].hash_of_key != 0;
       slot = (++slot % num_slots_))
  {
    if (slots_[slot].hash_of_key == hash_of_key)
      return &slots_[slot].value;
  }

  return NULL;
}

template <typename K, typename V>
void HashMap<K,V>::insert(const K &key, const V &value) {
  *this->reserve(key) = value;
}

template <typename K, typename V>
V *HashMap<K,V>::reserve(const K &key) {
  // We don't support dynamically resizing hash-maps, yet. Generally, a
  // decent load-factor is ~75%, so if we cross that threshold we should
  // manually bump up the size of the hash-map.
  yeti_assert_development(((occupants_ * 100) / (num_slots_ * 100)) <= 75);

  const Hash hash_of_key = hash(key);

  size_t slot = hash_of_key % num_slots_;
  for (; slots_[slot].hash_of_key != 0; slot = (++slot % num_slots_))
    yeti_assert_debug(slots_[slot].hash_of_key != hash_of_key);

  Entry *entry = &slots_[slot];
  entry->hash_of_key = hash_of_key;
  memset((void *)&entry->value, 0, sizeof(V));

  occupants_ += 1;

  return &entry->value;
}

template <typename K, typename V>
void HashMap<K,V>::remove(const K &key) {
  const Hash hash_of_key = hash(key);
  for (size_t slot = hash_of_key % num_slots_;
       slots_[slot].hash_of_key != 0;
       slot = (++slot % num_slots_))
  {
    if (slots_[slot].hash_of_key == hash_of_key) {
      slots_[slot].hash_of_key = 0;
      break;
    }
  }
}

template <typename K, typename V>
typename HashMap<K,V>::Hash HashMap<K,V>::hash(const K &key) {
  const Hash hash_of_key = hash_map::hash<K>(key);

  // We set `Entry::hash_of_key` to zero to indicate an unused slot. Of course,
  // if the actual hash of |key| is zero, then we'll be confused.
  yeti_assert_development(hash_of_key != 0);

  return hash_of_key;
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_HASH_MAP_H_
