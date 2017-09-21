//===-- yeti/core/support/likeliness.h ------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_SUPPORT_LIKELINESS_H_
#define _YETI_CORE_SUPPORT_LIKELINESS_H_

/// \def YETI_LIKELY
/// \brief @Condition is likely to evaluate to be true.
#if defined(DOXYGEN)
  #define YETI_LIKELY(Condition) (!!(Condition))
#else
  #if defined(__GNUC__)
    #define YETI_LIKELY(Condition) __builtin_expect(!!(Condition), 1)
  #else
    #define YETI_LIKELY(Condition) (!!(Condition))
  #endif
#endif

/// \def YETI_UNLIKELY
/// \brief @Condition is likely to evaluate to be false.
#if defined(DOXYGEN)
  #define YETI_UNLIKELY(Condition) (!!(Condition))
#else
  #if defined(__GNUC__)
    #define YETI_UNLIKELY(Condition) __builtin_expect(!!(Condition), 0)
  #else
    #define YETI_UNLIKELY(Condition) (!!(Condition))
  #endif
#endif

#endif // _YETI_CORE_SUPPORT_LIKELINESS_H_
