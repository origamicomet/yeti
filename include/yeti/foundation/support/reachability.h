//===-- yeti/foundation/support/reachability.h ------------*- mode: C++ -*-===//
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
/// compiler the reachability of code.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_REACHABILITY_H_
#define _YETI_FOUNDATION_SUPPORT_REACHABILITY_H_

/// \def YETI_TRAP
/// \brief Errant, but reachable, code path.
#if defined(DOXYGEN)
  #define YETI_TRAP()
#else // !defined(DOXYGEN)
  #if defined(_MSC_VER)
    #define YETI_TRAP() __debugbreak()
  #elif defined(__GNUC__)
    #define YETI_TRAP() __builtin_trap()
  #endif
#endif

/// \def YETI_UNREACHABLE
/// \brief Code is unreachable.
#if defined(DOXYGEN)
  #define YETI_UNREACHABLE()
#else // !defined(YETI_UNREACHABLE)
  #if defined(_MSC_VER)
    #define YETI_UNREACHABLE() __assume(0)
  #elif defined(__clang__)
    #define YETI_UNREACHABLE() __builtin_unreachable()
  #elif defined(__GNUC__)
    #if __GNUC_VERSION__ >= 40500
      #define YETI_UNREACHABLE() __builtin_unreachable()
    #else // __GNUC_VERSION__ < 40500
      #include <signal.h>
      #define YETI_UNREACHABLE() do { ::signal(SIGTRAP); } while(0, 0)
    #endif
  #endif
#endif

#endif // _YETI_FOUNDATION_SUPPORT_REACHABILITY_H_
