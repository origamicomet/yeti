//===-- yeti/color.cc -----------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/color.h"

namespace yeti {

const Color Color::BLACK = Color(0.f, 0.f, 0.f, 1.f);
const Color Color::WHITE = Color(1.f, 1.f, 1.f, 1.f);
const Color Color::RED   = Color(1.f, 0.f, 0.f, 1.f);
const Color Color::GREEN = Color(0.f, 1.f, 0.f, 1.f);
const Color Color::BLUE  = Color(0.f, 0.f, 1.f, 1.f);

} // yeti
