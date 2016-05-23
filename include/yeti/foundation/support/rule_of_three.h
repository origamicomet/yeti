//===-- yeti/foundation/support/rule_of_three.h -----------*- mode: C++ -*-===//
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
/// \brief Defines pre-processor macros that assist in conveying the intented
/// usage of classes' constructors, assignment operators, and destructors.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_RULE_OF_THREE_H_
#define _YETI_FOUNDATION_SUPPORT_RULE_OF_THREE_H_

/// \def YETI_DISALLOW_CONSTRUCTION
/// \brief Prevent construction of @_Class_name.
#define YETI_DISALLOW_CONSTRUCTION(_Class_name) \
 private: \
  _Class_name(); \
  ~_Class_name();

/// \def YETI_DISALLOW_COPYING
/// \brief Prevent copying of @_Class_name.
#define YETI_DISALLOW_COPYING(_Class_name) \
 private: \
  _Class_name (const _Class_name &); \
  _Class_name &operator= (const _Class_name &);

#endif // _YETI_FOUNDATION_SUPPORT_RULE_OF_THREE_H_
