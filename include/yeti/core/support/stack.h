//===-- yeti/core/support/stack.h -----------------------*- mode: C++11 -*-===//
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
/// \brief Normalizes dynamic stack allocation.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_CORE_SUPPORT_STACK_H_
#define _YETI_CORE_SUPPORT_STACK_H_

#if YETI_COMPILER == YETI_COMPILER_MSVC
  #include <malloc.h>

  #ifndef alloca
    #define alloca _alloca
  #endif
#elif YETI_COMPILER == YETI_COMPILER_CLANG || \
      YETI_COMPILER == YETI_COMPILER_GCC
  // Dynamic stack allocation is only inlined if `alloca.h` is included when
  // non-extended standards are specified on the command line.
  #include <alloca.h>
#endif

#endif // _YETI_CORE_SUPPORT_STACK_H_
