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
/*! @file bt/time_step_policy.h
      Provides various time-step policies.                                    */
/* ========================================================================== */

#ifndef _BT_TIME_STEP_POLICY_H_
#define _BT_TIME_STEP_POLICY_H_

#include <bt/foundation.h>

/* ========================================================================== */
/*  Time Step Policy:                                                         */
/* ========================================================================== */

/*! Controls how the time-step is advanced. */
typedef struct bt_time_step_policy {
  void (*update)(
    struct bt_time_step_policy *time_step_policy,
    const bt_monotonic_clock_t *wall,
    const bt_monotonic_clock_t *frame);
  size_t (*num_of_ticks)(
    const struct bt_time_step_policy *time_step_policy);
  float (*step_per_tick)(
    const struct bt_time_step_policy *time_step_policy);
} bt_time_step_policy_t;

/* ========================================================================== */

/*! */
extern bt_time_step_policy_t *bt_time_step_policy_variable();

/*! */
extern bt_time_step_policy_t *bt_time_step_policy_fixed(
  const float frame_rate);

/*! */
extern bt_time_step_policy_t *bt_time_step_policy_smoothed(
  const size_t history,
  const size_t outliers,
  const float rate);

/*! */
extern void bt_time_step_policy_destroy(
  bt_time_step_policy_t *time_step_policy);

/* ========================================================================== */

/*! */
void bt_time_step_policy_update(
  bt_time_step_policy_t *time_step_policy,
  const bt_monotonic_clock_t *wall,
  const bt_monotonic_clock_t *frame);

/*! */
size_t bt_time_step_policy_num_of_ticks(
  const bt_time_step_policy_t *time_step_policy);

/*! */
float bt_time_step_policy_step_per_tick(
  const bt_time_step_policy_t *time_step_policy);

#endif /* _BT_TIME_STEP_POLICY_H_ */
