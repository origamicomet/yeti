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
/*! @file bt/foundation/type_info.h
      Provides a system for identifying object type at compile and run time.  */
/* ========================================================================== */

#ifndef _BT_FOUNDATION_TYPE_INFO_H_
#define _BT_FOUNDATION_TYPE_INFO_H_

#include <bt/foundation/compat.h>
#include <bt/foundation/hash.h>

/* ========================================================================== */
/*  Type Info:                                                                */
/* ========================================================================== */

typedef uint32_t bt_type_id_t;

typedef struct bt_type_info {
  /* ======================================================================== */
  bt_type_id_t id;
  const char *name;
  /* ======================================================================== */
  bt_type_info(const bt_type_id_t id, const char *name) : id(id), name(name) {}
  bt_type_info(const bt_type_info &type_info) : id(type_info.id), name(type_info.name) {}
  bt_type_info &operator= (const bt_type_info &type_info) { id = type_info.id; name = type_info.name; return *this; }
} bt_type_info_t;

typedef struct bt_has_type_info_t {
  /* ======================================================================== */
  const bt_type_info_t *__type_info;
  /* ======================================================================== */
  bt_has_type_info_t(const bt_type_info_t *type_info) : __type_info(type_info) {}
  bt_has_type_info_t(const bt_has_type_info_t &type_info) : __type_info(type_info.__type_info) {}
  bt_has_type_info_t &operator= (const bt_has_type_info_t &type_info) { __type_info = type_info.__type_info; return *this; }
} bt_has_type_info_t;

template <typename _Type>
struct bt_has_type_info_t_ : public bt_has_type_info_t {
  /* ======================================================================== */
  static const bt_type_info_t _;
  /* ======================================================================== */
  bt_has_type_info_t_() : bt_has_type_info_t(&bt_has_type_info_t_<_Type>::_) {}
  bt_has_type_info_t_(const bt_has_type_info_t_ &type_info) {}
  bt_has_type_info_t_ &operator= (const bt_has_type_info_t_ &type_info) { return *this; }
};

#define bt_has_type_info(_Type) \
  : public bt_has_type_info_t_<_Type>

#define bt_def_type_info(_Type) \
  template <> \
  const bt_type_info_t bt_has_type_info_t_<_Type>::_ = \
    bt_type_info_t(bt_murmur_hash_str(#_Type, 0), #_Type);

#define bt_init_type_info(_Type, _Ptr) \
  do { (((bt_has_type_info_t *)_Ptr)->__type_info = &bt_has_type_info_t_<_Type>::_) } while(0)

#define bt_type_info(_Ptr) \
  (((const bt_has_type_info_t *)_Ptr)->__type_info)

#define bt_static_type_info(_Type) \
  (&bt_has_type_info_t_<_Type>::_)

#define bt_type_id(_Ptr) \
  (bt_type_info(_Ptr)->id)

#define bt_static_type_id(_Type) \
  (bt_static_type_info(_Type)->id)

#define bt_type_name(_Ptr) \
  (bt_type_info(_Ptr)->name)

#define bt_static_type_name(_Type) \
  (bt_static_type_info(_Type)->name)

#endif /* _BT_FOUNDATION_TYPE_INFO_H_ */
