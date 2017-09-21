//===-- yeti/core/atomics.h -----------------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Atomic operations.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_ATOMICS_H_
#define _YETI_CORE_ATOMICS_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

// For `YETI_MIN` and `YETI_MAX`.
#include "yeti/core/utilities.h"

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #include <intrin.h>

  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86 || \
      YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    #pragma intrinsic(_InterlockedIncrement)
    #pragma intrinsic(_InterlockedDecrement)
    #pragma intrinsic(_InterlockedCompareExchange)
    #pragma intrinsic(_InterlockedCompareExchangePointer)

    #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
      #pragma intrinsic(_InterlockedIncrement64)
      #pragma intrinsic(_InterlockedDecrement64)
    #endif

    #pragma intrinsic(_InterlockedCompareExchange64)
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  // No header file for intrinsics.
#endif

// We disable local function reference checking because we don't care if our
// conversion helpers are dead-code eliminated as they're in scope to promote
// inlining.
#pragma warning(disable: 4505)

namespace yeti {

/// \namespace ::yeti::atomic
/// \brief Atomic operations.
namespace atomic {
  /// \brief Atomically gets value of @v.
  /// @{
  static i32 load(const volatile i32 *v);
  static u32 load(const volatile u32 *v);
  static i64 load(const volatile i64 *v);
  static u64 load(const volatile u64 *v);
  static void *load(void ** const volatile v);
  /// @}

  /// \brief Atomically sets @v to @desired.
  /// @{
  static void store(volatile i32 *v, const i32 desired);
  static void store(volatile u32 *v, const u32 desired);
  static void store(volatile i64 *v, const i64 desired);
  static void store(volatile u64 *v, const u64 desired);
  static void store(void *volatile *v, void *desired);
  /// @}

  /// \brief Atomically increments @v.
  /// \return Incremented value.
  /// @{
  static i32 increment(volatile i32 *v);
  static u32 increment(volatile u32 *v);
  static i64 increment(volatile i64 *v);
  static u64 increment(volatile u64 *v);
  /// @}

  /// \brief Atomically decrements @v.
  /// \return Decrement value.
  /// @{
  static i32 decrement(volatile i32 *v);
  static u32 decrement(volatile u32 *v);
  static i64 decrement(volatile i64 *v);
  static u64 decrement(volatile u64 *v);
  /// @}

  /// \brief Atomically sets @v to @desired if it matched @expected.
  /// \return Original value of @v.
  /// @{
  static i32 cmp_and_xchg(volatile i32 *v, const i32 expected, const i32 desired);
  static u32 cmp_and_xchg(volatile u32 *v, const u32 expected, const u32 desired);
  static i64 cmp_and_xchg(volatile i64 *v, const i64 expected, const i64 desired);
  static u64 cmp_and_xchg(volatile u64 *v, const u64 expected, const u64 desired);
  static void *cmp_and_xchg(void *volatile *v, void *expected, void *desired);
  /// @}

  /// \brief Atomically sets @a to lesser of @a and @b.
  /// \return Lesser of @a and @b.
  template <typename T>
  static T min(volatile T *a, const T b);

  /// \brief Atomically sets @a to greater of @a and @b.
  /// \return Greater of @a and @b.
  template <typename T>
  static T max(volatile T *a, const T b);
}

#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  // Naturally aligned loads are atomic on x86 and x86_64.
  static i32 atomic::load(const volatile i32 *v) { return *v; }
  static u32 atomic::load(const volatile u32 *v) { return *v; }
#endif

static i64 atomic::load(const volatile i64 *v) {
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  #if YETI_COMPILER == YETI_COMPILER_MSVC
    i64 r;
    __asm {
      mov esi, v;
      mov ebx, eax;
      mov ecx, edx;
      lock cmpxchg8b [esi];
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
        (YETI_COMPILER == YETI_COMPILER_CLANG)
    i64 r;
    asm volatile("movl %%ebx, %%eax\n"
                 "movl %%ecx, %%edx\n"
                 "lock; cmpxchg8b %1"
                 : "=&A"(r)
                 : "m"(v));
    return r;
  #endif
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  return *v;
#endif
}

static u64 atomic::load(const volatile u64 *v) {
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  #if YETI_COMPILER == YETI_COMPILER_MSVC
    u64 r;
    __asm {
      mov esi, v;
      mov ebx, eax;
      mov ecx, edx;
      lock cmpxchg8b [esi];
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
        (YETI_COMPILER == YETI_COMPILER_CLANG)
    u64 r;
    asm volatile("movl %%ebx, %%eax\n"
                 "movl %%ecx, %%edx\n"
                 "lock; cmpxchg8b %1"
                 : "=&A"(r)
                 : "m"(v));
    return r;
  #endif
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  return *v;
#endif
}

static void *atomic::load(void ** const volatile v) {
#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  return *v;
#endif
}

#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  // Naturally aligned stores are atomic on x86 and x86_64.
  static void atomic::store(volatile i32 *v, const i32 desired) { *v = desired; }
  static void atomic::store(volatile u32 *v, const u32 desired) { *v = desired; }
#endif

static void atomic::store(volatile i64 *v, const i64 desired) {
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  #if YETI_COMPILER == YETI_COMPILER_MSVC
    __asm {
      mov esi, v;
      mov ebx, dword ptr desired;
      mov ecx, dword ptr desired[4];
    retry:
      cmpxchg8b [esi];
      jne retry;
    }
  #elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
        (YETI_COMPILER == YETI_COMPILER_CLANG)
    i64 expected = *v;
    asm volatile("1:  cmpxchg8b %0\n"
                 "    jne 1b"
                 : "=m"(*v)
                 : "b"((u32)desired),
                   "c"((u32)(desired >> 32)),
                   "A"(expected));
  #endif
#else
  *v = desired;
#endif
}

static void atomic::store(volatile u64 *v, const u64 desired) {
#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
  #if YETI_COMPILER == YETI_COMPILER_MSVC
    __asm {
      mov esi, v;
      mov ebx, dword ptr desired;
      mov ecx, dword ptr desired[4];
    retry:
      cmpxchg8b [esi];
      jne retry;
    }
  #elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
        (YETI_COMPILER == YETI_COMPILER_CLANG)
    u64 expected = *v;
    asm volatile("1:  cmpxchg8b %0\n"
                 "    jne 1b"
                 : "=m"(*v)
                 : "b"((u32)desired),
                   "c"((u32)(desired >> 32)),
                   "A"(expected));
  #endif
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  *v = desired;
#endif
}

static void atomic::store(void *volatile *v, void *desired) {
#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  *v = desired;
#endif
}

static i32 atomic::increment(volatile i32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (i32)_InterlockedIncrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

static u32 atomic::increment(volatile u32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (u32)_InterlockedIncrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

// TODO(mtwilliams): Validate inline assembly.

static i64 atomic::increment(volatile i64 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    i64 r;
    __asm {
      mov esi, v;
    retry:
      mov ebx, dword ptr v;
      mov ecx, dword ptr v[4];
      add ebx, 1
      adc ecx, 0
      lock cmpxchg8b [esi];
      jne retry;
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return (i64)_InterlockedIncrement64((volatile __int64 *)v);
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

static u64 atomic::increment(volatile u64 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    u64 r;
    __asm {
      mov esi, v;
    retry:
      mov ebx, dword ptr v;
      mov ecx, dword ptr v[4];
      add ebx, 1
      adc ecx, 0
      lock cmpxchg8b [esi];
      jne retry;
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return (u64)_InterlockedIncrement64((volatile __int64 *)v);
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

static i32 atomic::decrement(volatile i32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (i32)_InterlockedDecrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

static u32 atomic::decrement(volatile u32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (u32)_InterlockedDecrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

// TODO(mtwilliams): Validate inline assembly.

static i64 atomic::decrement(volatile i64 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    i64 r;
    __asm {
      mov esi, v;
    retry:
      mov ebx, dword ptr v;
      mov ecx, dword ptr v[4];
      sub ecx, 1
      sbb ebx, 0
      lock cmpxchg8b [esi];
      jne retry;
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return (i64)_InterlockedDecrement64((volatile __int64 *)v);
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

static u64 atomic::decrement(volatile u64 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    u64 r;
    __asm {
      mov esi, v;
    retry:
      mov ebx, dword ptr v;
      mov ecx, dword ptr v[4];
      sub ecx, 1
      sbb ebx, 0
      lock cmpxchg8b [esi];
      jne retry;
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return (u64)_InterlockedDecrement64((volatile __int64 *)v);
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

static i32 atomic::cmp_and_xchg(volatile i32 *v, const i32 expected, const i32 desired) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (i32)_InterlockedCompareExchange((volatile long *)v,
                                          (long)desired,
                                          (long)expected);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_val_compare_and_swap(v, expected, desired);
#endif
}

static u32 atomic::cmp_and_xchg(volatile u32 *v, const u32 expected, const u32 desired) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (u32)_InterlockedCompareExchange((volatile long *)v,
                                          (long)desired,
                                          (long)expected);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_val_compare_and_swap(v, expected, desired);
#endif
}

static i64 atomic::cmp_and_xchg(volatile i64 *v, const i64 expected, const i64 desired) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (i64)_InterlockedCompareExchange64((volatile __int64 *)v,
                                            (__int64)desired,
                                            (__int64)expected);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_val_compare_and_swap(v, expected, desired);
#endif
}

static u64 atomic::cmp_and_xchg(volatile u64 *v, const u64 expected, const u64 desired) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (u64)_InterlockedCompareExchange64((volatile __int64 *)v,
                                            (__int64)desired,
                                            (__int64)expected);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_val_compare_and_swap(v, expected, desired);
#endif
}

static void *atomic::cmp_and_xchg(void *volatile *v, void *expected, void *desired) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return _InterlockedCompareExchangePointer(v, desired, expected);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_val_compare_and_swap(v, desired, expected);
#endif
}

template <typename T>
static T atomic::min(volatile T *a, const T b) {
  while (true) {
    const T value  = atomic::load(a);
    const T lesser = YETI_MIN(value, b);

    if (atomic::cmp_and_xchg(a, value, lesser) != value)
      continue;

    return lesser;
  }
}

template <typename T>
static T atomic::max(volatile T *a, const T b) {
  while (true) {
    const T value   = atomic::load(a);
    const T greater = YETI_MAX(value, b);

    if (atomic::cmp_and_xchg(a, value, greater) != value)
      continue;

    return greater;
  }
}

} // yeti

#endif // _YETI_CORE_ATOMICS_H_
