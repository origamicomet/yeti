// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <bt/window.h>

namespace bt {
  Window::Mode::Mode( Value value )
    : _value(value)
  {
  }

  Window::Mode::Mode( const Window::Mode& mode )
    : _value(mode._value)
  {
  }

  Window::Mode& Window::Mode::operator= ( const Window::Mode& mode )
  {
    _value = mode._value;
    return *this;
  }

  String Window::Mode::to_string() const
  {
    return String(Allocator::scratch(),
      "#<Window::Mode:%016" PRIx64 "> "
      "[%s]",
      (uint64_t)this,
      to_string(_value));
  }

  const char* Window::Mode::to_string( Value value )
  {
    switch (value) {
      case WINDOWED:
        return "Windowed";
      case FULLSCREEN:
        return "Fullscreen";
      case FULLSCREEN_WINDOWED:
        return "Fullscreen (Windowed)";
      default:
        return "Unknown";
    }
  }

  String Window::to_string() const
  {
    return String(Allocator::scratch(),
      "#<Window:%016" PRIx64 "> "
      "[title=`%s`,width=%u,height=%u,mode=%s,sys_handle=%016" PRIx64 "]",
      (uint64_t)this,
      _title.to_ptr(),
      _width, _height,
      _mode.to_string().to_ptr(),
      (uint64_t)_sys_handle);
  }
} // bt