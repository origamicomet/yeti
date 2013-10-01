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
/*! @file bt/foundation/detect/architecture.h
      Auto-detects architecture based on various pre-processor defines.       */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_DETECT_ARCHITECTURE_H_
#define _BT_FOUNDATION_DETECT_ARCHITECTURE_H_

#include <bt/config.h>
#include <bt/foundation/architecture.h>

#ifdef BT_DONT_AUTODETECT_ARCHITECTURE
  #ifndef BT_ARCHITECTURE
    #error ("Architecture auto-detection is disabled, but no architecture is defined!")
  #else
    #if (BT_ARCHITECTURE != BT_ARCHITECTURE_X86) || \
        (BT_ARCHITECTURE != BT_ARCHITECTURE_X86_64) || \
        (BT_ARCHITECTURE != BT_ARCHITECTURE_ARM)
      #define BT_ARCHITECTURE BT_ARCHITECTURE_UNKNOWN_OR_UNSUPPORTED
    #endif
  #endif
#else /* #ifndef BT_DONT_AUTODETECT_ARCHITECTURE */
  #if (defined(_M_IX86) || defined(__i386__))
    #define BT_ARCHITECTURE BT_ARCHITECTURE_X86
  #elif (defined(_M_X64) || defined(__x86_64__))
    #define BT_ARCHITECTURE BT_ARCHITECTURE_X86_64
  #elif defined(__ARMEL__)
    #define BT_ARCHITECTURE BT_ARCHITECTURE_ARM
  #else
    #define BT_ARCHITECTURE BT_ARCHITECTURE_UNKNOWN_OR_UNSUPPORTED
  #endif
#endif /* #ifndef BT_DONT_AUTODETECT_ARCHITECTURE */

#if (BT_ARCHITECTURE == BT_ARCHITECTURE_UNKNOWN_OR_UNSUPPORTED)
  #error ("Unknown or unsupported architecture!")
#endif /* (BT_ARCHITECTURE == BT_ARCHITECTURE_UNKNOWN_OR_UNSUPPORTED) */

#endif /* _BT_FOUNDATION_DETECT_ARCHITECTURE_H_ */
