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
/*! @file bt/config.h
      Documents a collection of pre-processor defines used for configuration. */
/* ========================================================================== */

#ifndef _BT_CONFIG_H_
#define _BT_CONFIG_H_

/* ========================================================================== */
/*  Configuration:                                                            */
/*   * Compiler, architecture, and platform auto-detection                    */
/*   * Debugging / Build Type                                                 */
/* ========================================================================== */

/* ==========================================================================
    Compiler, architecture, and platform auto-detection:
   ========================================================================== */

/*! @def BT_DONT_AUTODETECT_COMPILER
  If defined Butane won't attempt to detect the current compiler based on
  pre-processor defines, instead it is expected to be user defined. */
#if defined(DOXYGEN)
  #define BT_DONT_AUTODETECT_COMPILER
#endif

/*! @def BT_DONT_AUTODETECT_PLATFORM
  If defined Butane won't attempt to detect the current platform based on
  pre-processor defines, instead it is expected to be user defined. */
#if defined(DOXYGEN)
  #define BT_DONT_AUTODETECT_PLATFORM
#endif

/*! @def BT_DONT_AUTODETECT_ARCHITECTURE
  If defined Butane won't attempt to detect the current architecture based on
  pre-processor defines, instead it is expected to be user defined. */
#if defined(DOXYGEN)
  #define BT_DONT_AUTODETECT_ARCHITECTURE
#endif

/* ==========================================================================
    Debugging / Build Type:
   ========================================================================== */

/*! @def BT_PARANOID
  If defined Butane will perform paranoid checks. */
#if defined(DOXYGEN)
  #define BT_PARANOID
#endif

/*! @def BT_DEBUG
  If defined Butane will perform debugging checks. */
#if defined(DOXYGEN)
  #define BT_DEBUG
#endif

/*! @def BT_DEVELOPMENT
  If defined Butane will perform development checks. */
#if defined(DOXYGEN)
  #define BT_DEVELOPMENT
#endif

/*! @def BT_RELEASE
  If defined Butane won't perform any checks. */
#if defined(DOXYGEN)
  #define BT_RELEASE
#endif

#endif /* _BT_CONFIG_H_ */
