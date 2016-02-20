//===-- yeti.h --------------------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Your sherpa for the journey to the center of Yeti.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_H_
#define _YETI_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation.h"

#include "yeti/math.h"
#include "yeti/input.h"

#include "yeti/window.h"

YETI_BEGIN_EXTERN_C // {

extern YETI_PUBLIC void __yeti__();
extern YETI_PUBLIC const char *__yeti_copyright__();
extern YETI_PUBLIC const char *__yeti_version__();

YETI_END_EXTERN_C // }

#endif // _YETI_H_
