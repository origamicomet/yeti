// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/application/run.h>

#include <butane/butane.h>
#include <butane/script.h>
#include <butane/lua/script.h>
#include <butane/resource.h>
#include <butane/resource/config.h>
#include <butane/window.h>
#include <butane/math.h>

namespace butane {
namespace Application {
  static void on_compile_log(
    void* closure,
    const char* format, ... )
  {
  }

  static void on_window_closed(
    void* closure,
    Window* window )
  {
    window->close();
    Application::quit();
  }

  void run(
    const Array<const char*>& args )
  {
    const Resource::Compiler::Status result = Resource::Compiler::compile(
      "data", "data_src",
      "data_src/manifest.config",
      &on_compile_log);

    ConfigResource* manifest = 
      (ConfigResource*)Resource::load(ConfigResource::type, "manifest");
    manifest->reference();

    Vec2f dimensions;
    if (!manifest->find("application.window.dimensions", dimensions))
      dimensions = Vec2f(1280.0f, 720.0f);

    Window* window =
      Window::open("butane", (uint32_t)dimensions.x, (uint32_t)dimensions.y);

    window->set_on_closed_handler(&on_window_closed);
    window->show();

    manifest->dereference();

    while (true) {
      window->update();
    }
  }
} // Application
} // butane
