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
/*! @file bt/foundation/compat/hinting.h
      Provides methods of hinting optimizations to the compiler.              */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_COMPAT_HINTS_H_
#define _BT_FOUNDATION_COMPAT_HINTS_H_

#include <bt/foundation/detect/compiler.h>
#include <bt/foundation/compat/warning.h>

/*! @def BT_INLINE
  Hint to the compiler to inline a function aggressively as possible. */
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_INLINE __attribute__ ((always_inline))
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_INLINE __forceinline
#else
  bt_warning ("Inline forcing function attribute is unsupported, thus performance may be degraded!")
  #define BT_INLINE inline
#endif

/*! @def BT_NEVER_INLINE
  Hint to the compiler to never inline a function. */
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_NEVER_INLINE __attribute__ ((noinline))
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_NEVER_INLINE __declspec(noinline)
#else
  bt_warning ("Never inline forcing function attribute is unsupported, thus performance may be degraded!")
  #define BT_NEVER_INLINE
#endif

/*! @def BT_NO_ALIAS
  Hint to the compiler that global state is never mutated by a function. */
#if (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_NO_ALIAS __declspec(noalias)
#else
  bt_warning ("No-alias function attribute is unsupported, thus performance may be degraded!")
  #define BT_NO_ALIAS
#endif

/*! @def BT_DEPRECATED
  Hint to the compiler and user that a function is deprecated and that usage should be limited upon usage. */
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_DEPRECATED __attribute__ ((deprecated))
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_DEPRECATED __declspec(deprecated)
#else
  bt_warning ("Deprecated function attribute is unsupported, thus deprecated function usage does not raise a warning!")
  #define BT_DEPRECATED
#endif

/*! @def BT_RESTRICT
  Hint to the compiler that no other pointer aliases to this pointer. */
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_RESTRICT __restrict
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_RESTRICT __restrict
#else
  bt_warning ("Restrict attribute is unsupported, thus performance may be degraded!")
  #define BT_RESTRICT
#endif

#endif /* _BT_FOUNDATION_COMPAT_HINTS_H_ */
