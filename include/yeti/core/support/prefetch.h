//===-- yeti/core/support/prefetch.h --------------------*- mode: C++11 -*-===//
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
/// compiler to insert prefetch instructions.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_SUPPORT_PREFETCH_H_
#define _YETI_CORE_SUPPORT_PREFETCH_H_

#if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86 || \
    YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
  #include <mmintrin.h>
#endif

/// \def YETI_PREFETCH_L1
/// \brief Fetch into lowest cache level for inclusive access.
#if defined(DOXYGEN)
  #define YETI_PREFETCH_L1
#endif

/// \def YETI_PREFETCH_L2
/// \brief Fetch into second lowest cache level for inclusive access.
#if defined(DOXYGEN)
  #define YETI_PREFETCH_L2
#endif

/// \def YETI_PREFETCH_L3
/// \brief Fetch into third lowest cache level for inclusive access.
#if defined(DOXYGEN)
  #define YETI_PREFETCH_L3
#endif

/// \def YETI_PREFETCH_NTA
/// \brief Fetch into lowest cache level and spill to memory on eviction.
#if defined(DOXYGEN)
  #define YETI_PREFETCH_NTA
#endif

/// \def YETI_PREFETCH
/// \brief Hint to prefetch @Address with @Hint.
#if defined(DOXYGEN)
  #define YETI_PREFETCH(Address, Hint)
#else
  #if YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86 || \
       YETI_ARCHITECTURE == YETI_ARCHITECTURE_X86_64
    #define YETI_PREFETCH_L1  _MM_HINT_T0
    #define YETI_PREFETCH_L2  _MM_HINT_T1
    #define YETI_PREFETCH_L3  _MM_HINT_T2
    #define YETI_PREFETCH_NTA _MM_HINT_NTA

    #define YETI_PREFETCH(Address, Hint) \
      _mm_prefetch(((void *)Address), Hint)
  #else
    #define YETI_PREFETCH(Address, Hint)
  #endif
#endif

#endif // _YETI_CORE_SUPPORT_PREFETCH_H_
