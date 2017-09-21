//===-- yeti/input/feeder.h -----------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_INPUT_FEEDER_H_
#define _YETI_INPUT_FEEDER_H_

#include "yeti/core.h"

namespace yeti {

// See `yeti/window.h`.
class Window;

namespace input {

/// \internal Watches @window for input and feeds into the pertinent singletons.
extern YETI_PUBLIC void from(const Window *window);

} // input

} // yeti

#endif // _YETI_INPUT_FEEDER_H_
