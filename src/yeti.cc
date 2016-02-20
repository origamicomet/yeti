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

YETI_BEGIN_EXTERN_C // {

YETI_PUBLIC
void __yeti__(void)
{
  // HACK(mtwilliams): Export at least one symbol. This is to make sure shitty
  // linkers like Microsoft's actually produce a dynamically-linked library.
}

YETI_PUBLIC
const char *__yeti_copyright__() {
  // Copyright 2013-20xx Origami Comet Games, Inc. All rights reserved.
  // Refer to our Rakefile for details.
  return __YETI_COPYRIGHT__;
}

YETI_PUBLIC
const char *__yeti_version__() {
  // The hash of the commit this was built from: `git rev-parse HEAD`.
  // Refer to our Rakefile for details.
  return __YETI_VERSION__;
}

YETI_END_EXTERN_C // }
