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
    #include <bt/foundation/clocks/monotonic.h>
/* ========================================================================== */

#include <bt/foundation/detect/compiler.h>
#include <bt/foundation/detect/architecture.h>
#include <bt/foundation/detect/platform.h>

/* Bless me, Father, for I have sinned. Again. */

#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  #include <unistd.h>
  #include <time.h>
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  #include <unistd.h>
  #include <mach/mach_time.h>
  static mach_timebase_info_data_t __mach_timebase_info;
  static void __attribute__((constructor)) __mach_timebase_info_ctor()
  { mach_timebase_info(&__mach_timebase_info); }
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  #include <windows.h>
  #if (BT_COMPILER == BT_COMPILER_MSVC)
    static LARGE_INTEGER __perf_freq;
    static void __cdecl __perf_freq_ctor()
    { QueryPerformanceFrequency(&__perf_freq); }
    __declspec(allocate(".CRT$XCU")) void (__cdecl*__perf_freq_ctor_)() = &__perf_freq_ctor;
  #else
    static LARGE_INTEGER __perf_freq;
    static void __attribute__((constructor)) __perf_freq_ctor()
    { QueryPerformanceFrequency(&__perf_freq); }
  #endif
#elif ((BT_ARCHITECTURE == BT_ARCHITECTURE_X86) || (BT_ARCHITECTURE == BT_ARCHITECTURE_X86_64))
  #include <unistd.h>
  static inline uint64_t rdtsc() {
    uint32_t high, low;
    asm volatile(
      "rdtsc\n movl%%edx, %0\nmovl %%eax, %1\ncpuid"
      : "=r" (high), "=r" (low) : : "%rax", "%rbx", "%rcx", "%rdx");
    return ((((uint64_t)high) << 32ull) | (uint64_t)low); }
  static uint64_t __rdtsc_per_sec = 0;
  static void __attribute__((constructor)) __rdtsc_per_sec_ctor() {
    const uint64_t before = rdtsc();
    usleep(1000000u);
    const uint64_t after = rdtsc();
    __rdtsc_per_sec = after - before; }
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif

bt_timestamp_t bt_monotonic_now() {
#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return bt_timestamp_from_nsec(t.tv_sec, t.tv_nsec);
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  return bt_timestamp_from_nsec(
    (mach_absolute_time() * mach_absolute_time().numer) /
    (__mach_timebase_info.denom * 1000000000ull),
    ((mach_absolute_time() * mach_absolute_time().numer) /
    __mach_timebase_info.denom) % 1000000000ull);
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  LARGE_INTEGER perf_counter;
  QueryPerformanceCounter(&perf_counter);
  return bt_timestamp_from_nsec(
    (perf_counter.QuadPart / __perf_freq.QuadPart),
    (perf_counter.QuadPart / (__perf_freq.QuadPart / 1000000000ull)) % 1000000000ull);
#elif ((BT_ARCHITECTURE == BT_ARCHITECTURE_X86) || (BT_ARCHITECTURE == BT_ARCHITECTURE_X86_64))
  const uint64_t tsc = rdtsc();
  return bt_timestamp_from_nsec(
    (tsc / __rdtsc_per_sec),
    (tsc / (__rdtsc_per_sec / 1000000000ull)) % 1000000000ull);
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif
}
