//===-- yeti/foundation.h ---------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_H_
#define _YETI_FOUNDATION_H_

#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/atomics.h"
#include "yeti/foundation/assert.h"
#include "yeti/foundation/allocator.h"
#include "yeti/foundation/global_heap_allocator.h"
#include "yeti/foundation/array.h"
#include "yeti/foundation/high_res_timer.h"

#endif // _YETI_FOUNDATION_H_
