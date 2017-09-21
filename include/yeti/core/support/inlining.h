//===-- yeti/core/support/inlining.h --------------------*- mode: C++11 -*-===//
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
/// compiler when code should be inlined.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_SUPPORT_INLINING_H_
#define _YETI_CORE_SUPPORT_INLINING_H_

/// \def YETI_INLINE
/// \brief Code should be inlined.
#if defined(DOXYGEN)
  #define YETI_INLINE
#else
  #if defined(_MSC_VER)
    #define YETI_INLINE __forceinline
  #elif defined(__clang__) || defined(__GNUC__)
    #define YETI_INLINE __inline __attribute__ ((always_inline))
  #endif
#endif

#endif // _YETI_CORE_SUPPORT_INLINING_H_
