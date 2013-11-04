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
/*! @file include/butane/config.h
     Documents a collection of pre-processor defines used for the
     configuration of Butanes's compile-, link-, and runtime behaviour.       */
/* ========================================================================== */

#ifndef _BUTANE_CONFIG_H_
#define _BUTANE_CONFIG_H_

/* ========================================================================== */
/*  Configuration:                                                            */
/*   * Compiler, architecture, and platform autodetection;                    */
/*   * Paranoid and/or debug, development, and release selection;             */
/*   * Task scheduler.                                                        */
/* ========================================================================== */

/* ========================================================================== */
/*  Compiler, architecture, and platform autodetection:                       */
/* ========================================================================== */

/*! @def BT_DONT_AUTODETECT_COMPILER
  See FND_DONT_AUTODETECT_COMPILER. */
#ifdef BT_DONT_AUTODETECT_COMPILER
  #define FND_DONT_AUTODETECT_COMPILER
#endif

/*! @def BT_DONT_AUTODETECT_PLATFORM
  See FND_DONT_AUTODETECT_PLATFORM. */
#ifdef BT_DONT_AUTODETECT_PLATFORM
  #define FND_DONT_AUTODETECT_PLATFORM
#endif

/*! @def BT_DONT_AUTODETECT_ARCHITECTURE
  See FND_DONT_AUTODETECT_ARCHITECTURE. */
#ifdef BT_DONT_AUTODETECT_ARCHITECTURE
  #define FND_DONT_AUTODETECT_ARCHITECTURE
#endif

/* ========================================================================== */
/*  Paranoid and/or debug, development, and release selection:                */
/* ========================================================================== */

/*! @def BT_PARANOID
  See FND_PARANOID. */
#ifdef BT_PARANOID
  #define FND_PARANOID
#endif

/*! @def BT_DEBUG
  See FND_DEBUG. */
#define BT_DEBUG FND_DEBUG

/*! @def BT_DEVELOPMENT
  See FND_DEVELOPMENT. */
#define BT_DEVELOPMENT FND_DEVELOPMENT

/*! @def BT_RELEASE
  See FND_RELEASE. */
#define BT_RELEASE FND_RELEASE

/*! @def BT_CONFIGURATION
  See FND_CONFIGURATION. */
#ifndef BT_CONFIGURATION
  #error ("Please specify a configuration by defining `BT_CONFIGURATION`.")
#else
  #define FND_CONFIGURATION BT_CONFIGURATION
#endif

/* ========================================================================== */
/*  Task scheduler:                                                           */
/* ========================================================================== */

/*! @def BT_TASK_SCHED_QUEUE_SZ
  Specifies the size of the task scheduler's global task queue. */
#define BT_TASK_SCHED_QUEUE_SZ 256

/* ========================================================================== */
 #  include <foundation/config.h>
/* ========================================================================== */

#endif /* _BUTANE_CONFIG_H_ */
