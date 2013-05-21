// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WIN32_WINDOW_H_
#define _BUTANE_WIN32_WINDOW_H_

#include <butane/window.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace butane {
namespace win32 {
  class Window final
    : public butane::Window
  {
    __foundation_trait(Window, non_copyable);

    private:
      friend butane::Window* butane::Window::open(
        const char* title,
        uint32_t width,
        uint32_t height,
        bool fullscreen );

    public:
      Window(
        const char* title,
        uint32_t width,
        uint32_t height,
        bool fullscreen );

      ~Window();

    public:
      void close();

    public:
      void show();
      void hide();
      void minimize();
      void maximize();
      
      void update();

    public:
      const String& title() const;

      void set_title(
        const char* title );

      bool fullscreen() const;

      void set_fullscreen(
        bool fullscreen );

    private:
      static void _initialize();

      static LRESULT WINAPI _window_proc(
        HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam );

    private:
      HWND _hwnd;
  };
} // win32
} // butane

#endif // _BUTANE_WIN32_WINDOW_H_
