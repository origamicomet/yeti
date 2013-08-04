// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_WINDOWS_WINDOW_H_
#define _BUTANE_WINDOWS_WINDOW_H_

#include <butane/window.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace butane {
namespace windows {
  class BUTANE_EXPORT Window final
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
      void close() override;

    public:
      void show() override;
      void hide() override;
      void minimize() override;
      void maximize() override;
      bool focused() override;

      void update() override;

    public:
      void set_title(
        const char* title ) override;

      void set_dimensions(
        const uint32_t width,
        const uint32_t height ) override;

      void set_fullscreen(
        bool fullscreen ) override;

    private:
      static void _initialize();

      static LRESULT WINAPI _window_proc(
        HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam );

    public:
      FOUNDATION_INLINE HWND handle() const
      { return _hwnd; }

    private:
      HWND _hwnd;
  };
} // windows
} // butane

#endif // _BUTANE_WINDOWS_WINDOW_H_
