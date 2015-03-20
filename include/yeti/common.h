/*===-- yeti/common.h ---------------------------------------*- mode: C -*-===*\
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

#ifndef _YETI_COMMON_H_
#define _YETI_COMMON_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

/* TODO(mtwilliams): Clean this up. */

#ifdef __cplusplus
  #define YETI_BEGIN_EXTERN_C \
    extern "C" {
  #define YETI_END_EXTERN_C \
    }
#else
  #define YETI_BEGIN_EXTERN_C
  #define YETI_END_EXTERN_C
#endif

#define __BITBYTE_FOUNDATION_IMPORT__

#include "bitbyte/foundation/tier0.h"
#include "bitbyte/foundation/tier1.h"
#include "bitbyte/foundation/tier2.h"
#include "bitbyte/foundation/tier3.h"
#include "bitbyte/foundation/tier4.h"

/*! \def yeti_assert_debug
 *  \copydoc bitbyte_foundation_assert
 */
/*! \def yeti_assertf_debug
 *  \copydoc bitbyte_foundation_assertf
 */
#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEBUG
  #define yeti_assert_debug bitbyte_foundation_assert
  #define yeti_assertf_debug bitbyte_foundation_assertf
#else
  #define yeti_assert_debug
  #define yeti_assertf_debug
#endif

/*! \def yeti_assert_development
 *  \copydoc bitbyte_foundation_assert
 */
/*! \def yeti_assertf_development
 *  \copydoc bitbyte_foundation_assertf
 */
#if YETI_CONFIGURATION == YETI_CONFIGURATION_DEVELOPMENT
  #define yeti_assert_development bitbyte_foundation_assert
  #define yeti_assertf_development bitbyte_foundation_assertf
#else
  #define yeti_assert_development
  #define yeti_assertf_development
#endif

/*! \def yeti_assert_release
 *  \copydoc bitbyte_foundation_assert
 */
/*! \def yeti_assertf_release
 *  \copydoc bitbyte_foundation_assertf
 */
#if YETI_CONFIGURATION == YETI_CONFIGURATION_RELEASE
  #define yeti_assert_release bitbyte_foundation_assert
  #define yeti_assertf_release bitbyte_foundation_assertf
#else
  #define yeti_assert_release
  #define yeti_assertf_release
#endif

YETI_BEGIN_EXTERN_C /* { */

typedef int8_t yeti_int8_t;
typedef uint8_t yeti_uint8_t;

typedef int16_t yeti_int16_t;
typedef uint16_t yeti_uint16_t;

typedef int32_t yeti_int32_t;
typedef uint32_t yeti_uint32_t;

typedef int64_t yeti_int64_t;
typedef uint64_t yeti_uint64_t;

typedef intptr_t yeti_intptr_t;
typedef uintptr_t yeti_uintptr_t;

typedef size_t yeti_size_t;
#ifdef _MSC_VER
  typedef intptr_t yeti_ssize_t;
#else
  typedef ssize_t yeti_ssize_t;
#endif

typedef bool yeti_bool_t;
#define YETI_TRUE true
#define YETI_FALSE false

YETI_END_EXTERN_C /* } */

#endif /* _YETI_COMMON_H_ */

/*============================================================================*/
