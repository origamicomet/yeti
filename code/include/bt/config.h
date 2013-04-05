// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_CONFIG_H_
#define _BUTANE_CONFIG_H_

#include <foundation/preprocessor.h>
#include <foundation/detect.h>
#include <foundation/compat.h>
#include <foundation/config.h>

// Provides a central location to modify engine form and functionality.

#if defined(FOUNDATION_ARCH_X86_64)
  #define BT_ARCHITECTURE x86_64
#elif defined(FOUNDATION_ARCH_X86)
  #define BT_ARCHITECTURE x86
#elif defined(FOUNDATION_ARCH_ARM)
  #define BT_ARCHITECTURE arm
#else
  #define BT_ARCHITECTURE unknown
#endif

#if defined(FOUNDATION_PLATFORM_WINDOWS)
  #define BT_PLATFORM windows
#elif defined(FOUNDATION_PLATFORM_MACOSX)
  #define BT_PLATFORM macosx
#elif defined(FOUNDATION_PLATFORM_LINUX)
  #define BT_PLATFORM linux
#elif defined(FOUNDATION_PLATFORM_BSD)
  #define BT_PLATFORM bsd
#elif defined(FOUNDATION_PLATFORM_IOS)
  #define BT_PLATFORM ios
#elif defined(FOUNDATION_PLATFORM_ANDROID)
  #define BT_PLATFORM android
#else
  #define BT_PLATFORM unknown
#endif

#if defined(BT_DEBUG_BUILD)
  #define BT_BUILD debug
#elif defined(BT_DEVELOPMENT_BUILD)
  #define BT_BUILD development
#elif defined(BT_RELEASE_BUILD)
  #define BT_BUILD release
#else
  #define BT_PLATFORM unknown
#endif

#endif // _BUTANE_CONFIG_H_
