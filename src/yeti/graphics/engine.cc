//===-- yeti/graphics/engine.cc -------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/graphics/engine.h"

namespace yeti {
namespace graphics {

// SMELL(mtwilliams): All this could do with a clean up.

namespace engine {
  static gala_engine_t *underlying_ = NULL;

  static foundation::Mutex *lock_ = foundation::Mutex::create();

  // Signaled whenever some work is available.
  static foundation::Event *work_to_be_done_ = foundation::Event::create();

  struct Commands {
    ::gala_command_buffer_t command_buffer;
    void (*executed)(void *);
    void *executed_ctx;
  };

  static foundation::Queue<Commands> submitted_(foundation::heap(), 64);

  struct BootInfo {
    const Settings *settings;
    foundation::Event *booted;
  };

  static void thread(uintptr_t boot_info_ptr);

  static void boot(const Settings &settings);
  static void execute(const Commands &commands);
}

bool engine::supported(const Backend backend) {
  switch (backend) {
    case engine::D3D11:
    case engine::D3D12:
      #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS
        return true;
      #else
        return false;
      #endif

    case engine::OPENGL:
      #if YETI_PLATFORM == YETI_PLATFORM_WINDOWS || \
          YETI_PLATFORM == YETI_PLATFORM_MAC || \
          YETI_PLATFORM == YETI_PLATFORM_LINUX
        return true;
      #else
        return false;
      #endif
  }

  return false;
}

void engine::initialize(const Settings &settings) {
  yeti_assert(supported(settings.backend));

  BootInfo boot_info;
  boot_info.settings = &settings;
  boot_info.booted = foundation::Event::create();

  foundation::Thread::Options thread_opts;
  sprintf(&thread_opts.name[0], "Render");
  thread_opts.affinity = ~0ull;
  thread_opts.stack_size = 0x100000 /* 1MiB */;

  foundation::Thread::spawn(&engine::thread, (uintptr_t)&boot_info, &thread_opts)->detach();

  boot_info.booted->wait();

  boot_info.booted->destroy();
}

void engine::shutdown() {
  // Not implemented yet.
  YETI_TRAP();
}

void engine::submit(const ::gala_command_buffer_t *command_buffer,
                    void (*executed)(void *),
                    void *executed_ctx) {
  {
    YETI_SCOPED_LOCK(lock_);

    Commands commands;
    commands.command_buffer.start = command_buffer->start;
    commands.command_buffer.end = command_buffer->end;
    commands.command_buffer.current = command_buffer->current;
    commands.executed = executed;
    commands.executed_ctx = executed_ctx;

    submitted_.push(commands);
  }

  work_to_be_done_->signal();
}

::gala_engine_t *engine::underlying() {
  return underlying_;
}

void engine::thread(uintptr_t boot_info_ptr) {
  const BootInfo *boot_info = (const BootInfo *)boot_info_ptr;

  engine::boot(*boot_info->settings);
  boot_info->booted->signal();

  for (;;) {
    work_to_be_done_->wait();

    Commands commands[64];
    size_t count = 0;

    {
      YETI_SCOPED_LOCK(lock_);

      while (submitted_.pop(&commands[count]))
        ++count;
    }

    for (size_t processed = 0; processed < count; ++processed)
      engine::execute(commands[processed]);
  }
}

void engine::boot(const Settings &settings) {
  gala_engine_creation_params_t engine_creation_params; {
    engine_creation_params.type = GALA_ENGINE_TYPE_HARDWARE;

    engine_creation_params.flags = 0;

  #if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG || \
      YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
    engine_creation_params.flags |= GALA_ENGINE_DEBUG;
  #endif

    if (settings.fast_and_loose)
      engine_creation_params.flags |= GALA_ENGINE_FAST_AND_LOOSE;
  }

  switch (settings.backend) {
    case engine::D3D11:
      ::gala_d3d11_init();
      underlying_ = ::gala_d3d11_create_and_init_engine(&engine_creation_params);
      break;

    case engine::OPENGL:
      ::gala_ogl_init();
      underlying_ = ::gala_ogl_create_and_init_engine(&engine_creation_params);
      break;
  }
}

void engine::execute(const Commands &commands) {
  ::gala_engine_execute(underlying_, &commands.command_buffer);

  if (commands.executed)
    commands.executed(commands.executed_ctx);
}

} // graphics
} // yeti
