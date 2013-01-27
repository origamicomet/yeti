// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#ifndef _LWE_FOUNDATION_PREPROCESSOR_H_
#define _LWE_FOUNDATION_PREPROCESSOR_H_

#include <lwe/foundation/log.h>

#include <signal.h>

#define __LWE_STRINGIFY( x ) #x
#define  _LWE_STRINGIFY( x ) __LWE_STRINGIFY(x)
#define LWE_STRINGIFY( x ) _LWE_STRINGIFY(x)

#if defined(_DEBUG)
  #define lwe_assertf( condition, format, ... ) \
    do { if (!(condition)) { lwe_log(format, ##__VA_ARGS__); raise(SIGABRT); } } while (0, 0)
#else
  #define lwe_assertf( condition, format, ... ) \
    do { (void)sizeof(condition); } while(0, 0)
#endif

#define lwe_assert( condition ) \
  lwe_assertf(condition, "\nIn " LWE_STRINGIFY(__FILE__) " : " LWE_STRINGIFY(__LINE__) "\n\n  > An assertion failed:\n  > " #condition "\n\n")

#define lwe_fail( msg ) \
  do {  { lwe_log("\nIn " LWE_STRINGIFY(__FILE__) " : " LWE_STRINGIFY(__LINE__) "\n\n  > \"" msg "\"\n\n"); raise(SIGABRT); } } while (0, 0)

#define lwe_fail_if( condition, format, ... ) \
  do { if ((condition)) { lwe_log("\nIn " LWE_STRINGIFY(__FILE__) " : " LWE_STRINGIFY(__LINE__) "\n\n  > \"" #condition "\"\n  > " format "\n\n", ##__VA_ARGS__); raise(SIGABRT); } } while (0, 0)


#if defined(_DEBUG)
  #define LWE_DEBUG_BUILD
#elif defined(_DEVELOPMENT)
  #define LWE_DEVELOPMENT_BUILD
#else
  #define LWE_RELEASE_BUILD
#endif


#if defined(_MSC_VER)
  #define LWE_MSVC_BUILD
#endif

#if defined(__MINGW32__)
  #define LWE_MINGW_BUILD
#endif

#if defined(__CYGWIN32__)
  #define LWE_CYGWIN_BUILD
#endif

#if defined(__MINGW32__) || defined(__CYGWIN32__) || defined(__GNUC__)
  #define LWE_GCC_BUILD
#endif

#if defined(__clang__)
  #define LWE_CLANG_BUILD
#endif


#if defined(_MSC_VER)
  #define LWE_INLINE __forceinline
#else
  #define LWE_INLINE inline
#endif


#if defined(_WIN64)
  #define LWE_WIN64_BUILD
#elif defined(_WIN32)
  #define LWE_WIN32_BUILD
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define LWE_WINDOWS_BUILD
#endif

#if defined(__APPLE__) && defined(__MACH__)
  #define LWE_MACOSX_BUILD
#endif

#if defined(__linux)
  #define LWE_LINUX_BUILD
#endif

#if defined(LWE_MACOSX_BUILD) || defined(LWE_LINUX_BUILD)
  #define LWE_POSIX_BUILD
#endif

#endif // _LWE_FOUNDATION_PREPROCESSOR_H_