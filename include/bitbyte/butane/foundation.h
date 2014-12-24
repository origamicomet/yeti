//===-- bitbyte/butane/foundation.h -----------------------------*- C++ -*-===//
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
/// \brief Imports Foundation.
///
//===----------------------------------------------------------------------===//

#ifndef _BITBYTE_BUTANE_FOUNDATION_H_
#define _BITBYTE_BUTANE_FOUNDATION_H_

//============================================================================//

#include "bitbyte/butane/config.h"

//===----------------------------------------------------------------------===//

#define __BITBYTE_FOUNDATION_IMPORT__

#include "bitbyte/foundation/tier0.h"
#include "bitbyte/foundation/tier1.h"
#include "bitbyte/foundation/tier2.h"
#include "bitbyte/foundation/tier3.h"
#include "bitbyte/foundation/tier4.h"

//============================================================================//

/// \def butane_assert
/// \copydoc bitbyte_butane_assert
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assert bitbyte_butane_assert
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assert
/// \copydoc bitbyte_foundation_assert
#define bitbyte_butane_assert bitbyte_foundation_assert

//===----------------------------------------------------------------------===//

/// \def butane_assertf
/// \copydoc bitbyte_butane_assertf
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assertf bitbyte_butane_assertf
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assertf
/// \copydoc bitbyte_foundation_assertf
#define bitbyte_butane_assertf bitbyte_foundation_assertf

//===----------------------------------------------------------------------===//

/// \def butane_assertf_always
/// \copydoc bitbyte_butane_assertf_always
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assertf_always bitbyte_butane_assertf_always
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assertf_always
/// \copydoc bitbyte_foundation_assertf
#define bitbyte_butane_assertf_always(_Format, ...) \
  bitbyte_foundation_assertf(0, _Format, ##__VA_ARGS__)

//===----------------------------------------------------------------------===//

/// \def butane_assert_debug
/// \copydoc bitbyte_butane_assert_debug
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assert_debug bitbyte_butane_assert_debug
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assert_debug
/// \copydoc bitbyte_foundation_assert
#if BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_DEBUG
  #define bitbyte_butane_assert_debug bitbyte_foundation_assert
#else
  #define bitbyte_butane_assert_debug
#endif

/// \def butane_assertf_debug
/// \copydoc bitbyte_butane_assertf_debug
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assertf_debug bitbyte_butane_assertf_debug
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assertf_debug
/// \copydoc bitbyte_foundation_assertf
#if BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_DEBUG
  #define bitbyte_butane_assertf_debug bitbyte_foundation_assertf
#else
  #define bitbyte_butane_assertf_debug
#endif

//===----------------------------------------------------------------------===//

/// \def butane_assert_development
/// \copydoc bitbyte_butane_assert_development
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assert_development bitbyte_butane_assert_development
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assert_development
/// \copydoc bitbyte_foundation_assert
#if BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_DEVELOPMENT
  #define bitbyte_butane_assert_development bitbyte_foundation_assert
#else
  #define bitbyte_butane_assert_development
#endif

/// \def butane_assertf_development
/// \copydoc bitbyte_butane_assertf_development
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assertf_development bitbyte_butane_assertf_development
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assertf_development
/// \copydoc bitbyte_foundation_assertf
#if BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_DEVELOPMENT
  #define bitbyte_butane_assertf_development bitbyte_foundation_assertf
#else
  #define bitbyte_butane_assertf_development
#endif

//===----------------------------------------------------------------------===//

/// \def butane_assert_release
/// \copydoc bitbyte_butane_assert_release
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assert_release bitbyte_butane_assert_release
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assert_release
/// \copydoc bitbyte_foundation_assert
#if BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_RELEASE
  #define bitbyte_butane_assert_release bitbyte_foundation_assert
#else
  #define bitbyte_butane_assert_release
#endif

/// \def butane_assertf_release
/// \copydoc bitbyte_butane_assertf_release
#ifdef __BITBYTE_BUTANE_IMPORT__
  #define butane_assertf_release bitbyte_butane_assertf_release
#endif // __BITBYTE_BUTANE_IMPORT__

/// \def bitbyte_butane_assertf_release
/// \copydoc bitbyte_foundation_assertf
#if BITBYTE_BUTANE_CONFIGURATION == BITBYTE_BUTANE_CONFIGURATION_RELEASE
  #define bitbyte_butane_assertf_release bitbyte_foundation_assertf
#else
  #define bitbyte_butane_assertf_release
#endif

//============================================================================//

#endif // _BITBYTE_BUTANE_FOUNDATION_H_

//============================================================================//
