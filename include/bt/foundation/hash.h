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
/*! @file bt/foundation/hash.h
      Provides various hashing and digesting functions.                       */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_HASH_H_
#define _BT_FOUNDATION_HASH_H_

#include <bt/foundation/compat.h>

/* ========================================================================== */
/*  * Hashes                                                                  */
/*    * MurmurHash2                                                           */
/*    * MurmurHash3                                                           */
/* ========================================================================== */

/* ========================================================================== */
/*  MurmurHash2:                                                              */
/* ========================================================================== */

/*! Hashes a buffer with MurmurHash2. */
extern uint32_t bt_murmur_hash(const void *buf, const size_t buf_sz, const uint32_t seed);

/*! Hashes a string with MurmurHash2. */
extern uint32_t bt_murmur_hash_str(const char *str, const uint32_t seed);

/* ========================================================================== */
/*  MurmurHash3:                                                              */
/* ========================================================================== */

/*! Mixes a signed-integer with MurmurHash3's finalizer. */
extern int32_t bt_murmur_hash_s32(const int32_t key);

/*! Mixes an unsigned-integer with MurmurHash3's finalizer. */
extern uint32_t bt_murmur_hash_u32(const uint32_t key);

#endif /* _BT_FOUNDATION_HASH_H_ */
