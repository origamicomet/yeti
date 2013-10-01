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
/*! @file bt/foundation/architecture.h
      Defines known architectures.                                            */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_ARCHITECTURE_H_
#define _BT_FOUNDATION_ARCHITECTURE_H_

/*! @def BT_ARCHITECTURE_UNKNOWN_OR_UNSUPPORTED
  An unknown or unsupported architecture. */
#define BT_ARCHITECTURE_UNKNOWN_OR_UNSUPPORTED 0

/*! @def BT_ARCHITECTURE_X86
  Intel x86. */
#define BT_ARCHITECTURE_X86 1

/*! @def BT_ARCHITECTURE_X86_64
  Intel x86-64, aka AMD 64. */
#define BT_ARCHITECTURE_X86_64 2

/*! @def BT_ARCHITECTURE_ARM
  ARM's RISC. */
#define BT_ARCHITECTURE_ARM 3

#endif /* _BT_FOUNDATION_ARCHITECTURE_H_ */
