//===-- yeti/foundation/support/usage.h -------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines pre-processor macros that assist in conveying to the
/// compiler the usage of variables.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_USAGE_H_
#define _YETI_FOUNDATION_SUPPORT_USAGE_H_

/// \def YETI_UNUSED
/// \brief Unused argument.
#if defined(DOXYGEN)
  #define YETI_UNUSED(_Argument)
#else // !defined(DOXYGEN)
  #define YETI_UNUSED(_Argument) \
    (void)sizeof((_Argument));
#endif

#endif // _YETI_FOUNDATION_SUPPORT_USAGE_H_
