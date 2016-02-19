//===-- yeti/foundation/atomics.h -------------------------*- mode: C++ -*-===//
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

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #include <intrin.h>
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86
    #pragma intrinsic(_InterlockedExchangeAdd)
    #pragma intrinsic(_InterlockedCompareExchange)
    #pragma intrinsic(_InterlockedCompareExchange64)
  #elif YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    #pragma intrinsic(_InterlockedExchangeAdd)
    #pragma intrinsic(_InterlockedExchangeAdd64)
    #pragma intrinsic(_InterlockedCompareExchange)
    #pragma intrinsic(_InterlockedCompareExchange64)
  #endif
#elif (YETI_COMPILER == YETI_COMPILER_GCC) || \
      (YETI_COMPILER == YETI_COMPILER_CLANG)
#endif

namespace yeti {
namespace foundation {

/// \namespace ::yeti::foundation::atomic
/// \brief TODO(mtwilliam): Document this.
namespace atomic {
  i32 load(const volatile i32 *v);
  u32 load(const volatile u32 *v);
  i64 load(const volatile i64 *v);
  u64 load(const volatile u64 *v);
  void *load(const volatile void **v);

  void store(volatile i32 *v, const i32 desired);
  void store(volatile u32 *v, const u32 desired);
  void store(volatile i64 *v, const i64 desired);
  void store(volatile u64 *v, const u64 desired);
  void store(volatile void **v, const void *desired);

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
  void *cmp_and_xchg(volatile void **v, const void *expected, const void *desired);
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

YETI_INLINE void *atomic::load(const volatile void **v) {
  return (void *)load((uintptr_t *)v);
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

YETI_INLINE void atomic::store(volatile void **v, const void *desired) {
  store((uintptr_t *)v, (uintptr_t)desired);
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

YETI_INLINE void *atomic::cmp_and_xchg(volatile void **v, const void *expected, const void *desired) {
  return (void *)cmp_and_xchg((uintptr_t *)v, (uintptr_t)expected, (uintptr_t)desired);
}

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_ATOMICS_H_
