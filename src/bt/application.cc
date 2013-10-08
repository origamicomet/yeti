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
    #include <bt/application.h>
/* ========================================================================== */

#include <bt/profiler.h>

#include <stdlib.h>
#include <signal.h>

/* ========================================================================== */
/*  Build information:                                                        */
/* ========================================================================== */

const char *bt_application_architecture() {
#if (BT_ARCHITECTURE == BT_ARCHITECTURE_X86)
  return "x86";
#elif (BT_ARCHITECTURE == BT_ARCHITECTURE_X86_64)
  return "x86-64";
#elif (BT_ARCHITECTURE == BT_ARCHITECTURE_ARM)
  return "arm";
#else
  #error ("Unknown or unsupported architecture!")
#endif
}

const char *bt_application_platform() {
#if (BT_PLATFORM == BT_PLATFORM_WINDOWS)
  return "windows";
#elif (BT_PLATFORM == BT_PLATFORM_MACOSX)
  return "macosx";
#elif (BT_PLATFORM == BT_PLATFORM_LINUX)
  return "linux";
#else
  #error ("Unknown or unsupported platform!")
#endif
}

const char *bt_application_build() {
  return bt_stringificate(BT_BUILD);
}

/* ========================================================================== */
/*  Control (pause, unpause, etc.):                                           */
/* ========================================================================== */

static bt_time_step_policy_t *_time_step_policy = NULL;

void bt_application_set_time_step_policy(bt_time_step_policy_t *time_step_policy) {
  // bt_assert(debug, time_step_policy != NULL);
  _time_step_policy = time_step_policy;
}

void bt_application_pause() {
  raise(SIGABRT);
}

void bt_application_unpause() {
  raise(SIGABRT);
}

void bt_application_quit() {
  exit(EXIT_SUCCESS);
}

/* ========================================================================== */
/*  Boot and run:                                                              */
/* ========================================================================== */

static void bt_application_update(const float dt) {
  bt_profile_begin("bt_application_update");
  bt_profile_end();
}

static void bt_application_render() {
  bt_profile_begin("bt_application_render");
  bt_profile_end();
}

static void bt_application_run() {
  bt_profile_begin("bt_application_run");

  bt_monotonic_clock_t *wall = bt_monotonic_clock_create();
  bt_monotonic_clock_t *frame = bt_monotonic_clock_create();

  while (true) {
    bt_time_step_policy_update(_time_step_policy, wall, frame);
    bt_monotonic_clock_reset(frame);
    const size_t num_of_ticks = bt_time_step_policy_num_of_ticks(_time_step_policy);
    for (size_t tick = 0; tick < num_of_ticks; ++tick)
      bt_application_update(bt_time_step_policy_step_per_tick(_time_step_policy));
    bt_application_render();
  }

  bt_profile_end();
}

void bt_application_boot(const size_t num_of_args, const char *args[]) {
  (void)num_of_args; (void)args;
  fprintf(stdout, "Hello, Butane!\n");
  fprintf(stdout, " architecture = %s\n", bt_application_architecture());
  fprintf(stdout, " platform = %s\n", bt_application_platform());
  fprintf(stdout, " build = %s\n", bt_application_build());

  bt_application_set_time_step_policy(bt_time_step_policy_variable());
  bt_application_run();
  bt_application_quit();
}
