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
    #include <bt/foundation/timestamp.h>
/* ========================================================================== */

bt_timestamp_t bt_timestamp_from_sec(uint64_t sec) {
  return ((bt_timestamp_t)(((uint64_t)(sec)) << UINT64_C(32)));
}

bt_timestamp_t bt_timestamp_from_gsec(uint64_t sec, uint64_t gsec) {
  return ((bt_timestamp_t)((((uint64_t)(sec)) << UINT64_C(32)) | (((uint64_t)(gsec)) & UINT64_C(0xffffffff))));
}

bt_timestamp_t bt_timestamp_from_msec(uint64_t sec, uint64_t msec) {
  return ((bt_timestamp_t)((((uint64_t)(sec)) << UINT64_C(32)) | ((((uint64_t)(msec)) << UINT64_C(32)) / UINT64_C(1000))));
}

bt_timestamp_t bt_timestamp_from_usec(uint64_t sec, uint64_t usec) {
  return ((bt_timestamp_t)((((uint64_t)(sec)) << UINT64_C(32)) | ((((uint64_t)(usec)) << UINT64_C(32)) / UINT64_C(1000000))));
}

bt_timestamp_t bt_timestamp_from_nsec(uint64_t sec, uint64_t nsec) {
  return ((bt_timestamp_t)((((uint64_t)(sec)) << UINT64_C(32)) | ((((uint64_t)(nsec)) << UINT64_C(32)) / UINT64_C(1000000000))));
}

/* ========================================================================== */

uint32_t bt_timestamp_sec(const bt_timestamp_t ts) {
  return ((uint32_t)(ts >> UINT64_C(32)));
}

uint32_t bt_timestamp_gsec(const bt_timestamp_t ts) {
  return ((uint32_t)(ts & UINT64_C(0xffffffff)));
}

static uint64_t bt_timestamp_gsec_to_units(const bt_timestamp_t ts, const uint64_t denom) {
  return ((uint64_t)((ts & UINT64_C(0xffffffff)) * (denom >> UINT64_C(32))));
}

uint64_t bt_timestamp_msec(const bt_timestamp_t ts) {
  return bt_timestamp_gsec_to_units(ts, UINT64_C(1000));
}

uint64_t bt_timestamp_usec(const bt_timestamp_t ts) {
  return bt_timestamp_gsec_to_units(ts, UINT64_C(1000000));
}

uint64_t bt_timestamp_nsec(const bt_timestamp_t ts) {
  return bt_timestamp_gsec_to_units(ts, UINT64_C(1000000000));
}
