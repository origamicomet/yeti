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
/*! @file bt/profilier.h
      Provides the facilities to perform instrumented profiling .             */
/* ========================================================================== */

#ifndef _BT_PROFILIER_H_
#define _BT_PROFILIER_H_

#include <bt/config.h>
#include <bt/foundation.h>

/* ========================================================================== */
/*  Profiler:                                                                 */
/*   * Scope                                                                  */
/*   * Instrumenting macros                                                   */
/* ========================================================================== */

/* ========================================================================== */
/*  Scope:                                                                    */
/* ========================================================================== */

typedef struct bt_profiler_scope {
  struct bt_profiler_scope *parent;
  const char *name;
  uint64_t begin;
  uint64_t end;
} bt_profiler_scope_t;

/* ========================================================================== */

extern bt_profiler_scope_t *bt_profiler_current_scope();

extern void bt_profiler_set_current_scope(
  bt_profiler_scope_t *scope);

/* ========================================================================== */
/*  Instrumenting macros:                                                     */
/* ========================================================================== */

#if (defined(BT_DEBUG) || defined(BT_DEVELOPMENT))
  #define bt_profile_begin(_Name) \
    bt_profiler_scope_t bt_join(__prof_scope_, __LINE__) = { \
      bt_profiler_current_scope(), _Name, bt_rdtsc() }; \
    bt_profiler_set_current_scope(&bt_join(__prof_scope_, __LINE__));
  #define bt_profile_end() \
    do { \
      bt_profiler_scope_t *prof_scope = bt_profiler_current_scope(); \
      prof_scope->end = bt_rdtsc(); \
      bt_profiler_set_current_scope(prof_scope->parent); \
      const uint64_t cycles_in_scope = prof_scope->end - prof_scope->begin; \
      fprintf(stdout, "%s (%" PRIu64 " cycles)\n", prof_scope->name, cycles_in_scope); \
    } while (0)
#else /* (!defined(BT_DEBUG) && !defined(BT_DEVELOPMENT)) */
  #define bt_profile_begin(_Name) \
    do { (void)sizeof(_Name); } while (0)
  #define bt_profile_end() \
    do {} while (0)
#endif

#endif /* _BT_PROFILIER_H_ */
