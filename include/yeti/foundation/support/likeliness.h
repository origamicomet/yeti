//===-- yeti/foundation/support/likeliness.h --------------*- mode: C++ -*-===//
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
/// compiler the probability of a branch occuring.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_LIKELINESS_H_
#define _YETI_FOUNDATION_SUPPORT_LIKELINESS_H_

/// \def YETI_LIKELY
/// \brief @_Condition is likely to evaluate to be true.
#if defined(DOXYGEN)
  #define YETI_LIKELY(_Condition) (!!(_Condition))
#else // !defined(DOXYGEN)
  #if defined(__GNUC__)
    #define YETI_LIKELY(_Condition) __builtin_expect(!!(_Condition), 1)
  #else
    #define YETI_LIKELY(_Condition) (!!(_Condition))
  #endif
#endif

/// \def YETI_UNLIKELY
/// \brief @_Condition is likely to evaluate to be false.
#if defined(DOXYGEN)
  #define YETI_UNLIKELY(_Condition) (!!(_Condition))
#else // !defined(DOXYGEN)
  #if defined(__GNUC__)
    #define YETI_UNLIKELY(_Condition) __builtin_expect(!!(_Condition), 0)
  #else
    #define YETI_UNLIKELY(_Condition) (!!(_Condition))
  #endif
#endif

#endif // _YETI_FOUNDATION_SUPPORT_LIKELINESS_H_
