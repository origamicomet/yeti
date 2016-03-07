//===-- yeti/foundation/high_res_timer.cc -----------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/high_res_timer.h"

#include "yeti/foundation/global_heap_allocator.h"

#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  #include <windows.h>
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  #include <unistd.h>
  #include <mach/mach_time.h>
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  #include <unistd.h>
  #include <time.h>
#endif

namespace yeti {
namespace foundation {

struct HighResolutionTimer::Implementation {
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  LARGE_INTEGER freq;
  LARGE_INTEGER epoch;
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  mach_timebase_info_data_t tbi;
  u64 epoch;
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  u64 epoch;
#endif
};

HighResolutionTimer *HighResolutionTimer::create() {
  Implementation *impl = new (foundation::heap()) Implementation();
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::QueryPerformanceFrequency(&impl->freq);
  ::QueryPerformanceCounter(&impl->epoch);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  mach_timebase_info(&impl->tbi);
  impl->epoch = ::mach_absolute_time();
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  struct timespec ts;
  ::clock_gettime(CLOCK_MONOTONIC, &ts);
  impl->epoch = ts.tv_sec * 1000000000ull + ts.tv_nsec;
#endif
  return (HighResolutionTimer *)impl;
}

void HighResolutionTimer::destroy() {
  Implementation *impl = (Implementation *)this;
  delete impl;
}

void HighResolutionTimer::reset() {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  ::QueryPerformanceCounter(&impl->epoch);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  impl->epoch = ::mach_absolute_time();
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  struct timespec ts;
  ::clock_gettime(CLOCK_MONOTONIC, &ts);
  impl->epoch = ts.tv_sec * 1000000000ull + ts.tv_nsec;
#endif
}

u64 HighResolutionTimer::secs() const {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  LARGE_INTEGER elapsed;
  elapsed.QuadPart = (now.QuadPart - impl->epoch.QuadPart);
  return (elapsed.QuadPart / impl->freq.QuadPart);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  const u64 elapsed = ::mach_absolute_time() - impl->epoch;
  return ((elapsed * impl->tbi.numer * 1000000000ull) / impl->tbi.denom);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  struct timespec ts;
  ::clock_gettime(CLOCK_MONOTONIC, &ts);
  const u64 elapsed = (ts.tv_sec * 1000000000ull + ts.tv_nsec) - impl->epoch;
  return (elapsed / 1000000000ull);
#endif
}

u64 HighResolutionTimer::msecs() const {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  LARGE_INTEGER elapsed;
  elapsed.QuadPart = (now.QuadPart - impl->epoch.QuadPart);
  return ((elapsed.QuadPart * 1000ull) / impl->freq.QuadPart);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  const u64 elapsed = ::mach_absolute_time() - impl->epoch;
  return ((elapsed * impl->tbi.numer * 1000000ull) / impl->tbi.denom);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  struct timespec ts;
  ::clock_gettime(CLOCK_MONOTONIC, &ts);
  const u64 elapsed = (ts.tv_sec * 1000000000ull + ts.tv_nsec) - impl->epoch;
  return (elapsed / 1000000ull);
#endif
}

u64 HighResolutionTimer::usecs() const {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  LARGE_INTEGER elapsed;
  elapsed.QuadPart = (now.QuadPart - impl->epoch.QuadPart);
  return ((elapsed.QuadPart * 1000000ull) / impl->freq.QuadPart);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  const u64 elapsed = ::mach_absolute_time() - impl->epoch;
  return ((elapsed * impl->tbi.numer * 1000ull) / impl->tbi.denom);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  struct timespec ts;
  ::clock_gettime(CLOCK_MONOTONIC, &ts);
  const u64 elapsed = (ts.tv_sec * 1000000000ull + ts.tv_nsec) - impl->epoch;
  return (elapsed / 1000ull);
#endif
}

u64 HighResolutionTimer::nsecs() const {
  Implementation *impl = (Implementation *)this;
#if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
  LARGE_INTEGER now;
  ::QueryPerformanceCounter(&now);
  LARGE_INTEGER elapsed;
  elapsed.QuadPart = (now.QuadPart - impl->epoch.QuadPart);
  return ((elapsed.QuadPart * 1000000000ull) / impl->freq.QuadPart);
#elif YETI_PLATFORM == YETI_PLATFORM_MAC_OS_X
  const u64 elapsed = ::mach_absolute_time() - impl->epoch;
  return ((elapsed * impl->tbi.numer) / impl->tbi.denom);
#elif YETI_PLATFORM == YETI_PLATFORM_LINUX
  struct timespec ts;
  ::clock_gettime(CLOCK_MONOTONIC, &ts);
  const u64 elapsed = (ts.tv_sec * 1000000000ull + ts.tv_nsec) - impl->epoch;
  return elapsed;
#endif
}

} // foundation
} // yeti
