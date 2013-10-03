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
/*! @file bt/application.h
      A central point to manage global state and control the engine.          */
/* ========================================================================== */

#ifndef _BT_APPLICATION_H_
#define _BT_APPLICATION_H_

#include <bt/foundation.h>

/* ========================================================================== */
/*  Application:                                                              */
/*   * Build information                                                      */
/*   * Control (pause, unpause, etc.)                                         */
/*   * Boot and run                                                           */
/* ========================================================================== */

/* ========================================================================== */
/*  Build information:                                                        */
/* ========================================================================== */

/*! */
extern const char *bt_application_architecture();

/*! */
extern const char *bt_application_platform();

/*! */
extern const char *bt_application_build();

/* ========================================================================== */
/*  Control (pause, unpause, etc.):                                           */
/* ========================================================================== */

/*! */
extern void bt_application_pause();

/*! */
extern void bt_application_unpause();

/*! */
extern void bt_application_quit();

/* ========================================================================== */
/*  Boot and run:                                                             */
/* ========================================================================== */

/*! */
extern void bt_application_boot(
  const size_t num_of_args,
  const char *args[]);

#endif /* _BT_APPLICATION_H_ */
