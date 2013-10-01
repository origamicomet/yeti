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
/*! @file bt/foundation/detect/compiler.h
      Auto-detects compiler based on various pre-processor defines.           */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_DETECT_COMPILER_H_
#define _BT_FOUNDATION_DETECT_COMPILER_H_

#include <bt/config.h>
#include <bt/foundation/compiler.h>

#ifdef BT_DONT_AUTODETECT_COMPILER
  #ifndef BT_COMPILER
    #error ("Compiler auto-detection is disabled, but no compiler is defined!")
  #else
    #if (BT_COMPILER != BT_COMPILER_MSVC) || \
        (BT_COMPILER != BT_COMPILER_GCC) || \
        (BT_COMPILER != BT_COMPILER_CLANG)
      #define BT_COMPILER BT_COMPILER_UNKNOWN_OR_UNSUPPORTED
    #endif
  #endif
#else /* #ifndef BT_DONT_AUTODETECT_COMPILER */
  #if defined(_MSC_VER)
    #define BT_COMPILER BT_COMPILER_MSVC
  #elif (defined(__GNUC__) || defined(__GNUG__)) && \
        !(defined(__clang__) || defined(__ICC) || defined(__INTEL_COMPILER))
    #define BT_COMPILER BT_COMPILER_GCC
  #elif defined(__clang__)
    #define BT_COMPILER BT_COMPILER_CLANG
  #else
    #define BT_COMPILER BT_COMPILER_UNKNOWN_OR_UNSUPPORTED
  #endif
#endif /* #ifndef BT_DONT_AUTODETECT_COMPILER */

#if (BT_COMPILER == BT_COMPILER_UNKNOWN_OR_UNSUPPORTED)
  #error ("Unknown or unsupported compiler!")
#endif /* (BT_COMPILER == BT_COMPILER_UNKNOWN_OR_UNSUPPORTED) */

#endif /* _BT_FOUNDATION_DETECT_COMPILER_H_ */
