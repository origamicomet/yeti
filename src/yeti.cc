//===-- yeti.cc -------------------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti.h"

namespace yeti {
  // TODO(mtwilliams): Improve default assertion handler.
  static void default_assertion_handler(const foundation::Assertion &assertion, void *) {
    ::fprintf(stderr, "Assertion failed!\n");
    ::fprintf(stderr, " predicate=\"%s\"\n", assertion.predicate);
    if (assertion.reason)
      ::fprintf(stderr, " reason=\"%s\"\n", assertion.reason);
    ::fprintf(stderr, " file=\"%s\" line=%u\n\n", assertion.location.file, (unsigned int)assertion.location.line);
    ::exit(EXIT_FAILURE);
  }
}

void yeti::initialize() {
  yeti::foundation::set_assertion_handler(&yeti::default_assertion_handler);

  resource_manager::initialize();
  resource_manager::track(ScriptResource::type());
}

void yeti::shutdown() {
  resource_manager::shutdown();
}

YETI_BEGIN_EXTERN_C // {

void __yeti__(void) {
  // HACK(mtwilliams): Export at least one symbol. This is to make sure shitty
  // linkers like Microsoft's actually produce a dynamically-linked library.
}

const char *__yeti_copyright__() {
  // Copyright 2013-20xx Origami Comet Games, Inc. All rights reserved.
  // Refer to our Rybfile for details.
  return __YETI_COPYRIGHT__;
}

const char *__yeti_version__() {
  // The hash of the commit this was built from: `git rev-parse HEAD`.
  // Refer to our Rybfile for details.
  return __YETI_VERSION__;
}

yeti_uint32_t __yeti_revision__() {
  // The number of commits composing the code this was built from: `git rev-list --count HEAD`.
  // Refer to our Rybfile for details.
  return __YETI_REVISION__;
}

YETI_END_EXTERN_C // }
