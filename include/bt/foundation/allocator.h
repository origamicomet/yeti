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
/*! @file butane/foundation/allocator.h
      Defines a generic allocator interface.                                  */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_ALLOCATOR_H_
#define _BT_FOUNDATION_ALLOCATOR_H_

#include <bt/foundation/compat.h>

/* ========================================================================== */
/*  Allocator (bt_allocator_t):                                               */
/* ========================================================================== */

/*! A generic allocator interface. */
typedef struct bt_allocator {
  void *(*alloc)(
    struct bt_allocator *allocator,
    const size_t num_of_bytes,
    const size_t alignment);

  void *(*realloc)(
    struct bt_allocator *allocator,
    void *ptr,
    const size_t num_of_bytes,
    const size_t alignment);

  void (*free)(
    struct bt_allocator *allocator,
    void *ptr);
} bt_allocator_t;

/* ========================================================================== */

/*! Allocates `num_of_bytes` of memory aligned to `alignment`.
  @returns A non-null pointer on success, or null on failure. */
extern void *bt_allocator_alloc(
  bt_allocator_t *allocator,
  const size_t num_of_bytes,
  const size_t alignment);

/*! Reallocates `ptr` to `num_of_bytes` of memory aligned to `alignment`.
  @returns A non-null pointer on success, or null of failure. */
extern void *bt_allocator_realloc(
  bt_allocator_t *allocator,
  void *ptr,
  const size_t num_of_bytes,
  const size_t alignment);

/*! Deallocates the memory pointed to by `ptr`. */
extern void bt_allocator_free(
  bt_allocator_t *allocator,
  void *ptr);

#endif /* _BT_FOUNDATION_ALLOCATOR_H_ */
