// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WINDOW_H_
#define _BUTANE_WINDOW_H_

#include <butane/butane.h>

namespace butane {
  class BUTANE_EXPORT Window abstract {
    __foundation_trait(Window, non_copyable);

    public:
      typedef void (*OnClosedHandler)(
        void* closure,
        Window* window );

      typedef void (*OnResizedHandler)(
        void* closure,
        Window* window );

      typedef void (*OnWindowedHandler)(
        void* closure,
        Window* window );

      typedef void (*OnFullscreenHandler)(
        void* closure,
        Window* window );

    protected:
      static Allocator& allocator();

    protected:
      Window(
        const char* title,
        uint32_t width,
        uint32_t height,
        bool fullscreen );

      virtual ~Window();

    public:
      static Window* open(
        const char* title,
        uint32_t width,
        uint32_t height,
        bool fullscreen = false );

      virtual void close() = 0;

    public:
      virtual void show() = 0;
      virtual void hide() = 0;
      virtual void minimize() = 0;
      virtual void maximize() = 0;
      virtual bool focused() = 0;

      virtual void update() = 0;

    public:
      const String& title() const;

      virtual void set_title(
        const char* title );

      uint32_t width() const;
      uint32_t height() const;

      virtual void set_dimensions(
        const uint32_t width,
        const uint32_t height );

      bool fullscreen() const;

      virtual void set_fullscreen(
        bool fullscreen );

    public:
      void set_on_closed_handler(
        OnClosedHandler handler,
        void* closure = nullptr );

      void set_on_resized_handler(
        OnResizedHandler handler,
        void* closure = nullptr );

      void set_on_windowed_handler(
        OnWindowedHandler handler,
        void* closure = nullptr );

      void set_on_fullscreen_handler(
        OnFullscreenHandler handler,
        void* closure = nullptr );

    private:
      String _title;
      bool _fullscreen;
      uint32_t _width, _height;

    protected:
      struct {
        OnClosedHandler handler;
        void* closure;
      } _on_closed;

      struct {
        OnResizedHandler handler;
        void* closure;
      } _on_resized;

      struct {
        OnWindowedHandler handler;
        void* closure;
      } _on_windowed;

      struct {
        OnFullscreenHandler handler;
        void* closure;
      } _on_fullscreen;
  };
} // butane

#endif // _BUTANE_WINDOW_H_
