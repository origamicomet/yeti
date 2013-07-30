// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_VIEWPORT_H_
#define _BUTANE_VIEWPORT_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Viewport final {
    public:
      Viewport(
        const uint32_t top,
        const uint32_t left,
        const uint32_t bottom,
        const uint32_t right
      ) : _top(top), _left(left)
        , _bottom(bottom), _right(right)
      {
      }

      Viewport(
        const Viewport& v
      ) : _top(v._top), _left(v._left)
        , _bottom(v._bottom), _right(v._right)
      {}

      Viewport& operator=(
        const Viewport& v )
      {
        _top = v._top;
        _left = v._left;
        _bottom = v._bottom;
        _right = v._right;
        return *this;
      }

    public:
      FOUNDATION_INLINE uint32_t width() const
      { return (_right - _left); }

      FOUNDATION_INLINE uint32_t height() const
      { return (_bottom - _top); }

      FOUNDATION_INLINE uint32_t top() const
      { return _top; }

      FOUNDATION_INLINE uint32_t left() const
      { return _left; }

      FOUNDATION_INLINE uint32_t bottom() const
      { return _bottom; }

      FOUNDATION_INLINE uint32_t right() const
      { return _right; }

      FOUNDATION_INLINE void update(
        const uint32_t top,
        const uint32_t left,
        const uint32_t bottom,
        const uint32_t right )
      {
        _top = top; _left = left;
        _bottom = bottom; _right = right;
      }

    private:
      uint32_t _top, _left;
      uint32_t _bottom, _right;
  };
} // butane

#endif // _BUTANE_VIEWPORT_H_
