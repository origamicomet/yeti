//===-- yeti/core/platform/timer.cc ---------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/platform/timer.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  #include <unistd.h>
  #include <mach/mach_time.h>
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  #include <unistd.h>
  #include <time.h>
#endif

namespace yeti {
namespace core {

namespace {
  // TODO(mtwilliams): Prevent overflow.
  // See https://stackoverflow.com/questions/23378063.
  static u64 ticks_to_time(u64 ticks, u64 frequency, u64 factor) {
  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    return (ticks * factor) / frequency;
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC
    const u64 numerator = ((mach_timebase_info_t)&factor)->numer;
    const u64 denominator = ((mach_timebase_info_t)&factor)->denom;
    return (numerator * ticks * factor) / denominator;
  #elif YETI_PLATFORM == YETI_PLATFORM_LINUX
    return (ticks * factor) / frequency;
  #endif
  }

  static u64 ticks() {
  #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
    LARGE_INTEGER now;
    ::QueryPerformanceCounter(&now);
    return now.QuadPart;
  #elif YETI_PLATFORM == YETI_PLATFORM_MAC
    return ::mach_absolute_time();
  #elif YETI_PLATFORM == YETI_PLATFORM_LINUX
    struct timespec ts;
    ::clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000000000ull + ts.tv_nsec);
  #endif
  }
}

Timer::Timer() {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::QueryPerformanceFrequency((LARGE_INTEGER *)&frequency_);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC
  // HACK(mtwilliams): We can pack Mach's timebase into 64-bit integer as it
  // is composed of two 32-bit integers. One for the numerator and the other
  // for the denominator.
  ::mach_timebase_info((mach_timebase_info_t)&frequency_);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  frequency_ = 1000000000ull;
#endif

  reset();
}

Timer::~Timer() {
}

void Timer::reset() {
  epoch_ = ticks();
}

u64 Timer::secs() const {
  return ticks_to_time(elapsed(), frequency_, 1ull);
}

u64 Timer::msecs() const {
  return ticks_to_time(elapsed(), frequency_, 1000ull);
}

u64 Timer::usecs() const {
  return ticks_to_time(elapsed(), frequency_, 1000000ull);
}

u64 Timer::nsecs() const {
  return ticks_to_time(elapsed(), frequency_, 1000000000ull);
}

u64 Timer::elapsed() const {
  return (ticks() - epoch_);
}

} // core
} // yeti
