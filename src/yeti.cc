//===-- yeti.cc -----------------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti.h"

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #include <float.h>
  #include <crtdbg.h>
#endif

namespace yeti {

namespace {
  static void default_assertion_handler(const core::Assertion &assertion, void *) {
    core::logf(core::log::GENERAL, core::log::FATAL,
               "Assertion failed in %s on %u!\n",
               assertion.location.file,
               (unsigned)assertion.location.line);

    if (assertion.predicate) {
      core::logf(core::log::GENERAL, core::log::FATAL,
                 "Predicate: %s",
                 assertion.predicate);
    }

    if (assertion.reason) {
      core::logf(core::log::GENERAL, core::log::FATAL,
                 "Reason: %s",
                 assertion.reason);
    }
  }
}

void boot(const Config &config) {
  core::set_assertion_handler(&default_assertion_handler);

  if (config.debug.floating_point_exceptions) {
  #if YETI_COMPILER == YETI_COMPILER_MSVC
    static const unsigned int floating_point_exceptions = _EM_INVALID |
                                                          _EM_OVERFLOW |
                                                          _EM_UNDERFLOW |
                                                          _EM_ZERODIVIDE;

    // Enable floating point exceptions.
    _controlfp_s(NULL, ~floating_point_exceptions, _MCW_EM);
  #endif
  }

  if (config.debug.memory) {
  #if YETI_COMPILER == YETI_COMPILER_MSVC
    int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    // Check memory integrity every time memory is allocated or freed.
    flags |= _CRTDBG_CHECK_ALWAYS_DF;

    // Check for memory leaks at program exit.
    flags |= _CRTDBG_LEAK_CHECK_DF;

    _CrtSetDbgFlag(flags);
  #endif
  }

  if (config.resources.database) {
    resource_manager::Config resource_manager_config;
    resource_manager_config.database = config.resources.database;
    resource_manager_config.autoload = config.resources.autoload;
    resource_manager::initialize(resource_manager_config);
  }

  resource::register_a_type(RenderConfigResource::type());
  resource::register_a_type(EntityResource::type());
  resource::register_a_type(ScriptResource::type());

  task_scheduler::Config task_scheduler_config;
  task_scheduler_config.workers = config.workers;
  task_scheduler::initialize(task_scheduler_config);
}

void shutdown() {
  // Schedule any remaining work and stop worker threads.
  task_scheduler::shutdown();
}

} // yeti

YETI_BEGIN_EXTERN_C

void __yeti__(void) {
  // We export a `__yeti__` symbol for sanity checks.
}

const char *__yeti_copyright__() {
  // Refer to our Rybfile for details.
  return __YETI_COPYRIGHT__;
}

const char *__yeti_version__() {
  // Refer to our Rybfile for details.
  return __YETI_VERSION__;
}

yeti_uint32_t __yeti_revision__() {
  // Refer to our Rybfile for details.
  return __YETI_REVISION__;
}

YETI_END_EXTERN_C
