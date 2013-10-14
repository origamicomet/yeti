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
/*! @file bt/foundation/compat/warning.h
      Provides compiler independent warning messages.                         */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_COMPAT_WARNING_H_
#define _BT_FOUNDATION_COMPAT_WARNING_H_

#include <bt/foundation/detect/compiler.h>

#if ((BT_COMPILER == BT_COMPILER_GCC) || (BT_COMPILER == BT_COMPILER_CLANG))
  #define __bt_warning(_Msg) \
    _Pragma(#_Msg)
  #define bt_warning(_Msg) \
    __bt_warning(message "warning: " _Msg)
#elif (BT_COMPILER == BT_COMPILER_MSVC)
  // See http://stackoverflow.com/questions/3030099.
  #include <bt/foundation/preprocessor.h>
  #define bt_warning(_Msg) \
    __pragma(message(__FILE__ "(" bt_stringificate(__LINE__) ") : warning: " _Msg))
#else
  #error ("Unknown or unsupported compiler")
#endif

#endif /* _BT_FOUNDATION_COMPAT_WARNING_H_ */
