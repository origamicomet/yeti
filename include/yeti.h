//===-- yeti.h ------------------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_H_
#define _YETI_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/core.h"

#include "yeti/resource.h"
#include "yeti/resource_package.h"
#include "yeti/resource_manager.h"
#include "yeti/resource_database.h"
#include "yeti/resource_compiler.h"

#include "yeti/resources/render_config_resource.h"
#include "yeti/resources/entity_resource.h"
#include "yeti/resources/script_resource.h"

#include "yeti/entity.h"
#include "yeti/component.h"
#include "yeti/system.h"

#include "yeti/world.h"

#include "yeti/level.h"

#include "yeti/application.h"
#include "yeti/application/time_step_policy.h"

#include "yeti/window.h"
#include "yeti/input.h"

#include "yeti/task_scheduler.h"

#include "yeti/script.h"
#include "yeti/script/environment.h"
#include "yeti/script/binding.h"
#include "yeti/script/bindings/math_if.h"
#include "yeti/script/bindings/application_if.h"
#include "yeti/script/bindings/window_if.h"
#include "yeti/script/bindings/viewport_if.h"
#include "yeti/script/bindings/keyboard_if.h"
#include "yeti/script/bindings/mouse_if.h"
#include "yeti/script/bindings/world_if.h"
#include "yeti/script/bindings/entity_if.h"
#include "yeti/script/bindings/component_if.h"
#include "yeti/script/bindings/transform_if.h"
#include "yeti/script/bindings/camera_if.h"
#include "yeti/script/bindings/light_if.h"

namespace yeti {

struct Config {
  struct {
    /// \brief Application identifier.
    /// \note Affects environment and paths.
    const char *id;

    /// \brief Publisher identifier.
    /// \note Affects environment and paths.
    const char *publisher;
  } app;

  struct {
    // Absolute path to user settings.
    const char *settings;

    // Absolute path to root directory for saves.
    const char *saves;
  } user;

  struct {
    /// Path to resource database.
    /// \note Resources cannot be loaded or unloaded if `NULL`.
    const char *database;

    /// Toggles automatic loading of resources as required rather than loading
    /// through packages.
    bool autoload;
  } resources;

    struct {
    /// Use raw input, if available, for keyboard input.
    bool raw;
  } keyboard;

  struct {
    /// Use raw input, if available, for mouse input.
    bool raw;
  } mouse;

  struct {
    /// Toggles masking of floating-point exceptions.
    bool floating_point_exceptions;

    /// Toggles (extensive) memory checks.
    bool memory;
  } debug;

  /// \copydoc ::yeti::task_scheduler::Config::workers
  i32 workers;
};

/// Boots the Yeti.
extern YETI_PUBLIC void boot(const Config &config);

/// Gracefully shutsdown the Yeti.
extern YETI_PUBLIC void shutdown();

} // yeti

YETI_BEGIN_EXTERN_C

extern YETI_PUBLIC void __yeti__();

extern YETI_PUBLIC const char *__yeti_copyright__();
extern YETI_PUBLIC const char *__yeti_version__();
extern YETI_PUBLIC yeti_uint32_t __yeti_revision__();

YETI_END_EXTERN_C

#endif // _YETI_H_
