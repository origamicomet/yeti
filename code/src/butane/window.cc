// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/window.h>

namespace butane {
  Allocator& Window::allocator() {
    static ProxyAllocator allocator("windows", Allocators::heap());
    return allocator;
  }

  Window::Window(
    const char* title,
    uint32_t width,
    uint32_t height,
    bool fullscreen
  ) : _title(Window::allocator(), title)
    , _width(width)
    , _height(height)
    , _fullscreen(fullscreen)
  {
    _on_closed.handler = nullptr;
    _on_closed.closure = nullptr;
    _on_resized.handler = nullptr;
    _on_resized.closure = nullptr;
    _on_windowed.handler = nullptr;
    _on_windowed.closure = nullptr;
    _on_fullscreen.handler = nullptr;
    _on_fullscreen.closure = nullptr;
  }

  Window::~Window()
  {
  }

  const String& Window::title() const
  { return _title; }

  void Window::set_title(
    const char* title )
  { _title = title; }

  uint32_t Window::width() const
  { return _width; }

  uint32_t Window::height() const
  { return _height; }

  void Window::set_dimensions(
    const uint32_t width,
    const uint32_t height )
  {
    _width = width; _height = height;

    if (_on_resized.handler)
      _on_resized.handler(_on_resized.closure, this);
  }

  bool Window::fullscreen() const
  { return _fullscreen; }

  void Window::set_fullscreen(
    bool fullscreen )
  {
    if (_fullscreen != fullscreen) {
      if (fullscreen) {
        if (_on_fullscreen.handler)
          _on_fullscreen.handler(_on_fullscreen.closure, this);
      } else {
        if (_on_windowed.handler)
          _on_windowed.handler(_on_windowed.closure, this);
      }
    }

    _fullscreen = fullscreen;
  }

  void Window::set_on_closed_handler(
    OnClosedHandler handler,
    void* closure )
  {
    _on_closed.handler = handler;
    _on_closed.closure = closure;
  }

  void Window::set_on_resized_handler(
    OnResizedHandler handler,
    void* closure )
  {
    _on_resized.handler = handler;
    _on_resized.closure = closure;
  }

  void Window::set_on_windowed_handler(
    OnWindowedHandler handler,
    void* closure )
  {
    _on_windowed.handler = handler;
    _on_windowed.closure = closure;
  }

  void Window::set_on_fullscreen_handler(
    OnFullscreenHandler handler,
    void* closure )
  {
    _on_fullscreen.handler = handler;
    _on_fullscreen.closure = closure;
  }
} // butane

