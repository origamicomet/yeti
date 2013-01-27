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

#ifndef _LWE_FOUNDATION_VEC3_H_
#define _LWE_FOUNDATION_VEC3_H_

#include <lwe/foundation/types.h>
#include <lwe/foundation/preprocessor.h>

typedef struct vec3_t {
  public: /* constructors */
    LWE_INLINE vec3_t()
      : x(0.0f), y(0.0f), z(0.0f)
    {}

    LWE_INLINE vec3_t( float x, float y, float z )
      : x(x), y(y), z(z)
    {}

    LWE_INLINE vec3_t( const vec3_t& v )
      : x(v.x), y(v.y), z(v.z)
    {}

    LWE_INLINE vec3_t& operator= ( const vec3_t& v ) {
      x = v.x; y = v.y; z = v.z;
      return *this;
    }

  public: /* operator overloads */
    LWE_INLINE vec3_t operator+ ( const vec3_t& v ) const {
      return vec3_t(x + v.x, y + v.y, z + v.z);
    }

    LWE_INLINE vec3_t operator- ( const vec3_t& v ) const {
      return vec3_t(x - v.x, y - v.y, z - v.z);
    }

    LWE_INLINE vec3_t operator* ( const vec3_t& v ) const {
      return vec3_t(x * v.x, y * v.y, z * v.z);
    }

    LWE_INLINE vec3_t operator/ ( const vec3_t& v ) const {
      return vec3_t(x / v.x, y / v.y, z / v.z);
    }

    LWE_INLINE vec3_t operator* ( float s ) const {
      return vec3_t(x * s, y * s, z * s);
    }

    LWE_INLINE vec3_t operator/ ( float d ) const {
      return vec3_t(x / d, y / d, z / d);
    }

  public: /* methods */
    LWE_INLINE float dot( const vec3_t& v ) const {
      return (x * v.x) + (y * v.y) + (z * v.z);
    }

    LWE_INLINE vec3_t cross( const vec3_t& v ) const {
      return vec3_t(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    LWE_INLINE float length() const {
      return sqrt(dot(*this));
    }

    LWE_INLINE float magnitude() const {
      return sqrt(dot(*this));
    }

    LWE_INLINE vec3_t normalize() const {
      const float im = 1.0f / magnitude();
      return vec3_t(x * im, y * im, z * im);
    }

  public:
    float x, y, z;
} vec3_t;

#endif // _LWE_FOUNDATION_VEC3_H_