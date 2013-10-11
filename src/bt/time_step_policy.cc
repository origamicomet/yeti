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
    #include <bt/time_step_policy.h>
/* ========================================================================== */

/* ========================================================================== */
/*  Time Step Policy:                                                         */
/* ========================================================================== */

typedef struct variable {
  bt_time_step_policy_t tsp;
  float delta_;
} variable_t;

static void bt_time_step_policy_variable__update(
  bt_time_step_policy_t *time_step_policy,
  const bt_monotonic_clock_t *wall,
  const bt_monotonic_clock_t *frame)
{
  variable_t *time_step_policy_ = (variable_t *)time_step_policy;
  time_step_policy_->delta_ = ((float)bt_monotonic_clock_nsecs(frame) / 1000000000.0f);
}

static size_t bt_time_step_policy_variable__num_of_ticks(
  const bt_time_step_policy_t *time_step_policy)
{
  const variable_t *time_step_policy_ = (const variable_t *)time_step_policy;
  return 1;
}

static float bt_time_step_policy_variable__step_per_tick(
  const bt_time_step_policy_t *time_step_policy)
{
  const variable_t *time_step_policy_ = (const variable_t *)time_step_policy;
  return time_step_policy_->delta_;
}

bt_time_step_policy_t *bt_time_step_policy_variable() {
  variable_t *time_step_policy = (variable_t *)bt_allocator_alloc(
    bt_heap_allocator(),
    sizeof(variable_t),
    bt_alignof(variable_t));
  time_step_policy->tsp.update = &bt_time_step_policy_variable__update;
  time_step_policy->tsp.num_of_ticks = &bt_time_step_policy_variable__num_of_ticks;
  time_step_policy->tsp.step_per_tick = &bt_time_step_policy_variable__step_per_tick;
  time_step_policy->delta_ = 0.0f;
  return &time_step_policy->tsp;
}

/* ========================================================================== */

typedef struct fixed {
  bt_time_step_policy_t tsp;
  float frame_rate;
  float debt_;
  size_t num_of_ticks_;
} fixed_t;

static void bt_time_step_policy_fixed__update(
  bt_time_step_policy_t *time_step_policy,
  const bt_monotonic_clock_t *wall,
  const bt_monotonic_clock_t *frame)
{
  fixed_t *time_step_policy_ = (fixed_t *)time_step_policy;
  time_step_policy_->debt_ += ((float)bt_monotonic_clock_nsecs(frame) / 1000000000.0f);
  time_step_policy_->num_of_ticks_ = ((size_t)time_step_policy_->debt_ / time_step_policy_->frame_rate);
  time_step_policy_->debt_ -= time_step_policy_->num_of_ticks_ * time_step_policy_->frame_rate;
}

static size_t bt_time_step_policy_fixed__num_of_ticks(
  const bt_time_step_policy_t *time_step_policy)
{
  const fixed_t *time_step_policy_ = (const fixed_t *)time_step_policy;
  return time_step_policy_->num_of_ticks_;
}

static float bt_time_step_policy_fixed__step_per_tick(
  const bt_time_step_policy_t *time_step_policy)
{
  const fixed_t *time_step_policy_ = (const fixed_t *)time_step_policy;
  return time_step_policy_->frame_rate;
}

bt_time_step_policy_t *bt_time_step_policy_fixed(
  const float frame_rate)
{
  fixed_t *time_step_policy = (fixed_t *)bt_allocator_alloc(
    bt_heap_allocator(),
    sizeof(fixed_t),
    bt_alignof(fixed_t));
  time_step_policy->tsp.update = &bt_time_step_policy_fixed__update;
  time_step_policy->tsp.num_of_ticks = &bt_time_step_policy_fixed__num_of_ticks;
  time_step_policy->tsp.step_per_tick = &bt_time_step_policy_fixed__step_per_tick;
  time_step_policy->frame_rate = frame_rate;
  time_step_policy->debt_ = 0.0f;
  time_step_policy->num_of_ticks_ = 0;
  return &time_step_policy->tsp;
}

/* ========================================================================== */

typedef struct smoothed {
  bt_time_step_policy_t tsp;
  size_t history;
  size_t outliers;
  float rate;
  size_t saturation_;
  float history_[32];
  float delta_;
} smoothed_t;

static int bt_time_step_policy_smoothed__history_comparator(const void *lhs, const void *rhs) {
  const float lhs_ = *((const float *)lhs);
  const float rhs_ = *((const float *)rhs);
  if (lhs_ < rhs_)
    return -1;
  else if (rhs_ > lhs_)
    return 1;
  return 0;
}

static float bt_time_step_policy_smoothed__avg(const float *history, const size_t history_sz) {
  float sum = 0.0f;
  for (size_t i = 0; i < history_sz; ++i)
    sum += history[i];
  return (sum / (float)history_sz);
}

static void bt_time_step_policy_smoothed__update(
  bt_time_step_policy_t *time_step_policy,
  const bt_monotonic_clock_t *wall,
  const bt_monotonic_clock_t *frame)
{
  smoothed_t *time_step_policy_ = (smoothed_t *)time_step_policy;
  bt_copy_overlapped(
    (const void *)&time_step_policy_->history_[0],
    (void *)&time_step_policy_->history_[1],
    (time_step_policy_->history - 1) * sizeof(float));
  time_step_policy_->history_[0] =
    ((float)bt_monotonic_clock_nsecs(frame) / 1000000000.0f);
  time_step_policy_->saturation_++;
  if (bt_likely(time_step_policy_->saturation_ > time_step_policy_->history))
    time_step_policy_->saturation_ = time_step_policy_->history;
  if (bt_likely(time_step_policy_->saturation_ >= (time_step_policy_->outliers * 2 + 1))) {
    float sorted[32];
    bt_copy(
      (const void *)&time_step_policy_->history_[0],
      (void *)&sorted[0],
      time_step_policy_->saturation_ * sizeof(float));
    qsort((void *)&sorted[0], 32, sizeof(float), &bt_time_step_policy_smoothed__history_comparator);
    const float avg = bt_time_step_policy_smoothed__avg(
      &sorted[time_step_policy_->outliers - 1],
      time_step_policy_->saturation_ - (time_step_policy_->outliers * 2));
    const float rate = time_step_policy_->rate;
    const float smoothed =
      (rate * avg + (1 - rate) * time_step_policy_->history_[0]);
    time_step_policy_->delta_ = smoothed;
  } else {
    time_step_policy_->delta_ = time_step_policy_->history_[0];
  }
}

static size_t bt_time_step_policy_smoothed__num_of_ticks(
  const bt_time_step_policy_t *time_step_policy)
{
  const smoothed_t *time_step_policy_ = (const smoothed_t *)time_step_policy;
  return 1;
}

static float bt_time_step_policy_smoothed__step_per_tick(
  const bt_time_step_policy_t *time_step_policy)
{
  const smoothed_t *time_step_policy_ = (const smoothed_t *)time_step_policy;
  return time_step_policy_->delta_;
}

bt_time_step_policy_t *bt_time_step_policy_smoothed(
  const size_t history,
  const size_t outliers,
  const float rate)
{
  // bt_assert(development, history <= 32);
  // bt_assert(development, (outliers * 2 + 1) <= 32);
  // bt_assert(development, rate > 0.0f);
  smoothed_t *time_step_policy = (smoothed_t *)bt_allocator_alloc(
    bt_heap_allocator(),
    sizeof(smoothed_t),
    bt_alignof(smoothed_t));
  time_step_policy->tsp.update = &bt_time_step_policy_smoothed__update;
  time_step_policy->tsp.num_of_ticks = &bt_time_step_policy_smoothed__num_of_ticks;
  time_step_policy->tsp.step_per_tick = &bt_time_step_policy_smoothed__step_per_tick;
  time_step_policy->history = history;
  time_step_policy->outliers = outliers;
  time_step_policy->rate = rate;
  time_step_policy->saturation_ = 0;
  bt_zero((void *)&time_step_policy->history_[0], 32 * sizeof(float));
  time_step_policy->delta_ = 0.0f;
  return &time_step_policy->tsp;
}

/* ========================================================================== */

void bt_time_step_policy_destroy(
  bt_time_step_policy_t *time_step_policy)
{
  // bt_assert(debug, time_step_policy != NULL);
  bt_allocator_free(bt_heap_allocator(), (void *)time_step_policy);
}

/* ========================================================================== */

void bt_time_step_policy_update(
  bt_time_step_policy_t *time_step_policy,
  const bt_monotonic_clock_t *wall,
  const bt_monotonic_clock_t *frame)
{
  time_step_policy->update(time_step_policy, wall, frame);
}

size_t bt_time_step_policy_num_of_ticks(
  const bt_time_step_policy_t *time_step_policy)
{
  return time_step_policy->num_of_ticks(time_step_policy);
}

float bt_time_step_policy_step_per_tick(
  const bt_time_step_policy_t *time_step_policy)
{
  return time_step_policy->step_per_tick(time_step_policy);
}
