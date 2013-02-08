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

#ifndef _LWE_ASSET_COMPILER_H_
#define _LWE_ASSET_COMPILER_H_

#include <lwe/foundation.h>

/// Compiles the specified asset.
extern void lwe_asset_compiler_compile(
  lwe_const_str_t data,
  lwe_const_str_t data_src,
  lwe_const_str_t path );

/// Recursively compiles assets in a directory.
extern void lwe_asset_compiler_compile_dir(
  lwe_const_str_t data,
  lwe_const_str_t data_src );

/// Recursively watches a directory for modifications and re-compiles.
extern void lwe_asset_compiler_watch_dir(
  lwe_const_str_t data,
  lwe_const_str_t data_src );

#endif // _LWE_ASSET_COMPILER_H_