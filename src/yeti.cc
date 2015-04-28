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
  // HACK(mtwilliams): Export at least one symbol. This is to make sure
  // Microsoft's linker actually produces a dynamically-linked library.
}

YETI_END_EXTERN_C // }
