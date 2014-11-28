//===-- bitbyte/butane/window.h ---------------------------------*- C++ -*-===//
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
/// \brief Provides a standard interface for managing windows.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_WINDOW_H_
#define _BITBYTE_BUTANE_WINDOW_H_

//============================================================================//

#include "bitbyte/butane/config.h"
#include "bitbyte/butane/foundation.h"
#include "bitbyte/butane/linkage.h"

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

/// \brief
///
typedef struct bitbyte_butane_window_opts {
  /// \brief
  const char *title;
  /// \brief
  uint32_t width;
  /// \brief
  uint32_t height;
} bitbyte_butane_window_opts_t;

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_window_opts_t
  typedef bitbyte_butane_window_opts_t butane_window_opts_t;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
typedef struct bitbyte_butane_window bitbyte_butane_window_t;

#ifdef __BITBYTE_BUTANE_IMPORT__
  /// \copydoc bitbyte_butane_window_t
  typedef bitbyte_butane_window_t butane_window_t;
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
bitbyte_butane_window_t *
bitbyte_butane_window_open(
  const bitbyte_butane_window_opts_t *opts);

/// \def bitbyte_butane_window_open
/// \copydoc bitbyte_butane_window_open
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_open bitbyte_butane_window_open
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_close(
  bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_close
/// \copydoc bitbyte_butane_window_close
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_close bitbyte_butane_window_close
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_update(
  bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_update
/// \copydoc bitbyte_butane_window_update
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_update bitbyte_butane_window_update
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_rename(
  bitbyte_butane_window_t *window,
  const char *title);

/// \def bitbyte_butane_window_rename
/// \copydoc bitbyte_butane_window_rename
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_rename bitbyte_butane_window_rename
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_resize(
  bitbyte_butane_window_t *window,
  uint32_t width,
  uint32_t height);

/// \def bitbyte_butane_window_resize
/// \copydoc bitbyte_butane_window_resize
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_resize bitbyte_butane_window_resize
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_show(
  bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_show
/// \copydoc bitbyte_butane_window_show
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_show bitbyte_butane_window_show
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_hide(
  bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_hide
/// \copydoc bitbyte_butane_window_hide
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_hide bitbyte_butane_window_hide
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_focus(
  bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_focus
/// \copydoc bitbyte_butane_window_focus
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_focus bitbyte_butane_window_focus
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_minimize(
  bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_minimize
/// \copydoc bitbyte_butane_window_minimize
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_minimize bitbyte_butane_window_minimize
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_maximize(
  bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_maximize
/// \copydoc bitbyte_butane_window_maximize
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_maximize bitbyte_butane_window_maximize
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
const char *
bitbyte_butane_window_title(
  const bitbyte_butane_window_t *window);

/// \def bitbyte_butane_window_title
/// \copydoc bitbyte_butane_window_title
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_title bitbyte_butane_window_title
#endif // __BITBYTE_BUTANE_IMPORT__

//===----------------------------------------------------------------------===//

/// \brief
///
extern
BITBYTE_BUTANE_EXPORT
void
bitbyte_butane_window_dimensions(
  const bitbyte_butane_window_t *window,
  uint32_t *width,
  uint32_t *height);

/// \def bitbyte_butane_window_dimensions
/// \copydoc bitbyte_butane_window_dimensions
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_window_dimensions bitbyte_butane_window_dimensions
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

// ...

//===----------------------------------------------------------------------===//

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//

#endif // _BITBYTE_BUTANE_WINDOW_H_

//============================================================================//
