//===-- bitbyte/butane/console.h --------------------------------*- C++ -*-===//
//
//  Butane, a data-driven game engine.
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mwilliams@bitbyte.ca>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Exposes a logger that writes to stdout and connected consoles.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_CONSOLE_H_
#define _BITBYTE_BUTANE_CONSOLE_H_

//===----------------------------------------------------------------------===//

#include <bitbyte/butane/config.h>
#include <bitbyte/butane/export.h>
#include <bitbyte/butane/foundation.h>

//===----------------------------------------------------------------------===//

namespace bitbyte {
namespace butane {

/// \brief Represents stdout and connected consoles.
///
namespace Console {
  //===--------------------------------------------------------------------===//

  /// \brief Installs the console-proxying logger.
  ///
  extern BITBYTE_BUTANE_EXPORT void install();

  //===--------------------------------------------------------------------===//
}

} // butane
} // bitbyte

//===----------------------------------------------------------------------===//

#endif // _BITBYTE_BUTANE_CONSOLE_H_

//===----------------------------------------------------------------------===//
