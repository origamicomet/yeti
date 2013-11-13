/* ========================================================================== */
/*                                                                            */
/* This file is part of Butane.                                               */
/*                                                                            */
/* Author(s):                                                                 */
/*                                                                            */
/*   Michael Williams <devbug@bitbyte.ca>                                     */
/*                                                                            */
/* See LICENSE.md for licensing information.                                  */
/*                                                                            */
/* ========================================================================== */
/*! @file include/butane/foundation.h
     Provides foundational algorithims, data structures, etc.                 */
/* ========================================================================== */

#ifndef _BUTANE_FOUNDATION_H_
#define _BUTANE_FOUNDATION_H_

#include <butane/config.h>

#include <foundation.h>

/* ========================================================================== */
/*  Foundation:                                                               */
/*   * Assertions:                                                            */
/*      * Independent;                                                        */
/*      * Dependent.                                                          */
/*   * Export.                                                                */
/* ========================================================================== */

/* ========================================================================== */
/*  Assertions:                                                               */
/* ========================================================================== */

/* ========================================================================== */
/*  Assertions > Independent:                                                 */
/* ========================================================================== */

/*! @copydoc fnd_assertf_ */
#define butane_assertf_ fnd_assertf_

/*! @copydoc fnd_assert_ */
#define butane_assert_ fnd_assert_

/* ========================================================================== */
/*  Assertions > Dependent:                                                   */
/* ========================================================================== */

/*! @def butane_assertf
  butane_assert_f If, and only if, `_Level` is <= BUTANE_CONFIGURATION. */
#define butane_assertf(_Level, _Condition, _Format, ...) \
  butane_assertf_##_Level(_Condition, _Format, ## __VA_ARGS__)

#ifdef BUTANE_PARANOID
  #define butane_assertf_paranoid(_Condition, _Format, ...) \
    butane_assertf_(_Condition, _Format, ## __VA_ARGS__)
#else
  #define butane_assertf_paranoid(_Condition, _Format, ...) \
    do { (void)sizeof((_Condition)); } while (0)
#endif

#if (BUTANE_CONFIGURATION <= BUTANE_DEBUG)
  #define butane_assertf_debug(_Condition, _Format, ...) \
    butane_assertf_(_Condition, _Format, ## __VA_ARGS__)
#else
  #define butane_assertf_debug(_Condition, _Format, ...) \
    do { (void)sizeof((_Condition)); } while (0)
#endif

#if (BUTANE_CONFIGURATION <= BUTANE_DEVELOPMENT)
  #define butane_assertf_development(_Condition, _Format, ...) \
    butane_assertf_(_Condition, _Format, ## __VA_ARGS__)
#else
  #define butane_assertf_development(_Condition, _Format, ...) \
    do { (void)sizeof((_Condition)); } while (0)
#endif

#if (BUTANE_CONFIGURATION <= BUTANE_RELEASE)
  #define butane_assertf_release(_Condition, _Format, ...) \
    butane_assertf_(_Condition, _Format, ## __VA_ARGS__)
#else
  #define butane_assertf_release(_Condition, _Format, ...) \
    do { (void)sizeof((_Condition)); } while (0)
#endif

/*! @def butane_assert
  butane_assert_ If, and only if, `_Level` is <= BUTANE_CONFIGURATION. */
#define butane_assert(_Level, _Condition) \
  butane_assertf(_Level, _Condition, \
    "An assertion failed!\n" \
    "\n" \
    "  '%s' in %s on line %u\n" \
    "\n", \
    fnd_stringificate(_Condition), \
    fnd_stringificate(__FILE__), \
    ((unsigned)__LINE__) \
  )

/* ========================================================================== */
/*  Export:                                                                   */
/* ========================================================================== */

/*! @def BUTANE_API
  ... */
#if (BUTANE_LINKAGE == BUTANE_LINK_STATICALLY)
  #define BUTANE_API
#elif (BUTANE_LINKAGE == BUTANE_LINK_DYNAMICALLY)
  #if (FND_COMPILER == FND_COMPILER_MSVC)
    #if defined(BUTANE_COMPILING)
      #define BUTANE_API __declspec(dllexport)
    #else
      #define BUTANE_API __declspec(dllimport)
    #endif
  #else
    #define BUTANE_API
  #endif
#endif

/* ========================================================================== */

#endif /* _BUTANE_FOUNDATION_H_ */
