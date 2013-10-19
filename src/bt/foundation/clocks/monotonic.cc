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
#include <bt/foundation/allocators.h>

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
  static uint64_t __counts_per_sec;
  static void __perf_freq_ctor_() {
    LARGE_INTEGER perf_freq;
    QueryPerformanceFrequency(&perf_freq);
    __counts_per_sec = perf_freq.QuadPart;
  }
  #if (BT_COMPILER == BT_COMPILER_MSVC)
    static void __cdecl __perf_freq_ctor()
    { __perf_freq_ctor_(); }
    #pragma section(".CRT$XCU", read)
    __declspec(allocate(".CRT$XCU")) void (__cdecl*__perf_freq_ctor__)() = &__perf_freq_ctor;
  #else
    static void __attribute__((constructor)) __perf_freq_ctor() {
      __perf_freq_ctor_();
    }
  #endif
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif

/* ========================================================================== */
/*  Monotonic Clock:                                                          */
/* ========================================================================== */

struct bt_monotonic_clock {
#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  uint64_t epoch;
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  uint64_t epoch;
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  LARGE_INTEGER epoch;
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif
};

/* ========================================================================== */

bt_monotonic_clock_t *bt_monotonic_clock_create() {
  bt_monotonic_clock_t *clock =
    (bt_monotonic_clock_t *)bt_allocator_alloc(
      bt_heap_allocator(),
      sizeof(bt_monotonic_clock_t),
      bt_alignof(bt_monotonic_clock_t));
  bt_monotonic_clock_reset(clock);
  return clock;
}

void bt_monotonic_clock_destroy(bt_monotonic_clock_t *clock) {
  // bt_assert(debug, clock != NULL);
  bt_allocator_free(bt_heap_allocator(), (void *)clock);
}

/* ========================================================================== */

void bt_monotonic_clock_reset(bt_monotonic_clock_t *clock) {
#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  clock->epoch = t.tv_sec * UINT64_C(1000000000) + t.tv_nsec;
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  clock->epoch = mach_absolute_time();
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  QueryPerformanceCounter(&clock->epoch);
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif
}

uint64_t bt_monotonic_clock_secs(const bt_monotonic_clock_t *clock) {
#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  return bt_monotonic_clock_nsecs(clock) / UINT64_C(1000000000);
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  return bt_monotonic_clock_nsecs(clock) / UINT64_C(1000000000);
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return ((now.QuadPart - clock->epoch.QuadPart) / __counts_per_sec);
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif
}

uint64_t bt_monotonic_clock_msecs(const bt_monotonic_clock_t *clock) {
#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  return bt_monotonic_clock_nsecs(clock) / UINT64_C(1000000);
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  return bt_monotonic_clock_nsecs(clock) / UINT64_C(1000000);
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return (((now.QuadPart - clock->epoch.QuadPart) * UINT64_C(1000)) / __counts_per_sec);
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif
}

uint64_t bt_monotonic_clock_usecs(const bt_monotonic_clock_t *clock) {
#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  return bt_monotonic_clock_nsecs(clock) / UINT64_C(1000);
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  return bt_monotonic_clock_nsecs(clock) / UINT64_C(1000);
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return (((now.QuadPart - clock->epoch.QuadPart) * UINT64_C(1000000)) / __counts_per_sec);
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif
}

uint64_t bt_monotonic_clock_nsecs(const bt_monotonic_clock_t *clock) {
#if ((BT_PLATFORM == BT_PLATFORM_LINUX) || (BT_PLATFORM == BT_PLATFORM_ANDROID))
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return ((t.tv_sec * UINT64_C(1000000000) + t.tv_nsec) - clock->epoch);
#elif ((BT_PLATFORM == BT_PLATFORM_MACOSX) || (BT_PLATFORM == BT_PLATFORM_IOS))
  return (((mach_absolute_time() - clock->epoch) * __mach_timebase_info.numer) / __mach_timebase_info.denom);
#elif (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  LARGE_INTEGER now;
  QueryPerformanceCounter(&now);
  return (((now.QuadPart - clock->epoch.QuadPart) * UINT64_C(1000000000)) / __counts_per_sec);
#else
  #error ("Unknown or unsupported compiler, architecture, and platform configuration!")
#endif
}
