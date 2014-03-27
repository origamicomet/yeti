//=== bitbyte/butane/export.h ================================================//
//                                                                            //
//  Butane                                                                    //
//                                                                            //
//  This file is distributed under the terms described in LICENSE.            //
//                                                                            //
//  Author(s):                                                                //
//   Michael Williams <mwilliams@bitbyte.ca>                                  //
//                                                                            //
//===----------------------------------------------------------------------===//

/// @file bitbyte/butane/export.h
/// @brief Defines BITBYTE_BUTANE_EXPORT based on configuration.
///

#ifndef _BITBYTE_BUTANE_EXPORT_H_
#define _BITBYTE_BUTANE_EXPORT_H_

//===----------------------------------------------------------------------===//
// Pre-processor defines
//

/// @def BITBYTE_BUTANE_LINK_STATICALLY
/// @breif Linking to Butane statically, e.g., using libbutane.a.
///
#define BITBYTE_BUTANE_LINK_STATICALLY 1

/// @def BITBYTE_BUTANE_LINK_DYNAMICALLY
/// @brief Linking to Butane dynamically, e.g., using libbutane.so.
///
#define BITBYTE_BUTANE_LINK_DYNAMICALLY 2

/// @def BITBYTE_BUTANE_LINKAGE
/// @brief Specifies if Butane is being linked to statically, or dynamically.
#ifndef BITBYTE_BUTANE_LINKAGE
  #error ("Please specify how you are linking to Butane by defining `BITBYTE_BUTANE_LINKAGE`.")
#endif

/// @def BITBYTE_BUTANE_EXPORT
/// @brief ...
///
#if (BITBYTE_BUTANE_LINKAGE == BITBYTE_BUTANE_LINK_STATICALLY)
  #define BITBYTE_BUTANE_EXPORT
#elif (BITBYTE_BUTANE_LINKAGE == BITBYTE_BUTANE_LINK_DYNAMICALLY)
  #if defined(_MSC_VER)
    /// @def BITBYTE_BUTANE_COMPILING
    /// @brief ...
    ///
    #if defined(BITBYTE_BUTANE_COMPILING)
      #define BITBYTE_BUTANE_EXPORT __declspec(dllexport)
    #else
      #define BITBYTE_BUTANE_EXPORT __declspec(dllimport)
    #endif
  #else
    #define BITBYTE_BUTANE_EXPORT
  #endif
#endif

#endif // _BITBYTE_BUTANE_EXPORT_H_
