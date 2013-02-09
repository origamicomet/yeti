// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#ifndef _LWE_FOUNDATION_VEC2_H_
#define _LWE_FOUNDATION_VEC2_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>

#include <math.h>

typedef struct vec2_t {
  public: /* constructors */
    LWE_INLINE vec2_t()
      : x(0.0f), y(0.0f)
    {}

    LWE_INLINE vec2_t( float x, float y )
      : x(x), y(y)
    {}

    LWE_INLINE vec2_t( const vec2_t& v )
      : x(v.x), y(v.y)
    {}

    LWE_INLINE vec2_t& operator= ( const vec2_t& v ) {
      x = v.x; y = v.y;
      return *this;
    }

  public: /* operator overloads */
    LWE_INLINE vec2_t operator+ ( const vec2_t& v ) const {
      return vec2_t(x + v.x, y + v.y);
    }

    LWE_INLINE vec2_t operator- ( const vec2_t& v ) const {
      return vec2_t(x - v.x, y - v.y);
    }

    LWE_INLINE vec2_t operator* ( const vec2_t& v ) const {
      return vec2_t(x * v.x, y * v.y);
    }

    LWE_INLINE vec2_t operator/ ( const vec2_t& v ) const {
      return vec2_t(x / v.x, y / v.y);
    }

    LWE_INLINE vec2_t operator* ( float s ) const {
      return vec2_t(x * s, y * s);
    }

    LWE_INLINE vec2_t operator/ ( float d ) const {
      return vec2_t(x / d, y / d);
    }

  public: /* methods */
    LWE_INLINE float dot( const vec2_t& v ) const {
      return (x * v.x) + (y * v.y);
    }

    LWE_INLINE float length() const {
      return sqrt(dot(*this));
    }

    LWE_INLINE float magnitude() const {
      return sqrt(dot(*this));
    }

    LWE_INLINE vec2_t normalize() const {
      const float im = 1.0f / magnitude();
      return vec2_t(x * im, y * im);
    }

  public:
    float x, y;
} vec2_t;

#endif // _LWE_FOUNDATION_VEC2_H_