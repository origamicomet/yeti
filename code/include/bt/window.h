// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WINDOW_H_
#define _BUTANE_WINDOW_H_

#include <bt/foundation.h>
#include <bt/config.h>

namespace bt {
  class Window {
    FOUNDATION_NON_COPYABLE(Window);

    public:
      class Mode {
        public:
          enum Value {
            WINDOWED            = 0,
            FULLSCREEN          = 1,
            FULLSCREEN_WINDOWED = 2
          };

        public:
          Mode( Value value );
          Mode( const Mode& mode );
          Mode& operator= ( const Mode& mode );

        public:
          operator Value()
          { return _value; }

        public:
          String to_string() const;

        protected:
          static const char* to_string( Value value );

        protected:
          Value _value;
      };

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
        Window::Mode mode );

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

      FOUNDATION_INLINE Window::Mode window_mode() const
      { return _mode; }

      FOUNDATION_INLINE uintptr_t sys_handle() const
      { return _sys_handle; }

    public:
      String to_string() const;

    private:
      String _title;
      unsigned _width;
      unsigned _height;
      Window::Mode _mode;
      uintptr_t _sys_handle;
      // Array<Event> _events;
  };

  namespace Windows {
    extern void initialize();
    extern void update();
  } // Windows
} // bt

#endif // _BUTANE_WINDOW_H_