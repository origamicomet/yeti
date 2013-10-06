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
/*! @file bt/foundation/compat/attributes.h
      Provides methods to specify attributes to the compiler.                 */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_COMPAT_ATTRIBUTES_H_
#define _BT_FOUNDATION_COMPAT_ATTRIBUTES_H_

#include <bt/foundation/detect/compiler.h>

/* @def BT_ALIGN
  Defines a type `_Type` with an alignment of `_Alignment` bits. */
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_ALIGN(_Type, _Alignment) \
    __attribute__ ((aligned(_Alignment))) _Type
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_ALIGN(_Type, _Alignment) \
    __declspec(align(_Alignment)) _Type
#else
  #error ("Alignment attribute is unsupported!")
#endif

/* @def BT_THREAD_LOCAL
  Marks a variable as thread-local. */
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_THREAD_LOCAL __thread
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_THREAD_LOCAL __declspec(thread)
#else
  #error ("Thread-local storage attribute is unsupported!")
#endif

/* @def BT_NAKED
  Disables prologue and epilogue generation for function. */
#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_NAKED __attribute__ ((naked))
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_NAKED __declspec(naked)
#else
  #error ("Naked function attribute is unsupported!")
#endif

#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define BT_STDCALL __attribute__ ((stdcall))
  #define BT_CDECL __attribute__ ((cdecl))
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  #define BT_STDCALL __stdcall
  #define BT_CDECL __cdecl
#else
  #error ("Calling convention function attributes are unsupported!")
#endif

#endif /* _BT_FOUNDATION_COMPAT_ATTRIBUTES_H_ */
