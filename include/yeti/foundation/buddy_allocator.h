//===-- yeti/foundation/buddy_allocator.h -----------------*- mode: C++ -*-===//
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

#ifndef _YETI_FOUNDATION_BUDDY_ALLOCATOR_H_
#define _YETI_FOUNDATION_BUDDY_ALLOCATOR_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

#include "yeti/foundation/allocator.h"

namespace yeti {
namespace foundation {

// TODO(mtwilliams): Implement a buddy allocator with minimal overhead.
 // Refer to Dale's implementation https://github.com/graphitemaster/neothyne/blob/master/u_string.cpp

class YETI_PUBLIC BuddyAllocator : public Allocator {
};

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_BUDDY_ALLOCATOR_H_
