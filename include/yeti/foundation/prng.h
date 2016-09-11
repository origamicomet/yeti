//===-- yeti/foundation/prng.h ----------------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_PRNG_H_
#define _YETI_FOUNDATION_PRNG_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

/// Generates a pseudo-random number.
/// \note Random integeral numbers range from 0 to 2^n-1 inclusive.
/// \note Random floating-point numbers range from 0.0 to 1.0 inclusive.
template <typename T>
extern YETI_PUBLIC T prng();

/// Generates a pseudo-random number in the inclusive range specified.
template <typename T>
extern YETI_PUBLIC T prng(const T min, const T max);

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_PRNG_H_
