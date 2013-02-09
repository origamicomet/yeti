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

#include <lwe/assets/package.h>
#include <lwe/asset_manager.h>

#include <libconfig.h>

typedef struct lwe_package_asset_ref_t {
  lwe_type_id_t type_id;
  lwe_hash_t hash;
} lwe_package_asset_ref_t;

typedef struct lwe_package_blob_t {
  lwe_size_t num_refs;
  lwe_package_asset_ref_t refs[1];
} lwe_package_blob_t;

static lwe_asset_t* lwe_package_load(
  lwe_type_id_t type_id,
  lwe_asset_stream_t* stream )
{
  lwe_assert(stream != NULL);
  lwe_assert(type_id == LWE_ASSET_TYPE_ID_PACKAGE);

  uint8_t* blob = (uint8_t*)lwe_asset_stream_mrd(stream);

  lwe_package_blob_t* pkg_blob =
    (lwe_package_blob_t*)(blob + 0);

  lwe_package_t* package =
    (lwe_package_t*)lwe_alloc(
      sizeof(lwe_package_t) +
      sizeof(lwe_asset_t*) * pkg_blob->num_refs -
      sizeof(lwe_asset_t)
    );

  package->num_assets = pkg_blob->num_refs;

  // TODO: package streaming.
  for (lwe_size_t i = 0; i < pkg_blob->num_refs; ++i) {
    const lwe_package_asset_ref_t* ref = &pkg_blob->refs[i];
    lwe_asset_t** asset = &package->assets[i];

    if (*asset = lwe_asset_manager_find_and_ref_by_hash(ref->hash))
      continue;

    if (*asset = lwe_asset_manager_load_by_hash(ref->type_id, ref->hash))
      continue;

    lwe_fail(
      "Invalid reference to asset, type_id=%u hash=%" LWE_HASH_FORMAT,
      ref->type_id, ref->hash
    );
  }

  lwe_asset_stream_close(stream);
  return (lwe_asset_t*)package;
}

static void lwe_package_unload(
  lwe_asset_t* asset )
{
  lwe_assert(asset != NULL);
  lwe_assert(asset->type_id == LWE_ASSET_TYPE_ID_PACKAGE);

  lwe_package_t* package = (lwe_package_t*)asset;

  for (lwe_size_t i = 0; i < package->num_assets; ++i)
    lwe_asset_manager_deref(package->assets[i]);

  lwe_free((void*)package);
}

static bool lwe_package_compile(
  lwe_type_id_t type_id,
  lwe_asset_compile_data_t* acd )
{
  config_t cfg;
  config_init(&cfg);
  config_set_include_dir(&cfg, acd->data_src);

  if (!config_read(&cfg, acd->in)) {
    config_destroy(&cfg);
    return FALSE;
  }

  config_setting_t* references =
    config_lookup(&cfg, "references");

  if (!references) {
    lwe_log("  > Expected `references`\n");
    config_destroy(&cfg);
    return FALSE;
  }

  if (!config_setting_is_array(references)) {
    lwe_log("  > Malformed `references`, expected array\n");
    config_destroy(&cfg);
    return FALSE;
  }

  lwe_package_blob_t pkg_blob;
  pkg_blob.num_refs = config_setting_length(references);

  if (fwrite((void*)&pkg_blob, sizeof(lwe_package_blob_t) - sizeof(lwe_package_asset_ref_t), 1, acd->mrd) != 1) {
    lwe_log("  > Unable to write memory-resident data.\n");
    return FALSE;
  }

  for (lwe_size_t i = 0; i < pkg_blob.num_refs; ++i) {
    lwe_const_str_t path =
      (lwe_const_str_t)config_setting_get_string_elem(references, i);

    if (!path) {
      lwe_log("  > Malformed `references`, references[%u] is not a path\n", i);
      config_destroy(&cfg);
      return FALSE;
    }

    lwe_package_asset_ref_t ref;
    ref.type_id = lwe_asset_path_to_type_id(path);
    ref.hash = lwe_murmur_hash(path, 0);

    if (fwrite((void*)&ref, sizeof(lwe_package_asset_ref_t), 1, acd->mrd) != 1) {
      lwe_log("  > Unable to write memory-resident data.\n");
      config_destroy(&cfg);
      return FALSE;
    }
  }

  config_destroy(&cfg);
  return TRUE;
}

void lwe_package_register_type()
{
  lwe_asset_register_type(
    LWE_ASSET_TYPE_ID_PACKAGE,
    "package", "package",
    &lwe_package_load,
    &lwe_package_unload,
  #if defined(LWE_DEBUG_BUILD) || defined(LWE_DEVELOPMENT_BUILD)
    &lwe_package_compile
  #else
    NULL
  #endif
  );
}