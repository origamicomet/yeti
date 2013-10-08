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
    #include <bt/foundation/unordered_map.h>
/* ========================================================================== */

// /* ========================================================================== */

// void bt_hash_tbl_create(bt_hash_tbl_t *hash_tbl, bt_allocator_t *allocator, const size_t entry_sz) {
//   // bt_assert(debug, hash_tbl != NULL);
//   // bt_assert(debug, allocator != NULL);
//   // bt_assert(debug, entry_sz > 0);

//   hash_tbl->allocator = allocator;
//   hash_tbl->size = 0;
//   hash_tbl->load = 0;
//   hash_tbl->_entries = NULL;
//   hash_tbl->_entry_sz = sizeof(bt_hash_t) + entry_sz;
// }

// void bt_hash_tbl_destroy(bt_hash_tbl_t *hash_tbl) {
//   // bt_assert(debug, hash_tbl != NULL);

//   bt_allocator_free(hash_tbl->allocator, hash_tbl->_entries);
//   hash_tbl->size = 0;
//   hash_tbl->load = 0;
//   hash_tbl->_entries = NULL;
//   hash_tbl->_entry_sz = 0;
// }

// /* ========================================================================== */

// static bt_hash_t bt_hash_tbl_key_at(bt_hash_tbl_t *hash_tbl, const size_t idx) {
//   // bt_assert(paranoid, hash_tbl != NULL);
//   // bt_assert(paranoid, idx < hash_tbl->size);
//   return *((const bt_hash_t *)(((uintptr_t)hash_tbl->_entries) + idx * hash_tbl->_entry_sz));
// }

// static void *bt_hash_tbl_value_at(bt_hash_tbl_t *hash_tbl, const size_t idx) {
//   // bt_assert(paranoid, hash_tbl != NULL);
//   // bt_assert(paranoid, idx < hash_tbl->size);
//   return (void *)(((uintptr_t)hash_tbl->_entries) + idx * hash_tbl->_entry_sz + sizeof(bt_hash_t));
// }

// static bool bt_hash_tbl_should_grow(bt_hash_tbl_t *hash_tbl) {
//   // bt_assert(paranoid, hash_tbl != NULL);
//   return (hash_tbl->size ? (((hash_tbl->load * 100) / hash_tbl->size) >= 75) : true);
// }

// static void bt_hash_tbl_grow(bt_hash_tbl_t *hash_tbl) {
//   // bt_assert(paranoid, hash_tbl != NULL);

//   const size_t target_sz = ((hash_tbl->size ? hash_tbl->size : 1) * 2);
//   void *entries = bt_allocator_alloc(
//     hash_tbl->allocator,
//     target_sz * hash_tbl->_entry_sz, 1);
//   memset(entries, 0, target_sz * hash_tbl->_entry_sz);

//   for (size_t idx = 0; idx < hash_tbl->size; ++idx) {
//     const bt_hash_t key = bt_hash_tbl_key_at(hash_tbl, idx);
//     if (bt_unlikely(key == BT_HASH_C(0)))
//       continue;
//     size_t probe_idx = key;
//     while (true) {
//       const size_t actual_idx = (probe_idx & target_sz);
//       const bt_hash_t key_at_idx =
//         *((const bt_hash_t *)(((uintptr_t)entries) + actual_idx * hash_tbl->_entry_sz));
//       if (key_at_idx == BT_HASH_C(0)) {
//         *((bt_hash_t *)(((uintptr_t)entries) + actual_idx * hash_tbl->_entry_sz)) = key;
//         memcpy(
//           (void *)(((uintptr_t)hash_tbl->_entries) + actual_idx * hash_tbl->_entry_sz + sizeof(bt_hash_t)),
//           bt_hash_tbl_value_at(hash_tbl, idx),
//           hash_tbl->_entry_sz);
//         break; }
//       ++probe_idx;
//     }
//   }

//   bt_allocator_free(hash_tbl->allocator, hash_tbl->_entries);
//   hash_tbl->_entries = entries;
//   hash_tbl->size = target_sz;
// }

// /* ========================================================================== */

// bool bt_hash_tbl_exists(bt_hash_tbl_t *hash_tbl, const bt_hash_t key) {
//   // bt_assert(debug, hash_tbl != NULL);
//   // bt_assert(paranoid, key != BT_HASH_C(0));
//   return (bt_hash_tbl_find(hash_tbl, key) != NULL);
// }

// bool bt_hash_tbl_exists_str(bt_hash_tbl_t *hash_tbl, const char *key) {
//   return bt_hash_tbl_exists(hash_tbl, bt_hash_from_str(key, 0));
// }

// void *bt_hash_tbl_find(bt_hash_tbl_t *hash_tbl, const bt_hash_t key) {
//   // bt_assert(debug, hash_tbl != NULL);
//   // bt_assert(paranoid, key != BT_HASH_C(0));

//   if (bt_unlikely(hash_tbl->size == 0))
//     return NULL;
//   size_t search_idx = key;
//   while (true) {
//     const size_t actual_idx = (search_idx % hash_tbl->size);
//     const bt_hash_t key_at_idx = bt_hash_tbl_key_at(hash_tbl, actual_idx);
//     if (key_at_idx == key)
//       return bt_hash_tbl_value_at(hash_tbl, actual_idx);
//     if (key_at_idx == BT_HASH_C(0))
//       return NULL;
//     ++search_idx;
//   }
// }

// void *bt_hash_tbl_find_str(bt_hash_tbl_t *hash_tbl, const char *key) {
//   return bt_hash_tbl_find(hash_tbl, bt_hash_from_str(key, 0));
// }

// void *bt_hash_tbl_insert(bt_hash_tbl_t *hash_tbl, const bt_hash_t key) {
//   // bt_assert(debug, hash_tbl != NULL);
//   // bt_assert(paranoid, key != BT_HASH_C(0));

//   if (bt_unlikely(bt_hash_tbl_should_grow(hash_tbl)))
//     bt_hash_tbl_grow(hash_tbl);
//   size_t search_idx = key;
//   while (true) {
//     const size_t actual_idx = (search_idx % hash_tbl->size);
//     const bt_hash_t key_at_idx = bt_hash_tbl_key_at(hash_tbl, actual_idx);
//     if (key_at_idx == key)
//       return NULL;
//     if (key_at_idx != BT_HASH_C(0)) {
//       ++search_idx;
//       continue; }
//     *((bt_hash_t *)(((uintptr_t)hash_tbl->_entries) + actual_idx * hash_tbl->_entry_sz)) = key;
//     return (void *)(((uintptr_t)hash_tbl->_entries) + actual_idx * hash_tbl->_entry_sz + sizeof(bt_hash_t));
//   }
// }

// void *bt_hash_tbl_insert_str(bt_hash_tbl_t *hash_tbl, const char *key) {
//   return bt_hash_tbl_insert(hash_tbl, bt_hash_from_str(key, 0));
// }

// void *bt_hash_tbl_remove(bt_hash_tbl_t *hash_tbl, const bt_hash_t key) {
//   // bt_assert(debug, hash_tbl != NULL);
//   // bt_assert(paranoid, key != BT_HASH_C(0));

//   if (bt_unlikely(hash_tbl->size == 0))
//     return NULL;
//   size_t search_idx = key;
//   while (true) {
//     const size_t actual_idx = (search_idx % hash_tbl->size);
//     const bt_hash_t key_at_idx = bt_hash_tbl_key_at(hash_tbl, actual_idx);
//     if (key_at_idx == key) {
//       *((bt_hash_t *)(((uintptr_t)hash_tbl->_entries) + actual_idx * hash_tbl->_entry_sz)) = BT_HASH_C(0);
//       return bt_hash_tbl_value_at(hash_tbl, actual_idx); }
//     if (key_at_idx == BT_HASH_C(0))
//       return NULL;
//     ++search_idx;
//   }
// }

// void *bt_hash_tbl_remove_str(bt_hash_tbl_t *hash_tbl, const char *key) {
//   return bt_hash_tbl_remove(hash_tbl, bt_hash_from_str(key, 0));
// }

// /* ========================================================================== */

// void bt_hash_tbl_for_each(
//   bt_hash_tbl_t *hash_tbl,
//   bool (*callback)(void *closure, const bt_hash_t key, void *value),
//   void *closure)
// {
//   // bt_assert(debug, hash_tbl != NULL);
//   // bt_assert(debug, callback != NULL);

//   for (size_t idx = 0; idx < hash_tbl->size; ++idx) {
//     const bt_hash_t key = bt_hash_tbl_key_at(hash_tbl, idx);
//     if (key == BT_HASH_C(0))
//       continue;
//     if (!callback(closure, key, bt_hash_tbl_value_at(hash_tbl, idx)))
//       break;
//   }
// }
