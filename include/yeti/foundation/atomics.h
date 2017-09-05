//===-- yeti/foundation/atomics.h -----------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_ATOMICS_H_
#define _YETI_FOUNDATION_ATOMICS_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #include <intrin.h>
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    #pragma intrinsic(_InterlockedExchangeAdd)
    #pragma intrinsic(_InterlockedIncrement)
    #pragma intrinsic(_InterlockedDecrement)
    #pragma intrinsic(_InterlockedCompareExchange)
    #pragma intrinsic(_InterlockedCompareExchange64)
    #pragma intrinsic(_interlockedbittestandset)
    #pragma intrinsic(_interlockedbittestandreset)
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    #pragma intrinsic(_InterlockedExchangeAdd)
    #pragma intrinsic(_InterlockedExchangeAdd64)
    #pragma intrinsic(_InterlockedIncrement)
    #pragma intrinsic(_InterlockedDecrement)
    #pragma intrinsic(_InterlockedCompareExchange)
    #pragma intrinsic(_InterlockedCompareExchange64)
    #pragma intrinsic(_interlockedbittestandset)
    #pragma intrinsic(_interlockedbittestandset64)
    #pragma intrinsic(_interlockedbittestandreset)
    #pragma intrinsic(_interlockedbittestandreset64)
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif

// TODO(mtwilliams): Provide templated versions.
// TODO(mtwilliams): Verify type signatures.

namespace yeti {
namespace foundation {

/// \namespace ::yeti::foundation::atomic
namespace atomic {
  i32 load(const volatile i32 *v);
  u32 load(const volatile u32 *v);
  i64 load(const volatile i64 *v);
  u64 load(const volatile u64 *v);
  void *load(void ** const volatile v);

  void store(volatile i32 *v, const i32 desired);
  void store(volatile u32 *v, const u32 desired);
  void store(volatile i64 *v, const i64 desired);
  void store(volatile u64 *v, const u64 desired);
  void store(void ** volatile v, const void *desired);

  i32 increment(volatile i32 *v);
  u32 increment(volatile u32 *v);

  i32 decrement(volatile i32 *v);
  u32 decrement(volatile u32 *v);

  i32 add(volatile i32 *lhs, const i32 rhs);
  u32 add(volatile u32 *lhs, const u32 rhs);
  i64 add(volatile i64 *lhs, const i64 rhs);
  u64 add(volatile u64 *lhs, const u64 rhs);

  i32 sub(volatile i32 *lhs, const i32 rhs);
  u32 sub(volatile u32 *lhs, const u32 rhs);
  i64 sub(volatile i64 *lhs, const i64 rhs);
  u64 sub(volatile u64 *lhs, const u64 rhs);

  i32 cmp_and_xchg(volatile i32 *v, const i32 expected, const i32 desired);
  u32 cmp_and_xchg(volatile u32 *v, const u32 expected, const u32 desired);
  i64 cmp_and_xchg(volatile i64 *v, const i64 expected, const i64 desired);
  u64 cmp_and_xchg(volatile u64 *v, const u64 expected, const u64 desired);
  void *cmp_and_xchg(void ** volatile v, const void *expected, const void *desired);

#if YETI_COMPILER != YETI_COMPILER_MSVC
  // Conflicts with `u32` or `u64` in MSVC.
  uintptr_t load(const volatile uintptr_t *v);
  void store(volatile uintptr_t *v, const uintptr_t desired);
  uintptr_t cmp_and_xchg(volatile uintptr_t *v, const uintptr_t expected, const uintptr_t desired);
#endif

  bool set(volatile u32 *v, const u32 bit);
  bool set(volatile u64 *v, const u64 bit);

  bool unset(volatile u32 *v, const u32 bit);
  bool unset(volatile u64 *v, const u64 bit);

  template <typename T>
  T min(volatile T *v, const T versus);

  template <typename T>
  T max(volatile T *v, const T versus);
}

YETI_INLINE i32 atomic::load(const volatile i32 *v) {
#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  return *v;
#endif
}

YETI_INLINE u32 atomic::load(const volatile u32 *v) {
#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  return *v;
#endif
}

YETI_INLINE i64 atomic::load(const volatile i64 *v) {
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

YETI_INLINE u64 atomic::load(const volatile u64 *v) {
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

YETI_INLINE void *atomic::load(void ** const volatile v) {
  return (void *)load((const volatile uintptr_t *)v);
}

YETI_INLINE void atomic::store(volatile i32 *v, const i32 desired) {
#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  *v = desired;
#endif
}

YETI_INLINE void atomic::store(volatile u32 *v, const u32 desired) {
#if (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86) || \
    (YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64)
  *v = desired;
#endif
}

YETI_INLINE void atomic::store(volatile i64 *v, const i64 desired) {
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
                 : "b"((u32)desired), "c"((u32)(desired >> 32)), "A"(expected));
  #endif
#elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  *v = desired;
#endif
}

YETI_INLINE void atomic::store(volatile u64 *v, const u64 desired) {
  store((i64 *)v, (i64)desired);
}

YETI_INLINE void atomic::store(void ** volatile v, const void *desired) {
  store((volatile uintptr_t *)v, (uintptr_t)desired);
}

YETI_INLINE i32 atomic::increment(volatile i32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (i32)_InterlockedIncrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

YETI_INLINE u32 atomic::increment(volatile u32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (u32)_InterlockedIncrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

YETI_INLINE i32 atomic::decrement(volatile i32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (i32)_InterlockedDecrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

YETI_INLINE u32 atomic::decrement(volatile u32 *v) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return (u32)_InterlockedDecrement((volatile long *)v);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

YETI_INLINE i32 atomic::add(volatile i32 *lhs, const i32 rhs) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return _InterlockedExchangeAdd((volatile long *)lhs, (long)rhs);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_fetch_and_add(lhs, rhs);
#endif
}

YETI_INLINE u32 atomic::add(volatile u32 *lhs, const u32 rhs) {
  return (u32)add((i32 *)lhs, (i32)rhs);
}

YETI_INLINE i64 atomic::add(volatile i64 *lhs, const i64 rhs) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    // TODO(mtwilliams): Use XADD?
    // http://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/tree/arch/x86/include/asm/atomic.h?id=HEAD
    i64 r;
    __asm {
    retry:
      mov esi, lhs;
      mov eax, [esi];
      mov edx, [esi+4];
      mov ebx, dword ptr rhs;
      mov ecx, dword ptr rhs[4];
      add ebx, eax;
      adc ecx, edx;
      lock cmpxchg8b [esi];
      jne retry;
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return _InterlockedExchangeAdd64((volatile __int64 *)lhs, (__int64)rhs);
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_fetch_and_add(lhs, rhs);
#endif
}

YETI_INLINE u64 atomic::add(volatile u64 *lhs, const u64 rhs) {
  return (u64)add((i64 *)lhs, (i64)rhs);
}

YETI_INLINE i32 atomic::sub(volatile i32 *lhs, const i32 rhs) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return _InterlockedExchangeAdd((volatile long *)lhs, -((long)rhs));
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_fetch_and_sub(lhs, rhs);
#endif
}

YETI_INLINE u32 atomic::sub(volatile u32 *lhs, const u32 rhs) {
  return (u32)sub((i32 *)lhs, (i32)rhs);
}

YETI_INLINE i64 atomic::sub(volatile i64 *lhs, const i64 rhs) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    i64 r;
    __asm {
    retry:
      mov esi, lhs;
      mov eax, [esi];
      mov edx, [esi+4];
      mov ebx, dword ptr rhs;
      mov ecx, dword ptr rhs[4];
      sub ebx, eax;
      sbb ecx, edx;
      lock cmpxchg8b [esi];
      jne retry;
      mov dword ptr r, eax;
      mov dword ptr r[4], edx;
    }
    return r;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return _InterlockedExchangeAdd64((volatile __int64 *)lhs, -((__int64)rhs));
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_fetch_and_sub(lhs, rhs);
#endif
}

YETI_INLINE u64 atomic::sub(volatile u64 *lhs, const u64 rhs) {
  return (u64)sub((i64 *)lhs, (i64)rhs);
}

YETI_INLINE i32 atomic::cmp_and_xchg(volatile i32 *v, const i32 expected, const i32 desired) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return _InterlockedCompareExchange((volatile long *)v, (long)desired, (long)expected);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_val_compare_and_swap(v, expected, desired);
#endif
}

YETI_INLINE u32 atomic::cmp_and_xchg(volatile u32 *v, const u32 expected, const u32 desired) {
  return (u32)cmp_and_xchg((i32 *)v, (i32)expected, (i32)desired);
}

YETI_INLINE i64 atomic::cmp_and_xchg(volatile i64 *v, const i64 expected, const i64 desired) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return _InterlockedCompareExchange64((volatile __int64 *)v, (__int64)desired, (__int64)expected);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
  return __sync_val_compare_and_swap(v, expected, desired);
#endif
}

YETI_INLINE u64 atomic::cmp_and_xchg(volatile u64 *v, const u64 expected, const u64 desired) {
  return (u64)cmp_and_xchg((i64 *)v, (i64)expected, (i64)desired);
}

YETI_INLINE void *atomic::cmp_and_xchg(void ** volatile v, const void *expected, const void *desired) {
  return (void *)cmp_and_xchg((volatile uintptr_t *)v, (uintptr_t)expected, (uintptr_t)desired);
}

#if YETI_COMPILER != YETI_COMPILER_MSVC
  YETI_INLINE uintptr_t atomic::load(const volatile uintptr_t *v) {
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    return (uintptr_t)load((const volatile u32 *)v);
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return (uintptr_t)load((const volatile u64 *)v);
  #endif
  }

  YETI_INLINE void atomic::store(volatile uintptr_t *v, const uintptr_t desired) {
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    store((volatile u32 *)v, (u32)desired);
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    store((volatile u64 *)v, (u64)desired);
  #endif
  }

  YETI_INLINE uintptr_t atomic::cmp_and_xchg(volatile uintptr_t *v, const uintptr_t expected, const uintptr_t desired) {
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    return (uintptr_t)cmp_and_xchg((volatile u32 *)v, (u32)expected, (u32)desired);
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return (uintptr_t)cmp_and_xchg((volatile u64 *)v, (u64)expected, (u64)desired);
  #endif
  }
#endif

YETI_INLINE bool atomic::set(volatile u32 *v, const u32 bit) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return !!_interlockedbittestandset((volatile long *)v, bit);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

YETI_INLINE bool atomic::set(volatile u64 *v, const u64 bit) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    YETI_TRAP();
    return false;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return !!_interlockedbittestandset64((volatile __int64 *)v, bit);
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

YETI_INLINE bool atomic::unset(volatile u32 *v, const u32 bit) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  return !!_interlockedbittestandreset((volatile long *)v, bit);
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

YETI_INLINE bool atomic::unset(volatile u64 *v, const u64 bit) {
#if YETI_COMPILER == YETI_COMPILER_MSVC
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    YETI_TRAP();
    return false;
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    return !!_interlockedbittestandreset64((volatile __int64 *)v, bit);
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif
}

template <typename T>
YETI_INLINE T atomic::min(volatile T *v, const T versus) {
  while (true) {
    const T expected = atomic::load(v);
    const T smallest = (expected < versus) ? expected : versus;
    if (atomic::cmp_and_xchg(v, expected, smallest) == expected)
      return smallest;
  }
}

template <typename T>
YETI_INLINE T atomic::max(volatile T *v, const T versus) {
  while (true) {
    const T expected = atomic::load(v);
    const T largest = (expected > versus) ? expected : versus;
    if (atomic::cmp_and_xchg(v, expected, largest) == expected)
      return largest;
  }
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_ATOMICS_H_
