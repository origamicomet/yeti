//===-- yeti/foundation/support/macro.h -------------------*- mode: C++ -*-===//
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
/// \brief Defines pre-processor macros that assist in advanced pre-processor
/// magics.
///
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_MACRO_H_
#define _YETI_FOUNDATION_SUPPORT_MACRO_H_

/// \def YETI_STRINGIFY
/// \brief Expands @Value prior to converting to string.
#define YETI_STRINGIFY(Value) YETI_STRINGIFY__EXPAND(Value)
#define YETI_STRINGIFY__EXPAND(Value) YETI_STRINGIFY__CONVERT(Value)
#define YETI_STRINGIFY__CONVERT(Value) #Value

/// \def YETI_PASTE
/// \brief Expands @Left and @Right prior to concatenation.
#define YETI_PASTE(Left, Right) YETI_PASTE__EXPAND(Left, Right)
#define YETI_PASTE__EXPAND(Left, Right) YETI_PASTE__PASTE(Left, Right)
#define YETI_PASTE__PASTE(Left, Right) Left ## Right

/// \def YETI_NUM_OF_ARGS
/// \brief Returns the number of arguments supplied.
/// \warning Only supports up to 9 arguments!
#define YETI_NUM_OF_ARGS(...) __YETI_NUM_OF_ARGS__(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define __YETI_NUM_OF_ARGS__(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

/// \def YETI_FOLDL
/// \brief Left-associative fold.
#define YETI_FOLDL(Macro, Accumulator, ...) \
  YETI_PASTE(YETI_FOLDL_, YETI_NUM_OF_ARGS(__VA_ARGS__)) (Macro, Accumulator, __VA_ARGS__)

#define YETI_FOLDL_1(M, A, _1) M(A, _1)
#define YETI_FOLDL_2(M, A, _1, _2) M(M(A, _1), _2)
#define YETI_FOLDL_3(M, A, _1, _2, _3) M(M(M(A, _1), _2), _3)
#define YETI_FOLDL_4(M, A, _1, _2, _3, _4) M(M(M(M(A, _1), _2), _3), _4)
#define YETI_FOLDL_5(M, A, _1, _2, _3, _4, _5) M(M(M(M(M(A, _1), _2), _3), _4), _5)
#define YETI_FOLDL_6(M, A, _1, _2, _3, _4, _5, _6) M(M(M(M(M(M(A, _1), _2), _3), _4), _5), _6)
#define YETI_FOLDL_7(M, A, _1, _2, _3, _4, _5, _6, _7) M(M(M(M(M(M(M(A, _1), _2), _3), _4), _5), _6), _7)
#define YETI_FOLDL_8(M, A, _1, _2, _3, _4, _5, _6, _7, _8) M(M(M(M(M(M(M(A, _1), _2), _3), _4), _5), _6), _8)
#define YETI_FOLDL_9(M, A, _1, _2, _3, _4, _5, _6, _7, _8, _9) M(M(M(M(M(M(M(M(A, _1), _2), _3), _4), _5), _6), _8), _9)

/// \def YETI_FOLDR
/// \brief Right-associative fold.
#define YETI_FOLDR(Macro, Accumulator, ...) \
  YETI_PASTE(YETI_FOLDR_, YETI_NUM_OF_ARGS(__VA_ARGS__)) (Macro, Accumulator, __VA_ARGS__)

#define YETI_FOLDR_1(M, A, _1) M(_1, A)
#define YETI_FOLDR_2(M, A, _1, _2) M(_1, M(_2, A))
#define YETI_FOLDR_3(M, A, _1, _2, _3) M(_1, M(_2, M(_3, A)))
#define YETI_FOLDR_4(M, A, _1, _2, _3, _4) M(_1, M(_2, M(_3, M(_4, A))))
#define YETI_FOLDR_5(M, A, _1, _2, _3, _4, _5) M(_1, M(_2, M(_3, M(_4, M(_5, A)))))
#define YETI_FOLDR_6(M, A, _1, _2, _3, _4, _5, _6) M(_1, M(_2, M(_3, M(_4, M(_5, M(_6, A))))))
#define YETI_FOLDR_7(M, A, _1, _2, _3, _4, _5, _6, _7) M(_1, M(_2, M(_3, M(_4, M(_5, M(_6, M(_7, A)))))))
#define YETI_FOLDR_8(M, A, _1, _2, _3, _4, _5, _6, _7, _8) M(_1, M(_2, M(_3, M(_4, M(_5, M(_6, M(_7, M(_8, A))))))))
#define YETI_FOLDR_9(M, A, _1, _2, _3, _4, _5, _6, _7, _8, _9) M(_1, M(_2, M(_3, M(_4, M(_5, M(_6, M(_7, M(_8, M(_9, A)))))))))

#endif // _YETI_FOUNDATION_SUPPORT_MACRO_H_
