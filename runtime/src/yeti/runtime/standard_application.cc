//===-- yeti/runtime/standard_application.cc ------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti/runtime/standard_application.h"

#include "yeti/runtime/manifest.h"

#include <stdlib.h>
#include <stdio.h>

// TODO(mtwilliams): Respect user settings.

namespace yeti {
namespace runtime {

StandardApplication::StandardApplication(const Manifest *manifest)
  : manifest_(manifest) {
}

StandardApplication::~StandardApplication() {
}

bool StandardApplication::startup() {
  // Start logging to the console and to a file.
  this->start_logging_to_console();
  this->start_logging_to_file();

  // Set the appropriate logging level based on build configuration.
  this->set_appropriate_logging_level();

  // Can't say we didn't tell ya'.
  this->log_copyright_notices();

  // Engine and content revisions are logged to aid when debugging and filing
  // crash reports.
  this->log_pertinent_information_about_build();

  // Hardware information is logged too, for the same reasons.
  this->log_pertinent_information_about_system();

  // TODO(mtwilliams): Load the renderer configuration specified by manfiest.
#if 0
  const Resource::Type *render_config_resource_type = resource_manager::type_from_name("render_config");
  const Resource::Type::Id render_config_resource_type_id = resource_manager::id_from_type(render_config_resource_type);
  const Resource::Id render_config_resource_id = Resource::id_from_type_and_name(render_config_resource_type_id, manifest->graphics.config);
  RenderConfigResource *render_config_resource = (RenderConfigResource *)resource_manager::load(render_config_resource_id);
  while (render_config_resource->state() != Resource::LOADED);
#endif

  // TODO(mtwilliams): Configure rendering.
#if 0
  this->renderer_.configure(render_config_resource);
#endif

  // Create the main window and default viewport.
  this->create_main_window_and_default_viewport();

  // Default to a variable time-step policy.
  TimeStepPolicy::Description time_step_policy_desc;
  time_step_policy_desc.type = yeti::TimeStepPolicy::VARIABLE;
  this->time_step_policy_ = yeti::TimeStepPolicy::create(time_step_policy_desc);

  // Expose ourself to Lua. Hehe.
  this->expose_to_lua();

  // Load boot package and script.
  this->load_boot_package_and_script();

  // Boot!
  if (!this->script_.call("startup", 0))
    return false;

  // Conceal boot time by only showing window after booting.
  windows_[0]->show();

  return true;
}

void StandardApplication::update(const f32 delta_time) {
  this->script_.environment()->reset();

  if (!this->script_.call("update", 1, Script::T_FLOAT, delta_time))
    this->pause();
}

void StandardApplication::render() {
  this->script_.environment()->reset();

  if (!this->script_.call("render", 0))
    this->pause();
}

void StandardApplication::shutdown() {
  this->script_.call("shutdown", 0);

  yeti::shutdown();
}

void StandardApplication::start_logging_to_console() const {
  // TODO(mtwilliams): Log to stdout and stderr.
}

void StandardApplication::start_logging_to_file() const {
  // TODO(mtwilliams): Log to `YYYY-MM-DD-HH-MM-SS.log`.
}

void StandardApplication::set_appropriate_logging_level() const {
  // TODO(mtwilliams): Refactor start_logging_to_console() and
  // start_logging_to_file() into Console/Log and use set_logging_level().
}

void StandardApplication::log_copyright_notices() const {
  log::printf(YETI_LOG_APP, log::INFO,
    "                            __ __     _   _                            \n"
    "                           |  |  |___| |_|_|                           \n"
    "                           |_   _| -_|  _| |                           \n"
    "                             |_| |___|_| |_|                           \n\n"
  );

  log::printf(YETI_LOG_APP, log::INFO,
    "             %s             \n\n",
    __yeti_copyright__()
  );
}

void StandardApplication::log_pertinent_information_about_build() const {
  log::printf(YETI_LOG_APP, log::INFO,
    "Runtime\n"
    " platform = %s\n"
    " architecture = %s\n"
    " build = %s\n\n",
    platform(),
    architecture(),
    build()
  );

  // TODO(mtwilliams): Determine content version.
  log::printf(YETI_LOG_APP, log::INFO,
    "Version\n"
    " engine = %s\n"
    " content = %s\n\n",
    __yeti_version__(),
    "unknown"
  );
}

void StandardApplication::log_pertinent_information_about_system() const {
  // TODO(mtwilliams): Log CPU and GPU information.
}

void StandardApplication::create_main_window_and_default_viewport() {
  Window::Description wd;

  wd.title = manifest_->app.name;

  wd.dimensions.width = manifest_->graphics.width;
  wd.dimensions.height = manifest_->graphics.height;

  wd.resizeable = false;

  // Quits application. See `Application::window_event_handler_`.
  wd.closable = true;

  this->open_a_window(wd);
}

void StandardApplication::expose_to_lua() {
  application_if::expose(&this->script_, this);
  math_if::expose(&this->script_);
  window_if::expose(&this->script_);
  viewport_if::expose(&this->script_);
  keyboard_if::expose(&this->script_);
  mouse_if::expose(&this->script_);
  world_if::expose(&this->script_);
  transform_if::expose(&this->script_);
  camera_if::expose(&this->script_);
  light_if::expose(&this->script_);
}

void StandardApplication::load_boot_package_and_script() {
  // TODO(mtwilliams): Load the boot package specified by manfiest.
#if 0
  const Resource::Type *package_resource_type = resource_manager::type_from_name("package");
  const Resource::Type::Id package_resource_type_id = resource_manager::id_from_type(package_resource_type);
  const Resource::Id boot_package_id = Resource::id_from_type_and_name(package_resource_type_id, manifest_->boot.package);

  ResourcePackage *package = (ResourcePackage *)resource_manager::load(boot_package_id);

  // Block until boot resources are loaded.
  package->flush();
#endif

  const Resource::Type *script_resource_type =
    resource_manager::type_from_name("script");

  const Resource::Type::Id script_resource_type_id =
    resource_manager::id_from_type(script_resource_type);

  const Resource::Id boot_script_id =
    Resource::id_from_type_and_name(script_resource_type_id,
                                    manifest_->boot.script);

  ScriptResource *script =
    (ScriptResource *)resource_manager::load(boot_script_id);

#if 1
  while (script->state() != Resource::LOADED);
#else
  // Boot script must be part of boot package.
  yeti_assert_development(script->state() == Resource::LOADED);
#endif

  this->script_.inject(script);
}

} // runtime
} // yeti
