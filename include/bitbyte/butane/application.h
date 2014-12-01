//===-- bitbyte/butane/application.h ----------------------------*- C++ -*-===//
//
//  Butane
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@bitbyte.ca>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the interface that is implemented to "run" the engine.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_APPLICATION_H_
#define _BITBYTE_BUTANE_APPLICATION_H_

//============================================================================//

#include "bitbyte/butane/config.h"
#include "bitbyte/butane/foundation.h"
#include "bitbyte/butane/linkage.h"

//===----------------------------------------------------------------------===//

#include "bitbyte/butane/time_step_policy.h"
#include "bitbyte/butane/window.h"

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

/// \brief Called before entering the update-and-render loop.
///
typedef bool (*bitbyte_butane_application_initialize_fn)(
  struct bitbyte_butane_application *application);

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_application_initialize_fn
  typedef bitbyte_butane_application_initialize_fn butane_application_initialize_fn;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Called every frame to update.
///
typedef void (*bitbyte_butane_application_update_fn)(
  struct bitbyte_butane_application *application,
  const float detla_time);

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_application_update_fn
  typedef bitbyte_butane_application_update_fn butane_application_update_fn;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Called every frame to render.
///
typedef void (*bitbyte_butane_application_render_fn)(
  const struct bitbyte_butane_application *application);

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_application_render_fn
  typedef bitbyte_butane_application_render_fn butane_application_render_fn;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Called before terminating the application.
///
typedef void (*bitbyte_butane_application_shutdown_fn)(
  struct bitbyte_butane_application *application);

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_application_shutdown_fn
  typedef bitbyte_butane_application_shutdown_fn butane_application_shutdown_fn;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Runtime state and control.
///
typedef struct bitbyte_butane_application {
  /// \copdoc bitbyte_butane_application_initialize_fn
  bitbyte_butane_application_initialize_fn initialize;
  /// \copydoc bitbyte_butane_application_update_fn
  bitbyte_butane_application_update_fn update;
  /// \copydoc bitbyte_butane_application_render_fn
  bitbyte_butane_application_render_fn render;
  /// \copydoc bitbyte_butane_application_shutdown_fn
  bitbyte_butane_application_shutdown_fn shutdown;
  /// \brief QQQ.
  bitbyte_butane_time_step_policy_t *time_step_policy;
  /// \brief QQQ.
  bitbyte_butane_window_t *window;
} bitbyte_butane_application_t;

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_application_t
  typedef bitbyte_butane_application_t butane_application_t;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Initializes then enters the update-and-render loop.
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_application_start(
  bitbyte_butane_application_t *application);

/// \def bitbyte_butane_application_start
/// \copydoc bitbyte_butane_application_start
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_application_start bitbyte_butane_application_start
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief The update-and-render loop.
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_application_run(
  bitbyte_butane_application_t *application);

/// \def bitbyte_butane_application_run
/// \copydoc bitbyte_butane_application_run
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_application_run bitbyte_butane_application_run
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Terminates the application.
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_application_quit(
  bitbyte_butane_application_t *application);

/// \def bitbyte_butane_application_quit
/// \copydoc bitbyte_butane_application_quit
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_application_quit bitbyte_butane_application_quit
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Returns the name of the current platform.
///
extern
BITBYTE_BUTANE_EXPORT
const char *
bitbyte_butane_application_platform();

/// \def bitbyte_butane_application_platform
/// \copydoc bitbyte_butane_application_platform
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_application_platform bitbyte_butane_application_platform
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief Returns the name of the current build, i.e., "debug" or "development"
/// or "release".
///
extern
BITBYTE_BUTANE_EXPORT
const char *
bitbyte_butane_application_build();

/// \def bitbyte_butane_application_build
/// \copydoc bitbyte_butane_application_build
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_application_build bitbyte_butane_application_build
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

#ifdef __cplusplus
}
#endif // __cplusplus

//============================================================================//

#ifdef __cplusplus

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

//===----------------------------------------------------------------------===//

/// \copydoc ::bitbyte_butane_application_t
class BITBYTE_BUTANE_EXPORT Application
{
 protected:
  Application();
  Application(const Application &application);
  Application &operator=(const Application &application);
  ~Application();

 public:
  /// \copydoc ::bitbyte_butane_application_initialize_fn
  virtual bool initialize(void);

  /// \copydoc ::bitbyte_butane_application_update_fn
  virtual void update(const float delta_time);

  /// \copydoc ::bitbyte_butane_application_render_fn
  virtual void render(void) const;

  /// \copydoc ::bitbyte_butane_application_shutdown_fn
  virtual void shutdown(void);

 public:
  /// \copydoc ::bitbyte_butane_application_start
  void start(void);

  /// \copydoc ::bitbyte_butane_application_run
  void run(void);

  /// \copydoc ::bitbyte_butane_application_quit
  void quit(void);

 public:
  /// \copydoc ::bitbyte_butane_application_platform
  static const char *platform();

  /// \copydoc ::bitbyte_butane_application_build
  static const char *build();

 public:
  /// \brief QQQ
  ::bitbyte_butane_window_t *window(void) { return __application__.window; }

  /// \brief QQQ
  void set_window(::bitbyte_butane_window_t *window) { __application__.window = window; }

 private:
  bitbyte_butane_application_t __application__;
  Application *__self__;
};

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//

#endif // _BITBYTE_BUTANE_APPLICATION_H_

//============================================================================//
