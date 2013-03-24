// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WINDOW_H_
#define _BUTANE_WINDOW_H_

#include <bt/foundation.h>
#include <bt/config.h>
#include <bt/window_mode.h>

namespace bt {
  class Window {
    FOUNDATION_NON_COPYABLE(Window);

    private:
      Window();
      ~Window();

    public:
      static Window* open(
        const char* title,
        unsigned width,
        unsigned height );

      void close();

      void set_title(
        const char* title );

      void set_dimensions(
        unsigned width,
        unsigned height );

      void set_window_mode(
        WindowMode mode );

      void show();
      void hide();

      void minimize();
      void maximize();

    public:
      FOUNDATION_INLINE const char* title() const
      { return _title.to_ptr(); }

      FOUNDATION_INLINE void dimensions(
        unsigned& width,
        unsigned& height ) const
      {
        width = _width;
        height = _height;
      }

      FOUNDATION_INLINE WindowMode window_mode() const
      { return _mode; }

      FOUNDATION_INLINE uintptr_t sys_handle() const
      { return _sys_handle; }

    private:
      String _title;
      unsigned _width;
      unsigned _height;
      WindowMode _mode;
      uintptr_t _sys_handle;
      // Array<Event> _events;
  };

  namespace Windows {
    extern void initialize();
    extern void update();
  } // Windows
} // bt

#endif // _BUTANE_WINDOW_H_