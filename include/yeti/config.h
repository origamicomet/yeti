/*===-- yeti/config.h ---------------------------------------*- mode: C -*-===*\
|*                                                                            *|
|*                             __ __     _   _                                *|
|*                            |  |  |___| |_|_|                               *|
|*                            |_   _| -_|  _| |                               *|
|*                              |_| |___|_| |_|                               *|
|*                                                                            *|
|*       This file is distributed under the terms described in LICENSE.       *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|*                                                                            *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef _YETI_CONFIG_H_
#define _YETI_CONFIG_H_

/*! \def YETI_CONFIGURATION_DEBUG
 *  \brief Enables debugging, development, and release features.
 */
#define YETI_CONFIGURATION_DEBUG 1

/*! \def YETI_CONFIGURATION_DEVELOPMENT
 *  \brief Enables development and release features.
 */
#define YETI_CONFIGURATION_DEVELOPMENT 2

/*! \def YETI_CONFIGURATION_DEVELOPMENT
 *  \brief Enables release features.
 */
#define YETI_CONFIGURATION_RELEASE 3

/*! \def YETI_CONFIGURATION
 *  \brief Specifies how "loose and fast" Yeti is.
 */
#ifndef YETI_CONFIGURATION
  #error ("Please specify a configuration by defining `YETI_CONFIGURATION'.")
#endif

#endif /* _YETI_CONFIG_H_ */

/*============================================================================*/
