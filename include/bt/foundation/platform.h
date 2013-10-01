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
/*! @file bt/foundation/platform.h
      Defines known platforms.                                                */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_PLATFORM_H_
#define _BT_FOUNDATION_PLATFORM_H_

/*! @def BT_PLATFORM_UNKNOWN_OR_UNSUPPORTED
  An unknown or unsupported architecture. */
#define BT_PLATFORM_UNKNOWN_OR_UNSUPPORTED 0

/*! @def BT_PLATFORM_WINDOWS
  Microsoft Windows. */
#define BT_PLATFORM_WINDOWS 1

/*! @def BT_PLATFORM_MACOSX
  Apple Mac OSX. */
#define BT_PLATFORM_MACOSX 2

/*! @def BT_PLATFORM_LINUX
  GNU/Linux. */
#define BT_PLATFORM_LINUX 3

/*! @def BT_PLATFORM_ANDROID
  Google Android. */
#define BT_PLATFORM_ANDROID 4

/*! @def BT_PLATFORM_IOS
  Apple iOS. */
#define BT_PLATFORM_IOS 5

/*! @def BT_PLATFORM_IS_POSIX
  Determines if the specified platform conforms to the POSIX standard. */
#define BT_PLATFORM_IS_POSIX(_Platform) \
  ((_Platform == BT_PLATFORM_MACOSX) || \
   (_Platform == BT_PLATFORM_LINUX) || \
   (_Platform == BT_PLATFORM_ANDROID) || \
   (_Platform == BT_PLATFORM_IOS))

#endif /* _BT_FOUNDATION_PLATFORM_H_ */
