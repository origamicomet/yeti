//===-- yeti/config.h -------------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines and checks the sanity of various pre-processor macros that
/// control the building and behaviour of Yeti.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CONFIG_H_
#define _YETI_CONFIG_H_

/// \def YETI_CONFIGURATION_DEBUG
/// \brief Debug build.
#define YETI_CONFIGURATION_DEBUG 1

/// \def YETI_CONFIGURATION_DEVELOPMENT
/// \brief Development build.
#define YETI_CONFIGURATION_DEVELOPMENT 2

/// \def YETI_CONFIGURATION_RELEASE
/// \brief Release build.
#define YETI_CONFIGURATION_RELEASE 3

/// \def YETI_CONFIGURATION
/// \brief Specifies how "loose and fast" Yeti is.
#if defined(DOXYGEN)
  #define YETI_CONFIGURATION
#else
  #if !defined(YETI_CONFIGURATION)
    #error ("No configuration specified! Please specify a configuration by defining `YETI_CONFIGURATION'.")
  #else
    #if (YETI_CONFIGURATION != YETI_CONFIGURATION_DEBUG) && \
        (YETI_CONFIGURATION != YETI_CONFIGURATION_DEVELOPMENT) && \
        (YETI_CONFIGURATION != YETI_CONFIGURATION_RELEASE)
      #error ("Unknown configuration specified. See include/yeti/config.h for a list of possible configurations.")
    #endif
  #endif
#endif

/// \macro YETI_PLATFORM_WINDOWS
/// \brief Microsoft Windows.
#define YETI_PLATFORM_WINDOWS 1

/// \macro YETI_PLATFORM_MAC_OS_X
/// \brief Apple's Mac OS X.
#define YETI_PLATFORM_MAC_OS_X 2

/// \macro YETI_PLATFORM_LINUX
/// \brief GNU/Linux.
#define YETI_PLATFORM_LINUX 3

/// \macro YETI_PLATFORM_IOS
/// \brief Apple's iOS.
#define YETI_PLATFORM_IOS 4

/// \macro YETI_PLATFORM_ANDROID
/// \brief Google's Android.
#define YETI_PLATFORM_ANDROID 5

/// \macro YETI_PLATFORM
/// \brief Target platform.
#if defined(DOXYGEN)
  #define YETI_PLATFORM
#else // !defined(DOXYGEN)
  #if defined(_WIN32) || defined(_WIN64)
    #define YETI_PLATFORM YETI_PLATFORM_WINDOWS
  #elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if defined(TARGET_OS_IPHONE)
      #define YETI_PLATFORM YETI_PLATFORM_IOS
    #else
      #define YETI_PLATFORM YETI_PLATFORM_MAC_OS_X
    #endif
  #elif defined(__linux__)
    #if defined(ANDROID)
      #define YETI_PLATFORM YETI_PLATFORM_ANDROID
    #else
      #define YETI_PLATFORM YETI_PLATFORM_LINUX
    #endif
  #else
    #error ("Unknown or unsupported platform!")
  #endif
#endif

/// \macro YETI_COMPILER_MSVC
/// \brief Microsoft Visual C/C++.
#define YETI_COMPILER_MSVC 1

/// \macro YETI_COMPILER_GCC
/// \brief GNU Compiler Collection.
#define YETI_COMPILER_GCC 2

/// \macro YETI_COMPILER_CLANG
/// \brief LLVM/Clang.
#define YETI_COMPILER_CLANG 3

/// \macro YETI_COMPILER
/// \brief Host compiler.
#if defined(DOXYGEN)
  #define YETI_COMPILER
#else // !defined(DOXYGEN)
  #if defined(_MSC_VER)
    #define YETI_COMPILER YETI_COMPILER_MSVC
    #define YETI_MICROSOFT_VISUAL_STUDIO_2013 1800
    #define YETI_MICROSOFT_VISUAL_STUDIO_2012 1700
    #define YETI_MICROSOFT_VISUAL_STUDIO_2010 1600
    #define YETI_MICROSOFT_VISUAL_STUDIO_2008 1500
    #define YETI_MICROSOFT_VISUAL_STUDIO_2005 1400
    #define YETI_MICROSOFT_VISUAL_STUDIO_2003 1310
    #define YETI_MICROSOFT_VISUAL_STUDIO_2002 1300
    #define YETI_MICROSOFT_VISUAL_C_6 1200
    #if (_MSC_VER >= MICROSOFT_VISUAL_STUDIO_2005)
      // ASSUME(mtwilliams): Microsoft Visual Studio 2005 or newer.
    #elif (_MSC_VER == MICROSOFT_VISUAL_STUDIO_2003)
      #error ("Microsoft Visual C/C++ .NET 2003 is unsupported! Please upgrade to Microsoft Visual C/C++ 2005 or newer.")
    #elif (_MSC_VER == MICROSOFT_VISUAL_STUDIO_2002)
      #error ("Microsoft Visual C/C++ .NET 2002 is unsupported! Please upgrade to Microsoft Visual C/C++ 2005 or newer.")
      // Not to mention that it has major bugs so annoying that Microsoft provides
      // complementary upgrades to Microsoft Visual C/C++ .NET 2003!
    #elif (_MSC_VER == YETI_MICROSOFT_VISUAL_C_6)
      #error ("Microsoft Visual C/C++ 6 is unsupported! Please upgrade to Microsoft Visual C/C++ 2005 or newer.")
      // If we did ever want to support it, we should check for Service Pack 6 by
      // checking if _MSC_FULL_VER is less than 12008804.
    #else
      #error ("Your version of Microsoft Visual C/C++ is unsupported! Please upgrade to Microsoft Visual C/C++ 2005 or newer.")
    #endif
  #elif defined(__GNUC__)
    #if defined(__clang__)
      #define YETI_COMPILER YETI_COMPILER_CLANG
      #define __CLANG_VERSION__ (__clang_major__ * 10000 \
                                 + __clang_minor__ * 100 \
                                 + __clang_patchlevel__)
    #else // !defined(__clang__)
      // ASSUME(mtwilliams): That we're being compiled with GCC.
      #define YETI_COMPILER YETI_COMPILER_GCC
      #if defined(__GNUC_PATCHLEVEL__)
        #define __GNUC_VERSION__ (__GNUC__ * 10000 \
                                  + __GNUC_MINOR__ * 100 \
                                  + __GNUC_PATCHLEVEL__)
      #else
        #define __GNUC_VERSION__ (__GNUC__ * 10000 \
                                  + __GNUC_MINOR__ * 100)
      #endif
    #endif
  #else
    #error ("Unknown or unsupported compiler!")
  #endif
#endif

/// \macro YETI_ARCHITECTURE_X86
/// \brief Intel/AMD x86.
#define YETI_ARCHITECTURE_X86 1

/// \macro YETI_ARCHITECTURE_X86_64
/// \brief Intel/AMD x86-64.
#define YETI_ARCHITECTURE_X86_64 2

/// \macro YETI_ARCHITECTURE
/// \brief Target architecture.
#if defined(DOXYGEN)
  #define YETI_COMPILER
#else // !defined(DOXYGEN)
  #if defined(_M_IX86) || defined(__i386__)
    #define YETI_ARCHITECTURE YETI_ARCHITECTURE_X86
  #elif defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__)
    #define YETI_ARCHITECTURE YETI_ARCHITECTURE_X86_64
  #else
    #error ("Unknown or unsupported architecture!")
  #endif
#endif

#endif // _YETI_CONFIG_H_

//===----------------------------------------------------------------------===//
