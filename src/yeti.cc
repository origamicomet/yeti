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
  // HACK(mtwilliams): We need to export at least on symbol for Microsoft's
  // linker to actually produce a dynamically linked library.
}

YETI_END_EXTERN_C // }
