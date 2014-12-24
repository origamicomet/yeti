//===-- bitbyte/butane/linakge.h --------------------------------*- C++ -*-===//
//
//  Butane
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@origamicomet.com>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the pre-processor macros that assist with symbol visibility.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_LINKAGE_H_
#define _BITBYTE_BUTANE_LINKAGE_H_

//============================================================================//

/// \def BITBYTE_BUTANE_INTERNAL_LINKAGE
/// \brief Exports symbols for inclusion of Butane via objects.
///
#define BITBYTE_BUTANE_INTERNAL_LINKAGE 0

/// \def BITBYTE_BUTANE_STATIC_LINKAGE
/// \brief Exports symbols for inclusion of Butane via static library.
///
#define BITBYTE_BUTANE_STATIC_LINKAGE 1

/// \def BITBYTE_BUTANE_DYNAMIC_LINKAGE
/// \brief Exports symbols for inclusion of Butane via dynamic library.
///
#define BITBYTE_BUTANE_DYNAMIC_LINKAGE 2

//===----------------------------------------------------------------------===//

/// \def BITBYTE_BUTANE_EXPORT
/// \brief Marks a symbol for export.
///
#ifdef _MSC_VER
  #ifdef BITBYTE_BUTANE_LINKAGE
    #if BITBYTE_BUTANE_LINKAGE == BITBYTE_BUTANE_INTERNAL_LINKAGE
      #define BITBYTE_BUTANE_EXPORT
    #elif BITBYTE_BUTANE_LINKAGE == BITBYTE_BUTANE_STATIC_LINKAGE
      #define BITBYTE_BUTANE_EXPORT
    #elif BITBYTE_BUTANE_DYNAMIC_LINKAGE == BITBYTE_BUTANE_DYNAMIC_LINKAGE
      #ifdef BITBYTE_BUTANE_COMPILING
        #define BITBYTE_BUTANE_EXPORT __declspec(dllexport)
      #else
        #define BITBYTE_BUTANE_EXPORT __declspec(dllimport)
      #endif
    #endif
  #else
    #error ("You need to specify BITBYTE_BUTANE_LINKAGE when building and using tier0!")
  #endif
#else
  #define BITBYTE_BUTANE_EXPORT
#endif

//============================================================================//

#endif // _BITBYTE_BUTANE_LINKAGE_H_

//============================================================================//
