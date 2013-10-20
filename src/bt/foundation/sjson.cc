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
    #include <bt/foundation/sjson.h>
/* ========================================================================== */

/* ========================================================================== */
/*  Events:                                                                   */
/* ========================================================================== */

/* ... */

/* ========================================================================== */
/*  Parser (bt_sjson_parser_t):                                               */
/* ========================================================================== */

void bt_sjson_parser_init(
  bt_sjson_parser_t *parser,
  void *closure,
  bt_sjson_on_nil_fn on_nil,
  bt_sjson_on_boolean_fn on_boolean,
  bt_sjson_on_number_fn on_number,
  bt_sjson_on_string_fn on_string,
  bt_sjson_on_array_beg_fn on_array_beg,
  bt_sjson_on_array_end_fn on_array_end,
  bt_sjson_on_object_beg_fn on_object_beg,
  bt_sjson_on_object_end_fn on_object_end)
{
  // bt_assert(debug, parser != NULL);
  // bt_assert(debug, on_nil != NULL);
  // bt_assert(debug, on_boolean != NULL);
  // bt_assert(debug, on_number != NULL);
  // bt_assert(debug, on_string != NULL);
  // bt_assert(debug, on_array_beg != NULL);
  // bt_assert(debug, on_array_end != NULL);
  // bt_assert(debug, on_object_beg != NULL);
  // bt_assert(debug, on_object_end != NULL);

  parser->closure = closure;
  parser->on_nil = on_nil;
  parser->on_boolean = on_boolean;
  parser->on_number = on_number;
  parser->on_string = on_string;
  parser->on_array_beg = on_array_beg;
  parser->on_array_end = on_array_end;
  parser->on_object_beg = on_object_beg;
  parser->on_object_end = on_object_end;
}

/* ========================================================================== */

bt_sjson_err_t bt_sjson_parser_parse(
  const bt_sjson_parser_t *parser,
  const char *sjson,
  const size_t sjson_len)
{
  // bt_assert(debug, parser != NULL);
  // bt_assert(debug, sjson != NULL);

  return BT_SJSON_ENONE;
}
