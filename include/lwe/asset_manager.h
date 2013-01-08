// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#ifndef _LWE_ASSET_MANAGER_H_
#define _LWE_ASSET_MANAGER_H_

#include <lwe/foundation.h>
#include <lwe/asset.h>

extern lwe_asset_t* lwe_asset_manager_load(
  lwe_const_str_t path );

extern lwe_asset_t* lwe_asset_manager_load_by_hash(
  lwe_type_id_t type_id,
  lwe_hash_t hash );

extern lwe_asset_t* lwe_asset_manager_find(
  lwe_const_str_t path );

extern lwe_asset_t* lwe_asset_manager_find_and_ref(
  lwe_const_str_t path );

extern lwe_asset_t* lwe_asset_manager_find_by_hash(
  lwe_hash_t hash );

extern lwe_asset_t* lwe_asset_manager_find_and_ref_by_hash(
  lwe_hash_t hash );

extern void lwe_asset_manager_deref(
  lwe_asset_t* asset );

#endif // _LWE_ASSET_MANAGER_H_