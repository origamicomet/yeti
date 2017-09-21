//===-- yeti/core/debug/assert.cc -----------------------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/core/debug/assert.h"

namespace yeti {
namespace core {

namespace {
  static AssertionHandler assertion_handler_ = NULL;
  static void *assertion_handler_context_ = NULL;
}

void assertion_handler(AssertionHandler *assertion_handler,
                       void **context) {
  *assertion_handler = assertion_handler_;
  *context = assertion_handler_context_;
}

void set_assertion_handler(AssertionHandler assertion_handler,
                           void *context) {
  assertion_handler_ = assertion_handler;
  assertion_handler_context_ = context;
}

void raise_an_assertion(const Assertion &assertion) {
  if (assertion_handler_)
    assertion_handler_(assertion, assertion_handler_context_);
}

} // core
} // yeti
