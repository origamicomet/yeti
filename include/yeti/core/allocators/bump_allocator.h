//===-- yeti/core/allocators/bump_allocator.h -----------*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_ALLOCATORS_BUMP_ALLOCATOR_H_
#define _YETI_CORE_ALLOCATORS_BUMP_ALLOCATOR_H_

#include "yeti/core/allocator.h"

namespace yeti {
namespace core {

// TODO(mtwilliams): Implement a bump allocator.
// TODO(mtwilliams): Allow bookmarking.

class YETI_PUBLIC BumpAllocator : public Allocator {};

} // core
} // yeti

#endif // _YETI_CORE_ALLOCATORS_BUMP_ALLOCATOR_H_
