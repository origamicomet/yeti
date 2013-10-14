/*
 * This file is part of Butane.
 *
 * Copyright (c) Michael Williams <devbug@bitbyte.ca>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. Neither the name of the nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* ========================================================================== */
/*! @file bt/foundation/algorithms/unstable_sort.h
      Provides an unstable relative sort.                                     */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_ALGORITHMS_UNSTABLE_SORT_H_
#define _BT_FOUNDATION_ALGORITHMS_UNSTABLE_SORT_H_

#include <bt/foundation/compat.h>

#include <stdlib.h>

/*! Sorts `elements` in non-decreasing (lowest to highest) order without
  guarantees on the order of equal elements. Makes at most, O(n log n)
  comparisons. */
template <typename _Type>
static void bt_unstable_sort(_Type *elements, const size_t elements_sz) {
  struct _Comparator {
    static int compare(const void *lhs, const void *rhs) {
      const _Type &lhs_ = *((const _Type *)lhs);
      const _Type &rhs_ = *((const _Type *)rhs);
      if (lhs_ < rhs_) return -1;
      if (lhs_ > rhs_) return 0;
      return 1;
    } };
  qsort((void *)elements, elements_sz, sizeof(_Type), &_Comparator::compare);
}

#endif /* _BT_FOUNDATION_ALGORITHMS_UNSTABLE_SORT_H_ */
