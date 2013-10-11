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
    #include <bt/foundation/allocators/heap.h>
/* ========================================================================== */

#include <bt/foundation/memory.h>

#include <malloc.h>

typedef struct alloc_header {
  size_t num_of_bytes;
  void *unaligned;
} alloc_header_t;

static alloc_header_t *header_from_ptr(void *ptr) {
  return &((alloc_header_t *)ptr)[-1];
}

#include <stdio.h>

static void *bt_heap_allocator_alloc(
  bt_allocator_t *allocator,
  const size_t num_of_bytes,
  const size_t alignment)
{
  (void)allocator;
  uintptr_t ptr =
    (uintptr_t)malloc(num_of_bytes + sizeof(alloc_header_t) + alignment - 1);
  uintptr_t aligned = ((ptr + sizeof(alloc_header_t)) & ~(alignment - 1));
  header_from_ptr((void *)aligned)->num_of_bytes = num_of_bytes;
  header_from_ptr((void *)aligned)->unaligned = (void *)ptr;
  return ((void *)aligned);
}

static void *bt_heap_allocator_realloc(
  bt_allocator_t *allocator,
  void *ptr,
  const size_t num_of_bytes,
  const size_t alignment)
{
  void *ptr_ = allocator->alloc(allocator, num_of_bytes, alignment);
  if (ptr) {
    bt_copy(ptr, ptr_, header_from_ptr(ptr)->num_of_bytes);
    allocator->free(allocator, ptr); }
  return ptr_;
}

static void bt_heap_allocator_free(
  bt_allocator_t *allocator,
  void *ptr)
{
  (void)allocator;
  if (ptr)
    free(header_from_ptr(ptr)->unaligned);
}

static bt_allocator_t _heap_allocator = {
  &bt_heap_allocator_alloc,
  &bt_heap_allocator_realloc,
  &bt_heap_allocator_free,
};

bt_allocator_t *bt_heap_allocator() {
  return &_heap_allocator;
}
