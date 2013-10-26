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
/*! @file include/butane/assert.h
     Provides configuration dependent and independent assertion macros.       */
/* ========================================================================== */

#ifndef _BUTANE_ASSERT_H_
#define _BUTANE_ASSERT_H_

#include <butane/config.h>
#include <foundation/assert.h>

/* ========================================================================== */
/*  Assert:                                                                   */
/*   * Independent;                                                           */
/*   * Dependent.                                                             */
/* ========================================================================== */

/* ========================================================================== */
/*  Independent:                                                              */
/* ========================================================================== */

/*! @def bt_assertf_
  @copydoc fnd_assertf_ */
#define bt_assertf_ fnd_assertf_

/*! @def bt_assert_
  @copydoc fnd_assert_ */
#define bt_assert_ fnd_assert_

/* ========================================================================== */
/*  Dependent:                                                                */
/* ========================================================================== */

/*! @def bt_assertf
  @copydoc fnd_assertf */
#define bt_assertf fnd_assertf

/*! @def bt_assert
  @copydoc fnd_assert */
#define bt_assert fnd_assert

#endif /* _BUTANE_ASSERT_H_ */
