//===-- yeti/foundation/assert.cc -----------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

static AssertionHandler assertion_handler_ = NULL;
static void *assertion_handler_ctx_ = NULL;

void assertion_handler(AssertionHandler *assertion_handler, void **ctx) {
  *assertion_handler = assertion_handler_;
  *ctx = assertion_handler_ctx_;
}

void set_assertion_handler(AssertionHandler new_assertion_handler, void *ctx) {
  assertion_handler_ = new_assertion_handler;
  assertion_handler_ctx_ = ctx;
}

} // foundation
} // yeti
