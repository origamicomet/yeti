//===-- yeti/core/fp.h ----------------------------------*- mode: C++11 -*-===//
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
/// \brief Utilities to manage floating-point environment.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_FP_H_
#define _YETI_CORE_FP_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include <float.h>

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #pragma fenv_access (on)
#elif YETI_COMPILER == YETI_COMPILER_CLANG || \
      YETI_COMPILER == YETI_COMPILER_GCC
  #pragma STDC FENV ACCESS ON
#endif

namespace yeti {
namespace core {

namespace fp {

// Defined somewhere?
#undef OVERFLOW
#undef UNDERFLOW

enum {
  INVALID          = (_EM_INVALID),
  DENORMAL         = (_EM_DENORMAL),
  OVERFLOW         = (_EM_OVERFLOW),
  UNDERFLOW        = (_EM_UNDERFLOW),
  DIVISION_BY_ZERO = (_EM_ZERODIVIDE)
};

}

/// Enables floating-point exceptions for the duration of scope.
///
/// \note Restores original floating-point exception masks after destruction.
///
class EnableFloatingPointExceptions {
 YETI_DISALLOW_COPYING(EnableFloatingPointExceptions)

 public:
  EnableFloatingPointExceptions(unsigned int mask = fp::INVALID |
                                                    fp::OVERFLOW |
                                                    fp::UNDERFLOW |
                                                    fp::DIVISION_BY_ZERO) {
    // Retrieve the current exception masks so we can restore them later.
    _controlfp_s(&original_, _MCW_EM, _MCW_EM);

    // Make sure only exception masks have been specified.
    mask &= _MCW_EM;

    // Clear any pending floating-point exceptions. Otherwise there may be a
    // deferred crash as soon the exceptions are (re)enabled.
    _clearfp();

    // Clear the specified masks, leaving other bits alone.
    _controlfp_s(0, ~mask, mask);
  }

  ~EnableFloatingPointExceptions() {
    // Restore the exception masks.
    _controlfp_s(0, original_, _MCW_EM);
  }

 private:
  unsigned int original_;
};

/// Disables floating-point exceptions for the duration of scope.
///
/// \note Restores original floating-point exception masks after destruction.
///
class DisableFloatingPointExceptions {
 YETI_DISALLOW_COPYING(DisableFloatingPointExceptions)

 public:
  DisableFloatingPointExceptions() {
    // Retrieve the current exception masks so we can restore them later.
    _controlfp_s(&original_, 0, 0);

    // Set all the exception masks, which suppresses floating-point exceptions.
    _controlfp_s(0, _MCW_EM, _MCW_EM);
  }

  ~DisableFloatingPointExceptions() {
    // Clear any pending floating-point exceptions. Otherwise there may be a
    // deferred crash as soon the exceptions are (re)enabled.
    _clearfp();

    // Restore the exception masks.
    _controlfp_s(0, original_, _MCW_EM);
  }

 private:
  unsigned int original_;
};

} // core
} // yeti

#endif // _YETI_CORE_FP_H_
