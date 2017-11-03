//===-- yeti/kludge.h -----------------------------------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#ifdef _MSC_VER
  // Shut up about non-standard extensions.
  #pragma warning(disable: 4200 4201)

  // Disable warnings about types marked public when dynamically linking.
  #pragma warning(disable: 4251)

  // Disable warnings about synchronous exception handling.
  #pragma warning(disable: 4577)
#endif

#ifdef _MSC_VER
  // We don't care about buffer overflows, at least for the majority of our inputs.
  #define _CRT_SECURE_NO_WARNINGS 1
#endif
