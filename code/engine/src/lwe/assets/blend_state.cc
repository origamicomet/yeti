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

#include <lwe/assets/blend_state.h>

#if defined(LWE_DEBUG_BUILD) || defined(LWE_DEVELOPMENT_BUILD)

#include <libconfig.h>

static bool _string_to_blend_factor(
  lwe_const_str_t str,
  lwe_blend_factor_t* blend_factor )
{
  if (strcmp("ZERO", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_ZERO;
  else if (strcmp("ONE", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_ONE;
  else if (strcmp("SRC_COLOR", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_SRC_COLOR;
  else if (strcmp("INV_SRC_COLOR", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_INV_SRC_COLOR;
  else if (strcmp("SRC_ALPHA", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_SRC_ALPHA;
  else if (strcmp("INV_SRC_ALPHA", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_INV_SRC_ALPHA;
  else if (strcmp("DEST_ALPHA", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_DEST_ALPHA;
  else if (strcmp("INV_DEST_ALPHA", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_INV_DEST_ALPHA;
  else if (strcmp("DEST_COLOR", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_DEST_COLOR;
  else if (strcmp("INV_DEST_COLOR", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_INV_DEST_COLOR;
  else if (strcmp("SRC_ALPHA_SAT", str) == 0)
    *blend_factor = LWE_BLEND_FACTOR_SRC_ALPHA_SAT;
  else
    return false;

  return true;
}

static bool _string_to_blend_operation(
  lwe_const_str_t str,
  lwe_blend_operation_t* blend_op )
{
  if (strcmp("ADD", str) == 0)
    *blend_op = LWE_BLEND_OP_ADD;
  else if (strcmp("SUBTRACT", str) == 0)
    *blend_op = LWE_BLEND_OP_SUBTRACT;
  else if (strcmp("REV_SUBTRACT", str) == 0)
    *blend_op = LWE_BLEND_OP_REV_SUBTRACT;
  else if (strcmp("MIN", str) == 0)
    *blend_op = LWE_BLEND_OP_MIN;
  else if (strcmp("MAX", str) == 0)
    *blend_op = LWE_BLEND_OP_MAX;
  else
    return false;

  return true;
}

bool lwe_blend_state_compile(
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

  int enabled = FALSE;
  config_lookup_bool(&cfg, "enabled", &enabled);

  config_setting_t* color = config_lookup(&cfg, "color");
  if (!color) {
    lwe_log("  > Unable to find `color`\n");
    config_destroy(&cfg);
    return FALSE;
  }

  config_setting_t* alpha = config_lookup(&cfg, "alpha");
  if (!alpha) {
    lwe_log("  > Unable to find `alpha`\n");
    config_destroy(&cfg);
    return FALSE;
  }

  lwe_blend_state_blob_t bs_blob;
  bs_blob.enabled = (enabled != 0);

  /* bs_blob.color.src */ {
    const char* src_factor = NULL;
    if (!config_setting_lookup_string(color, "src", &src_factor)) {
      lwe_log("  > Unable to find `color::src`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_blend_factor((lwe_const_str_t)src_factor, &bs_blob.color.src)) {
      lwe_log("  > `%s` is not a valid blend factor\n", src_factor);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* bs_blob.color.dest */ {
    const char* dest_factor = NULL;
    if (!config_setting_lookup_string(color, "dest", &dest_factor)) {
      lwe_log("  > Unable to find `color::dest`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_blend_factor((lwe_const_str_t)dest_factor, &bs_blob.color.dest)) {
      lwe_log("  > `%s` is not a valid blend factor\n", dest_factor);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* bs_blob.color.op */ {
    const char* operation = NULL;
    if (!config_setting_lookup_string(color, "operation", &operation)) {
      lwe_log("  > Unable to find `color::operation`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_blend_operation((lwe_const_str_t)operation, &bs_blob.color.op)) {
      lwe_log("  > `%s` is not a valid blend operation\n", operation);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* bs_blob.alpha.src */ {
    const char* src_factor = NULL;
    if (!config_setting_lookup_string(alpha, "src", &src_factor)) {
      lwe_log("  > Unable to find `alpha::src`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_blend_factor((lwe_const_str_t)src_factor, &bs_blob.alpha.src)) {
      lwe_log("  > `%s` is not a valid blend factor\n", src_factor);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* bs_blob.alpha.dest */ {
    const char* dest_factor = NULL;
    if (!config_setting_lookup_string(alpha, "dest", &dest_factor)) {
      lwe_log("  > Unable to find `alpha::dest`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_blend_factor((lwe_const_str_t)dest_factor, &bs_blob.alpha.dest)) {
      lwe_log("  > `%s` is not a valid blend factor\n", dest_factor);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* bs_blob.alpha.op */ {
    const char* operation = NULL;
    if (!config_setting_lookup_string(alpha, "operation", &operation)) {
      lwe_log("  > Unable to find `alpha::operation`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_blend_operation((lwe_const_str_t)operation, &bs_blob.alpha.op)) {
      lwe_log("  > `%s` is not a valid blend operation\n", operation);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  config_destroy(&cfg);

  if (fwrite((void*)&bs_blob, sizeof(lwe_blend_state_blob_t), 1, acd->mrd) != 1) {
    lwe_log("  > Unable to write memory-resident data.\n");
    return FALSE;
  }

  return TRUE;
}

#endif