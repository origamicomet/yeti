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
/*! @file bt/foundation/sjson.h
      Provides an incredibly fast evented sjson parser.                       */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_SJSON_H_
#define _BT_FOUNDATION_SJSON_H_

#include <bt/foundation/compat.h>

/* ========================================================================== */
/*  Simplified JSON:                                                          */
/*   * Events                                                                 */
/*   * Parser                                                                 */
/* ========================================================================== */

/* ========================================================================== */
/*  Events:                                                                   */
/* ========================================================================== */

typedef bool (*bt_sjson_on_nil_fn)(
  void *closure, const char *key);
typedef bool (*bt_sjson_on_boolean_fn)(
  void *closure, const char *key, bool value);
typedef bool (*bt_sjson_on_number_fn)(
  void *closure, const char *key, double value);
typedef bool (*bt_sjson_on_string_fn)(
  void *closure, const char *key, const char *value, const size_t value_len);
typedef bool (*bt_sjson_on_array_beg_fn)(
  void *closure, const char *key);
typedef bool (*bt_sjson_on_array_end_fn)(
  void *closure);
typedef bool (*bt_sjson_on_object_beg_fn)(
  void *closure, const char *key);
typedef bool (*bt_sjson_on_object_end_fn)(
  void *closure);

/* ========================================================================== */
/*  Parser (bt_sjson_parser_t):                                               */
/* ========================================================================== */

/*! */
typedef enum bt_sjson_err {
  /*! Successful; no error. */
  BT_SJSON_ENONE     = 0,
  /*! Invalid or unexpected character. */
  BT_SJSON_EINVALCHR = -1,
} bt_sjson_err_t;

/* ========================================================================== */

/*! */
typedef struct bt_sjson_parser {
  void *closure;
  bt_sjson_on_nil_fn on_nil;
  bt_sjson_on_boolean_fn on_boolean;
  bt_sjson_on_number_fn on_number;
  bt_sjson_on_string_fn on_string;
  bt_sjson_on_array_beg_fn on_array_beg;
  bt_sjson_on_array_end_fn on_array_end;
  bt_sjson_on_object_beg_fn on_object_beg;
  bt_sjson_on_object_end_fn on_object_end;
} bt_sjson_parser_t;

/* ========================================================================== */

/*! Initializes `parser` to use events. */
extern void bt_sjson_parser_init(
  bt_sjson_parser_t *parser,
  void *closure,
  bt_sjson_on_nil_fn on_nil,
  bt_sjson_on_boolean_fn on_boolean,
  bt_sjson_on_number_fn on_number,
  bt_sjson_on_string_fn on_string,
  bt_sjson_on_array_beg_fn on_array_beg,
  bt_sjson_on_array_end_fn on_array_end,
  bt_sjson_on_object_beg_fn on_object_beg,
  bt_sjson_on_object_end_fn on_object_end);

/* ========================================================================== */

/*! @def BT_SJSON_UNSPECIFIED_LEN ... */
#define BT_SJSON_UNSPECIFIED_LEN ((size_t)0xffffffffffff)

/*! Parses `sjson`, calling events specified bt bt_sjson_parser_init. */
extern bt_sjson_err_t bt_sjson_parser_parse(
  const bt_sjson_parser_t *parser,
  const char *sjson,
  const size_t sjson_len);

#endif /* _BT_FOUNDATION_SJSON_H_ */
