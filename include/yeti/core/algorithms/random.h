//===-- yeti/core/algorithms/random.h -------------------*- mode: C++11 -*-===//
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
/// \brief (Pseudo) random number generation.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_ALGORITHMS_RANDOM_H_
#define _YETI_CORE_ALGORITHMS_RANDOM_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// Returns a pseudo-random number.
template <typename T>
extern YETI_PUBLIC T random();

/// Returns a pseudo-random number between @min and @max, inclusive.
template <typename T>
extern YETI_PUBLIC T random(const T min, const T max);

} // core
} // yeti

#endif // _YETI_CORE_ALGORITHMS_RANDOM_H_
