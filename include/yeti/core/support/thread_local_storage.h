//===-- yeti/core/support/thread_local_storage.h --------*- mode: C++11 -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines a pre-processor macro that assist in conveying to the
/// compiler that global variables should use thread-local storage.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_SUPPORT_THREAD_LOCAL_STORAGE_H_
#define _YETI_CORE_SUPPORT_THREAD_LOCAL_STORAGE_H_

/// \def YETI_THREAD_LOCAL
/// \brief Marks a static variable as "thread local" meaning each thread
/// has its own unique copy.
#if defined(DOXYGEN)
  #define YETI_THREAD_LOCAL
#else
  #if (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_THREADS__)
    #define YETI_THREAD_LOCAL _Thread_local
  #else
    #if defined(_MSC_VER)
      #define YETI_THREAD_LOCAL __declspec(thread)
    #elif defined(__clang__) || defined(__GNUC__)
      #define YETI_THREAD_LOCAL __thread
    #endif
  #endif
#endif

#endif // _YETI_CORE_SUPPORT_THREAD_LOCAL_STORAGE_H_
