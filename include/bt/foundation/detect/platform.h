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
/*! @file bt/foundation/detect/platform.h
      Auto-detects platform based on various pre-processor defines.           */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_DETECT_PLATFORM_H_
#define _BT_FOUNDATION_DETECT_PLATFORM_H_

#include <bt/config.h>
#include <bt/foundation/platform.h>

#ifdef BT_DONT_AUTODETECT_PLATFORM
  #ifndef BT_PLATFORM
    #error ("Platform auto-detection is disabled, but no platform is defined!")
  #else
    #if (BT_PLATFORM != BT_PLATFORM_WINDOWS) || \
        (BT_PLATFORM != BT_PLATFORM_MACOSX) || \
        (BT_PLATFORM != BT_PLATFORM_LINUX) || \
        (BT_PLATFORM != BT_PLATFORM_ANDROID) || \
        (BT_PLATFORM != BT_PLATFORM_IOS)
      #define BT_PLATFORM BT_PLATFORM_UNKNOWN_OR_UNSUPPORTED
    #endif
  #endif
#else /* #ifndef BT_DONT_AUTODETECT_PLATFORM */
  #if (defined(_WIN32) || defined(_WIN64))
    #define BT_PLATFORM BT_PLATFORM_WINDOWS
  #elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if defined(TARGET_OS_IPHONE)
      #define BT_PLATFORM BT_PLATFORM_IOS
    #else
      #define BT_PLATFORM BT_PLATFORM_MACOSX
    #endif
  #elif defined(__linux__)
    #if defined(ANDROID)
      #define BT_PLATFORM BT_PLATFORM_ANDROID
    #else
      #define BT_PLATFORM BT_PLATFORM_LINUX
    #endif
  #else
    #define BT_PLATFORM BT_PLATFORM_UNKNOWN_OR_UNSUPPORTED
  #endif
#endif /* #ifndef BT_DONT_AUTODETECT_PLATFORM */

#if (BT_PLATFORM == BT_PLATFORM_UNKNOWN_OR_UNSUPPORTED)
  #error ("Unknown or unsupported platform!")
#endif /* (BT_PLATFORM == BT_PLATFORM_UNKNOWN_OR_UNSUPPORTED) */

#endif /* _BT_FOUNDATION_DETECT_PLATFORM_H_ */
