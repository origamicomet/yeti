//===-- yeti/core/platform/entropy.h --------------------*- mode: C++11 -*-===//
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
/// \brief Heat death of the universe accelerator.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_PLATFORM_ENTROPY_H_
#define _YETI_CORE_PLATFORM_ENTROPY_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

namespace yeti {
namespace core {

/// Returns @amount bytes of entropy.
///
/// \warning Will block until enough entropy is available.
///
extern YETI_PUBLIC void entropy(void *buffer, size_t amount);

} // core
} // yeti

#endif // _YETI_CORE_PLATFORM_ENTROPY_H_
