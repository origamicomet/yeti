//===-- yeti/color.h ------------------------------------*- mode: C++11 -*-===//
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
/// \brief Colors!
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_COLOR_H_
#define _YETI_COLOR_H_

#include "yeti/core.h"

namespace yeti {

/// \brief Represents a color.
class YETI_PUBLIC Color {
 public:
  Color() : r(0.f), g(0.f), b(0.f), a(0.f) {}
  Color(f32 r, f32 g, f32 b, f32 a = 1.f) : r(r), g(g), b(b), a(a) {}
  Color(const Color &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
  Color operator=(const Color &c) { r = c.r; g = c.g; b = c.b; a = c.a; return *this; }

 public:
  /// \brief Constructs a color from HSL.
  static Color hsl(f32 hue, f32 saturation, f32 lightness, f32 alpha = 1.f);

  /// \brief Constructs a color from HSV.
  static Color hsv(f32 hue, f32 saturation, f32 value, f32 alpha = 1.f);

  /// \brief Constructs a color from YCoCg.
  static Color ycocg(f32 y, f32 co, f32 cg, f32 alpha = 1.f);

  /// \brief Constructs a color from YCbCr.
  static Color ycbcr(f32 y, f32 cb, f32 cr, f32 alpha = 1.f);

 public:
  friend Color operator+(const Color lhs, const Color rhs);
  friend Color operator-(const Color lhs, const Color rhs);
  friend Color operator*(const Color lhs, const Color rhs);

 public:
  /// \brief Linearly mixes between @a and @b based on @t.
  static Color mix(const Color a, const Color b, const f32 t);

 public:
  static const Color BLACK;
  static const Color WHITE;
  static const Color RED;
  static const Color GREEN;
  static const Color BLUE;

 public:
  f32 r, g, b, a;
};

YETI_INLINE Color operator+(const Color lhs, const Color rhs) {
  Color color = Color(lhs.r + rhs.r,
                      lhs.g + rhs.g,
                      lhs.b + rhs.b,
                      lhs.a + rhs.a);

  color.r = (color.r > 1.f) ? 1.f : color.r;
  color.g = (color.g > 1.f) ? 1.f : color.g;
  color.b = (color.b > 1.f) ? 1.f : color.b;
  color.a = (color.a > 1.f) ? 1.f : color.a;

  return color;
}

YETI_INLINE Color operator-(const Color lhs, const Color rhs) {
  Color color = Color(lhs.r - rhs.r,
                      lhs.g - rhs.g,
                      lhs.b - rhs.b,
                      lhs.a - rhs.a);

  color.r = (color.r < 0.f) ? 0.f : color.r;
  color.g = (color.g < 0.f) ? 0.f : color.g;
  color.b = (color.b < 0.f) ? 0.f : color.b;
  color.a = (color.a < 0.f) ? 0.f : color.a;

  return color;
}

YETI_INLINE Color operator*(const Color lhs, const Color rhs) {
  return Color(
    lhs.r * rhs.r,
    lhs.g * rhs.g,
    lhs.b * rhs.b,
    lhs.a * rhs.a
  );
}

YETI_INLINE Color Color::mix(const Color a, const Color b, const f32 t) {
  return Color(
    a.r + (b.r - a.r) * t,
    a.g + (b.g - a.g) * t,
    a.b + (b.b - a.b) * t,
    a.a + (b.a - a.a) * t
  );
}

} // yeti

#endif // _YETI_COLOR_H_
