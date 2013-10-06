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
/*! @file bt/foundation/timestamp.h
      Provides a timestamp based on libcork's gamma-sec.                    */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_TIMESTAMP_H_
#define _BT_FOUNDATION_TIMESTAMP_H_

#include <bt/foundation/compat/stdint.h>
#include <bt/foundation/compat/inttypes.h>

/*! A high-precision timestamp based on libcork's gamma-sec.
  "A timestamp is represented by a 64-bit integer, whose unit is the gamma-sec
   where one gamma-sec is equal to 1/2^32 seconds. With this representation,
   the upper 32 bits of a timestamp value represent the timestamp in seconds." */
typedef uint64_t bt_timestamp_t;

/* ========================================================================== */

extern bt_timestamp_t bt_timestamp_from_sec(uint64_t sec);
extern bt_timestamp_t bt_timestamp_from_gsec(uint64_t sec, uint64_t gsec);
extern bt_timestamp_t bt_timestamp_from_msec(uint64_t sec, uint64_t msec);
extern bt_timestamp_t bt_timestamp_from_usec(uint64_t sec, uint64_t usec);
extern bt_timestamp_t bt_timestamp_from_nsec(uint64_t sec, uint64_t nsec);

/* ========================================================================== */

extern uint32_t bt_timestamp_sec(const bt_timestamp_t ts);
extern uint32_t bt_timestamp_gsec(const bt_timestamp_t ts);
extern uint64_t bt_timestamp_msec(const bt_timestamp_t ts);
extern uint64_t bt_timestamp_usec(const bt_timestamp_t ts);
extern uint64_t bt_timestamp_nsec(const bt_timestamp_t ts);

#endif /* _BT_FOUNDATION_TIMESTAMP_H_ */
