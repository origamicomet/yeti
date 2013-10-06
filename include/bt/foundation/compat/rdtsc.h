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
/*! @file bt/foundation/compat/rdtsc.h
      Provides a method for reading the Time Stamp Counter.                   */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_COMPAT_RDTSC_H_
#define _BT_FOUNDATION_COMPAT_RDTSC_H_

#include <bt/foundation/detect/compiler.h>
#include <bt/foundation/compat/stdint.h>

/* ========================================================================== */

/*! Returns the number of cycles since reset. */
static inline uint64_t bt_rdtsc() {
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  uint32_t high, low;
  asm volatile(
    "rdtsc\n movl %%edx, %0\nmovl %%eax, %1\ncpuid"
    : "=r" (high), "=r" (low) : : "%rax", "%rbx", "%rcx", "%rdx");
  return ((((uint64_t)high) << 32ull) | (uint64_t)low);
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #error ("Not yet implemented!")
#else
  #error ("Time Stamp Counter is unsupported!")
#endif
}

/* ========================================================================== */

extern uint64_t bt_cycles_to_sec(const uint64_t cycles);
extern uint64_t bt_cycles_to_msec(const uint64_t cycles);
extern uint64_t bt_cycles_to_usec(const uint64_t cycles);

#endif /* _BT_FOUNDATION_COMPAT_RDTSC_H_ */
