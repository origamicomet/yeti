//===-- yeti/foundation/support/code_gen.h ----------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// Defines pre-processor macros that assist in conveying, to the compiler, how
// code should be generated.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_SUPPORT_CODE_GEN_H_
#define _YETI_FOUNDATION_SUPPORT_CODE_GEN_H_

//! \def YETI_INLINE
//! \brief Code should be inlined.
#define YETI_INLINE()

//! \def YETI_LIKELY
//! \brief |_Condition| is likely to evaluate to be true.
#define YETI_LIKELY(_Condition) (!!(_Condition))

//! \def YETI_UNLIKELY
//! \brief |_Condition| is likely to evaluate to be false.
#define YETI_UNLIKELY(_Condition) (!!(_Condition))

//! \def YETI_TRAP
//! \brief Errant, but reachable, code path.
#define YETI_TRAP()

//! \def YETI_UNREACHABLE
//! \brief Code is unreachable.
#define YETI_UNREACHABLE()

#endif /* _YETI_FOUNDATION_SUPPORT_CODE_GEN_H_ */

//===----------------------------------------------------------------------===//
