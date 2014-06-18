//===-- bitbyte/butane/config.h ---------------------------------*- C++ -*-===//
//
//  Butane
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
/// \brief Configuration.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_CONFIG_H_
#define _BITBYTE_BUTANE_CONFIG_H_

//============================================================================//

/// \def BITBYTE_BUTANE_PARANOID
/// \brief Enables paranoid checks.
///

/// \def BITBYTE_BUTANE_DEBUG
/// \brief Enables debugging, development, and release features.
///
#define BITBYTE_BUTANE_DEBUG 1

/// \def BITBYTE_BUTANE_DEVELOPMENT
/// \brief Enables development and release features.
///
#define BITBYTE_BUTANE_DEVELOPMENT 2

/// \def BITBYTE_BUTANE_RELEASE
/// \brief Enables release features.
///
#define BITBYTE_BUTANE_RELEASE 3

/// \def BITBYTE_BUTANE_CONFIGURATION
/// \brief Specifies how "loose and fast" Butane is.
///
#ifndef BITBYTE_BUTANE_CONFIGURATION
  #error ("Please specify a configuration by defining `BITBYTE_BUTANE_CONFIGURATION`.")
#endif

//============================================================================//

#endif // _BITBYTE_BUTANE_CONFIG_H_

//============================================================================//
