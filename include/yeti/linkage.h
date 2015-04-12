/*===-- yeti/linkage.h --------------------------------------*- mode: C -*-===*\
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
|* Defines pre-processor macros that specify symbol visibility.               *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef _YETI_LINKAGE_H_
#define _YETI_LINKAGE_H_

/*! \def YETI_INTERNAL_LINKAGE
 *  \brief Exports symbols for inclusion of Yeti via objects.
 */
#define YETI_INTERNAL_LINKAGE 0

/*! \def YETI_STATIC_LINKAGE
 *  \brief Exports symbols for inclusion of Yeti via static library.
 */
#define YETI_STATIC_LINKAGE 1

/*! \def YETI_DYNAMIC_LINKAGE
 *  \brief Exports symbols for inclusion of Yeti via dynamic library.
 */
#define YETI_DYNAMIC_LINKAGE 2

/*! \def YETI_LINKAGE
 *  \brief Specifies how you intend to link to Yeti.
 */
#ifndef YETI_LINKAGE
  #error ("Please specify how you intend on linking to Yei by defining `YETI_LINKAGE'.")
#endif

/*! \def YETI_LOCAL
 *  \brief Marks a symbol for internal usage.
 */
#ifdef DOXYGEN
  #define YETI_LOCAL
#else /* #ifndef DOXYGEN */
  #if YETI_LINKAGE == YETI_STATIC_LINKAGE
    #define YETI_LOCAL
  #elif YETI_LINKAGE == YETI_DYNAMIC_LINAKGE
    #ifdef __YETI_IS_BEING_COMPILED__
      #if defined(__GNUC__)
        #if __GNUC__ >= 4
          #define YETI_LOCAL __attribute__ ((visibility ("hidden")))
        #else
          #define YETI_LOCAL
        #endif
      #elif defined(__clang__)
        #define YETI_LOCAL __attribute__ ((visibility ("hidden")))
      #elif defined(_MSC_VER) || defined(__CYGWIN__)
        #define YETI_LOCAL
      #else
        #error ("Unknown or unsupported toolchain!")
      #endif
    #else /* #ifndef __YETI_IS_BEING_COMPILED__ */
      #define YETI_LOCAL
    #endif
  #endif
#endif

/*! \def YETI_PUBLIC
 *  \brief Marks a symbol for public usage.
 */
#ifdef DOXYGEN
  #define YETI_PUBLIC
#else /* #ifndef DOXYGEN */
  #if YETI_LINKAGE == YETI_STATIC_LINKAGE
    #define YETI_PUBLIC
  #elif YETI_LINKAGE == YETI_DYNAMIC_LINAKGE
    #ifdef __YETI_IS_BEING_COMPILED__
      #if defined(__GNUC__)
        #if __GNUC__ >= 4
          #define YETI_PUBLIC __attribute__ ((visibility ("default")))
        #else
          #define YETI_PUBLIC
        #endif
      #elif defined(__clang__)
        #define YETI_PUBLIC __attribute__ ((visibility ("default")))
      #elif defined(_MSC_VER) || defined(__CYGWIN__)
        #define YETI_PUBLIC __declspec(dllexport)
      #else
        #error ("Unknown or unsupported toolchain!")
      #endif
    #else /* #ifndef __YETI_IS_BEING_COMPILED__ */
      #if (defined(__GNUC__) && (__GNUC__ >= 4))
        #define YETI_PUBLIC
      #elif defined(__clang__)
        #define YETI_PUBLIC
      #elif defined(_MSC_VER) || defined(__CYGWIN__)
        #define YETI_PUBLIC __declspec(dllimport)
      #else
        #error ("Unknown or unsupported toolchain!")
      #endif
    #endif
  #endif
#endif

/*! \def YETI_BEGIN_EXTERN_C
 *  \internal
 */
/*! \def YETI_END_EXTERN_C
 *  \internal
 */
#ifdef DOXYGEN
  #define YETI_BEGIN_EXTERN_C
  #define YETI_END_EXTERN_C
#else /* #ifndef DOXYGEN */
  #if defined(__cplusplus)
    #define YETI_BEGIN_EXTERN_C extern "C" {
    #define YETI_END_EXTERN_C }
  #else /* #ifndef __cplusplus */
    #define YETI_BEGIN_EXTERN_C
    #define YETI_END_EXTERN_C
  #endif
#endif

#endif /* _YETI_LINKAGE_H_ */

/*============================================================================*/
