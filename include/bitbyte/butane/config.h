//=== bitbyte/butane/config.h ================================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/butane/config.h
/// @brief Configuration.
///

#ifndef _BITBYTE_BUTANE_CONFIG_H_
#define _BITBYTE_BUTANE_CONFIG_H_

/// @def BITBYTE_BUTANE_PARANOID
/// @brief Specifies how paranoid Butane is; if defined Butane will
///  perform more sanity checks.
///

/// @def BITBYTE_BUTANE_DEBUG
/// @brief Enables debugging (and higher) checks and profiling.
///
#define BITBYTE_BUTANE_DEBUG 1

/// @def BITBYTE_BUTANE_DEVELOPMENT
/// @brief Enables development (and higher) checks and profiling.
///
#define BITBYTE_BUTANE_DEVELOPMENT 2

/// @def BITBYTE_BUTANE_RELEASE
/// @brief Enables release checks.
///
#define BITBYTE_BUTANE_RELEASE 3

/// @def BITBYTE_BUTANE_CONFIGURATION
/// @brief Specifies how "loose and fast" Butane is.
///
#ifndef BITBYTE_BUTANE_CONFIGURATION
  #error ("Please specify a configuration by defining `BITBYTE_BUTANE_CONFIGURATION`.")
#endif

#endif // _BITBYTE_BUTANE_CONFIG_H_
