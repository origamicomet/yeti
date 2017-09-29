//===-- yeti/core/containers/map.h ----------------------*- mode: C++11 -*-===//
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
/// \brief Dynamically resized hash maps.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_CONTAINERS_MAP_H_
#define _YETI_CORE_CONTAINERS_MAP_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

#include "yeti/core/memory.h"
#include "yeti/core/allocator.h"

// It is a hash map, after all.
#include "yeti/core/algorithms/hash.h"

// For sanity checks in debug builds.
#include "yeti/core/debug/assert.h"

namespace yeti {
namespace core {

namespace map {

#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  typedef u32 Hash;
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  typedef u64 Hash;
#endif

enum {
  /// \internal Size of hash in bits.
  Bits = (sizeof(Hash) * 8)
};

/// \internal Declares signature of hash function.
template <typename K>
struct HashFunctionSignature {
  typedef map::Hash (*Type)(const K &key);
};

// TODO(mtwilliams): Specialize for pointers.
template <> struct HashFunctionSignature<const char *> {
  typedef map::Hash (*Type)(const char *key);
};

/// \brief Default hash function.
///
/// \details We default to FNV1A. This is good enough, because keys are
/// usually integers or pointers if they're not strings. If they are strings
/// we use Murmur instead.
///
template <typename K, size_t Bits = ::yeti::core::map::Bits>
struct DefaultHashFunction {};

template <typename K> struct DefaultHashFunction<K, 32> {
  static u32 hash(const K &key) {
    return fnv1a_hash_32((const void *)&key, sizeof(K)); }
};

template <typename K> struct DefaultHashFunction<K, 64> {
  static u64 hash(const K &key) {
    return fnv1a_hash_64((const void *)&key, sizeof(K));
  }
};

template <> struct DefaultHashFunction<const char *, 32> {
  static u32 hash(const char *key) {
    return murmur_hash_32(key);
  }
};

template <> struct DefaultHashFunction<const char *, 64> {
  static u64 hash(const char *key) {
    return murmur_hash_64(key);
  }
};

/// \brief Identity hash function.
///
/// \details Simply returns key unadulterated.
///
template <typename K, size_t Bits = ::yeti::core::map::Bits>
struct IdentityHashFunction {};

template <> struct IdentityHashFunction<u32, 32> {
  static u32 hash(const u32 key) { return key; }
};

template <> struct IdentityHashFunction<u64, 64> {
  static u64 hash(const u64 key) { return key; }
};

}

/// \brief A dynamically resized hash map.
template <typename K, typename V,
          typename map::HashFunctionSignature<K>::Type F =
            map::DefaultHashFunction<typename K>::hash>
class Map {
 // Copying a map does not make a lot of sense. If you find yourself needing to
 // copy a map, rethink what you're trying to do.
 YETI_DISALLOW_COPYING(Map)

 private:
  typedef map::Hash Hash;

  struct Entry {
    Hash hash_of_key;
    V    value;
  };

 public:
  Map();

  explicit Map(Allocator *allocator, size_t size = 0);
  explicit Map(Allocator &allocator, size_t size = 0);

  ~Map();

 public:
  /// Returns a pointer to the value associated with @key.
  /// @{
  V *find(const K &key);
  const V *find(const K &key) const;
  /// @}

  /// Associates @key with storage for a value.
  V &emplace(const K &key);

  /// Associates @key with @value.
  void insert(const K &key, const V &value);

  /// Deassociates a @key from its value, if it has one.
  void remove(const K &key);

  /// Deassociates all keys from all values.
  void clear();

  /// Determines if the map doesn't contain any associations.
  bool empty() const;

 private:
  Allocator *allocator_;

  Entry *entries_;

  // Total number of entries.
  size_t size_;

  // Number of occupied entries.
  size_t occupied_;
};

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
Map<K,V,F>::Map() {
  allocator_ = NULL;
  entries_   = NULL;
  size_      = 0;
  occupied_  = 0;
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
Map<K,V,F>::Map(Allocator *allocator, size_t size) {
  allocator_ = allocator;

  entries_   = (Entry *)allocator_->allocate(size * sizeof(Entry), alignof(Entry));
  size_      = size;
  occupied_  = 0;

  memory::zero((void *)entries_, size * sizeof(Entry));
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
Map<K,V,F>::Map(Allocator &allocator, size_t size) {
  allocator_ = &allocator;

  entries_   = (Entry *)allocator_->allocate(size * sizeof(Entry), alignof(Entry));
  size_      = size;
  occupied_  = 0;

  memory::zero((void *)entries_, size * sizeof(Entry));
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
Map<K,V,F>::~Map() {
  if (allocator_)
    allocator_->deallocate((void *)entries_);
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
V *Map<K,V,F>::find(const K &key) {
  const Hash hash_of_key = F(key);

  for (size_t entry = hash_of_key % size_;
       entries_[entry].hash_of_key != 0;
       entry = (entry + 1) % size_)
    if (entries_[entry].hash_of_key == hash_of_key)
      return &entries_[entry].value;

  // Not found.
  return NULL;
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
const V *Map<K,V,F>::find(const K &key) const {
  const Hash hash_of_key = F(key);

  for (size_t entry = hash_of_key % size_;
       entries_[entry].hash_of_key != 0;
       entry = (entry + 1) % size_) {
    if (entries_[entry].hash_of_key == hash_of_key)
      return &entries_[entry].value;
  }

  // Not found.
  return NULL;
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
V &Map<K,V,F>::emplace(const K &key) {
  // TODO(mtwilliams): Grow or shrink hash map when load is ~75%.
  yeti_assert_development(((occupied_ * 100) / (size_ * 100)) <= 75);

  const Hash hash_of_key = F(key);

  size_t entry;
  for (entry = hash_of_key % size_;
       entries_[entry].hash_of_key != 0;
       entry = (entry + 1) % size_) {
    // Prevent double insertion.
    yeti_assert_debug(entries_[entry].hash_of_key != hash_of_key);
  }

  occupied_ += 1;

  entries_[entry].hash_of_key = hash_of_key;
  return entries_[entry].value;
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
void Map<K,V,F>::insert(const K &key, const V &value) {
  this->emplace(key) = value;
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
void Map<K,V,F>::remove(const K &key) {
  const Hash hash_of_key = F(key);

  for (size_t entry = hash_of_key % size_;
       entries_[entry].hash_of_key != 0;
       entry = (entry + 1) % size_) {
    if (entries_[entry].hash_of_key == hash_of_key) {
      memory::zero((void *)&entries_[entry], sizeof(Entry));
      return;
    }
  }
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
void Map<K,V,F>::clear() {
  memory::zero((void *)entries_, size_ * sizeof(Entry));
  occupied_ = 0;
}

template <typename K, typename V, typename map::HashFunctionSignature<K>::Type F>
bool Map<K,V,F>::empty() const {
  return (occupied_ == 0);
}

} // core
} // yeti

#endif // _YETI_CORE_CONTAINERS_MAP_H_
