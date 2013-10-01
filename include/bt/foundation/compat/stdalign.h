/*
 * This file is part of PolySWF.
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
/*! @file bt/foundation/compat/stdalign.h
      Provides an ISO C1x compliant stdalign.h.                               */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_COMPAT_STDALIGN_H_
#define _BT_FOUNDATION_COMPAT_STDALIGN_H_

#include <stddef.h>

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ < 201112L))
  #ifndef alignof
    #define alignof(_Type) ((size_t)offsetof(struct{char _; _Type __;},__))
  #endif /* ifndef alignof */
#elif (defined(__cplusplus) && (__cplusplus < 201103L) && !defined(__GXX_EXPERIMENTAL_CXX0X__))
  /* Bless me, Father, for I have sinned. */
  template <typename _Type> struct __butane_alignof;
  template <int _SzDiff> struct __butane_alignof_ {
    template <typename _Type> struct E { enum { _ = _SzDiff }; }; };
  template <> struct __butane_alignof_<0> {
    template <typename _Type> struct E { enum { _ = __butane_alignof<_Type>::_ }; }; };
  template <typename _Type> struct __butane_alignof {
    struct C { _Type __; char _; };
    enum { D = (sizeof(C) - sizeof(_Type)), _ = __butane_alignof_<D>::template E<C>::_ }; };
  #ifndef alignof
    #define alignof(_Type) ((size_t)__butane_alignof<_Type>::_)
  #endif /* ifndef alignof */
#else
  #include <stdalign.h>
#endif

#endif /* _BT_FOUNDATION_COMPAT_STDALIGN_H_ */
