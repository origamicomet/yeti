//===-- yeti/foundation/atomic.h --------------------------*- mode: C++ -*-===//
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
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_ATOMIC_H_
#define _YETI_FOUNDATION_ATOMIC_H_

#include "yeti/config.h"
#include "yeti/linkage.h"
#include "yeti/types.h"

namespace yeti {
namespace foundation {
 /// \namespace ::yeti::foundation::atomic
 /// \brief
 namespace atomic {
  typedef ::yeti::i32 i32;
  typedef ::yeti::u32 u32;
  typedef ::yeti::i64 i64;
  typedef ::yeti::u64 u64;

  void load(i32 *v, const i32 desired);
  void load(u32 *v, const u32 desired);
  void load(i64 *v, const i64 desired);
  void load(u64 *v, const u64 desired);

  i32 store(const i32 *v);
  u32 store(const u32 *v);
  i64 store(const i64 *v);
  u64 store(const u64 *v);

  i32 add(i32 *lhs, const i32 rhs);
  u32 add(u32 *lhs, const u32 rhs);
  i64 add(i64 *lhs, const i64 rhs);
  u64 add(u64 *lhs, const u64 rhs);

  i32 sub(i32 *lhs, const i32 rhs);
  u32 sub(u32 *lhs, const u32 rhs);
  i64 sub(i64 *lhs, const i64 rhs);
  u64 sub(u64 *lhs, const u64 rhs);

  i32 cmp_and_xchg(i32 *v, const i32 expected, const i32 desired);
  u32 cmp_and_xchg(u32 *v, const u32 expected, const u32 desired);
  i64 cmp_and_xchg(i64 *v, const i64 expected, const i64 desired);
  u64 cmp_and_xchg(u64 *v, const u64 expected, const u64 desired);
 } // atomic
} // foundation
} // yeti

#endif /* _YETI_FOUNDATION_ATOMIC_H_ */

//===----------------------------------------------------------------------===//
