//===-- yeti/foundation/support/thread_local_storage.h ----*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_THREAD_LOCAL_STORAGE_H_
#define _YETI_FOUNDATION_SUPPORT_THREAD_LOCAL_STORAGE_H_

/// \def YETI_THREAD_LOCAL
/// \brief Marks a static variable as "thread local" meaning each thread
/// has its own unique copy.
#if defined(DOXYGEN)
  #define YETI_THREAD_LOCAL
#else // !defined(DOXYGEN)
  #if (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_THREADS__)
    #define YETI_THREAD_LOCAL _Thread_local
  #else // defined(__STDC_NO_THREADS__)
    #if defined(_MSC_VER)
      #define YETI_THREAD_LOCAL __declspec(thread)
    #elif defined(__clang__) || defined(__GNUC__)
      #define YETI_THREAD_LOCAL __thread
    #endif
  #endif
#endif

#endif // _YETI_FOUNDATION_SUPPORT_THREAD_LOCAL_STORAGE_H_
