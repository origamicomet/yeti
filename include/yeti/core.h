//===-- yeti/core.h -------------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_CORE_H_
#define _YETI_CORE_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core/types.h"
#include "yeti/core/support.h"

#include "yeti/core/atomics.h"
#include "yeti/core/bits.h"
#include "yeti/core/fp.h"

#include "yeti/core/utilities.h"

#include "yeti/core/debug/assert.h"
#include "yeti/core/debug/stacktrace.h"
#include "yeti/core/debug/registers.h"
#include "yeti/core/debug/errors.h"
#include "yeti/core/debug/handler.h"

#include "yeti/core/log.h"
#include "yeti/core/log/console.h"
#include "yeti/core/log/file.h"
#include "yeti/core/log/network.h"

#include "yeti/core/memory.h"

#include "yeti/core/allocator.h"
#include "yeti/core/allocators/global_heap_allocator.h"
#include "yeti/core/allocators/global_page_allocator.h"
#include "yeti/core/allocators/proxy_allocator.h"
#include "yeti/core/allocators/bump_allocator.h"
#include "yeti/core/allocators/buddy_allocator.h"
#include "yeti/core/allocators/thread_safe/bump_allocator.h"
#include "yeti/core/allocators/thread_safe/scratch_allocator.h"

#include "yeti/core/algorithms/hash.h"
#include "yeti/core/algorithms/digest.h"
#include "yeti/core/algorithms/random.h"

#include "yeti/core/containers/list.h"
#include "yeti/core/containers/array.h"
#include "yeti/core/containers/stack.h"
#include "yeti/core/containers/queue.h"
#include "yeti/core/containers/dequeue.h"
#include "yeti/core/containers/map.h"

#include "yeti/core/platform/info.h"
#include "yeti/core/platform/environment.h"
#include "yeti/core/platform/process.h"
#include "yeti/core/platform/thread.h"
#include "yeti/core/platform/entropy.h"
#include "yeti/core/platform/path.h"
#include "yeti/core/platform/filesystem.h"
#include "yeti/core/platform/lock.h"
#include "yeti/core/platform/reader_writer_lock.h"
#include "yeti/core/platform/event.h"
#include "yeti/core/platform/timer.h"

#include "yeti/core/network/dns.h"
#include "yeti/core/network/sockets.h"
#include "yeti/core/network/http.h"

#include "yeti/core/misc/ini.h"
#include "yeti/core/misc/sjson.h"

#include "yeti/core/misc/uuid.h"

#include "yeti/core/misc/pattern_file_parser.h"

#endif // _YETI_CORE_H_
