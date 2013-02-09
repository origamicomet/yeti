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

#include <lwe/assets/depth_stencil_state.h>

#if defined(LWE_DEBUG_BUILD) || defined(LWE_DEVELOPMENT_BUILD)

#include <libconfig.h>

static bool _string_to_comparison_func(
  lwe_const_str_t str,
  lwe_comparison_func_t* comparison_func )
{
  if (strcmp("NEVER", str) == 0)
    *comparison_func = LWE_COMPARISON_NEVER;
  else if (strcmp("LESS", str) == 0)
    *comparison_func = LWE_COMPARISON_LESS;
  else if (strcmp("EQUAL", str) == 0)
    *comparison_func = LWE_COMPARISON_EQUAL;
  else if (strcmp("LESS_EQUAL", str) == 0)
    *comparison_func = LWE_COMPARISON_LESS_EQUAL;
  else if (strcmp("GREATER", str) == 0)
    *comparison_func = LWE_COMPARISON_GREATER;
  else if (strcmp("NOT_EQUAL", str) == 0)
    *comparison_func = LWE_COMPARISON_NOT_EQUAL;
  else if (strcmp("GREATER_EQUAL", str) == 0)
    *comparison_func = LWE_COMPARISON_GREATER_EQUAL;
  else if (strcmp("ALWAYS", str) == 0)
    *comparison_func = LWE_COMPARISON_ALWAYS;
  else
    return false;

  return true;
}

static bool _string_to_stencil_operation(
  lwe_const_str_t str,
  lwe_stencil_operation_t* stencil_op )
{
  if (strcmp("KEEP", str) == 0)
    *stencil_op = LWE_STENCIL_OP_KEEP;
  else if (strcmp("ZERO", str) == 0)
    *stencil_op = LWE_STENCIL_OP_ZERO;
  else if (strcmp("REPLACE", str) == 0)
    *stencil_op = LWE_STENCIL_OP_REPLACE;
  else if (strcmp("INCR_SAT", str) == 0)
    *stencil_op = LWE_STENCIL_OP_INCR_SAT;
  else if (strcmp("DECR_SAT", str) == 0)
    *stencil_op = LWE_STENCIL_OP_DECR_SAT;
  else if (strcmp("INVERT", str) == 0)
    *stencil_op = LWE_STENCIL_OP_INVERT;
  else if (strcmp("INCR", str) == 0)
    *stencil_op = LWE_STENCIL_OP_INCR;
  else if (strcmp("DECR", str) == 0)
    *stencil_op = LWE_STENCIL_OP_DECR;
  else
    return false;

  return true;
}

bool lwe_depth_stencil_state_compile(
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

  int depth_tests = TRUE;
  config_lookup_bool(&cfg, "depth_tests", &depth_tests);
  
  int depth_writes = TRUE;
  config_lookup_bool(&cfg, "depth_writes", &depth_writes);

  lwe_comparison_func_t depth_func = LWE_COMPARISON_LESS; {
    const char* str = NULL;
    if (config_lookup_string(&cfg, "depth_func", &str)) {
      if (!_string_to_comparison_func((lwe_const_str_t)str, &depth_func)) {
        lwe_log("  > `%s` is an invalid depth function\n", str);
        config_destroy(&cfg);
        return FALSE;
      }
    }
  }

  int stencil_enabled = FALSE;
  config_lookup_bool(&cfg, "stencil_enabled", &stencil_enabled);

  config_setting_t* front_face = config_lookup(&cfg, "front_face");
  if (!front_face) {
    lwe_log("  > Unable to find `front_face`\n");
    config_destroy(&cfg);
    return FALSE;
  }

  config_setting_t* back_face = config_lookup(&cfg, "back_face");
  if (!back_face) {
    lwe_log("  > Unable to find `back_face`\n");
    config_destroy(&cfg);
    return FALSE;
  }

  lwe_depth_stencil_state_blob_t dss_blob;
  dss_blob.depth.tests = depth_tests;
  dss_blob.depth.writes = depth_writes;
  dss_blob.depth.func = depth_func;
  dss_blob.stencil.enabled = stencil_enabled;

  /* dss_blob.stencil.front_face.stencil_fail_op */ {
    const char* stencil_fail_op = NULL;
    if (!config_setting_lookup_string(front_face, "stencil_fail_op", &stencil_fail_op)) {
      lwe_log("  > Unable to find `front_face::stencil_fail_op`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_stencil_operation((lwe_const_str_t)stencil_fail_op, &dss_blob.stencil.front_face.stencil_fail_op)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", stencil_fail_op);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* dss_blob.stencil.front_face.depth_fail_op */ {
    const char* depth_fail_op = NULL;
    if (!config_setting_lookup_string(front_face, "depth_fail_op", &depth_fail_op)) {
      lwe_log("  > Unable to find `front_face::depth_fail_op`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_stencil_operation((lwe_const_str_t)depth_fail_op, &dss_blob.stencil.front_face.depth_fail_op)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", depth_fail_op);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* dss_blob.stencil.front_face.pass_op */ {
    const char* pass_op = NULL;
    if (!config_setting_lookup_string(front_face, "pass_op", &pass_op)) {
      lwe_log("  > Unable to find `front_face::pass_op`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_stencil_operation((lwe_const_str_t)pass_op, &dss_blob.stencil.front_face.pass_op)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", pass_op);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* dss_blob.stencil.front_face.func */ {
    const char* func = NULL;
    if (!config_setting_lookup_string(front_face, "func", &func)) {
      lwe_log("  > Unable to find `front_face::func`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_comparison_func((lwe_const_str_t)func, &dss_blob.stencil.front_face.func)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", func);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* dss_blob.stencil.back_face.stencil_fail_op */ {
    const char* stencil_fail_op = NULL;
    if (!config_setting_lookup_string(back_face, "stencil_fail_op", &stencil_fail_op)) {
      lwe_log("  > Unable to find `back_face::stencil_fail_op`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_stencil_operation((lwe_const_str_t)stencil_fail_op, &dss_blob.stencil.back_face.stencil_fail_op)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", stencil_fail_op);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* dss_blob.stencil.back_face.depth_fail_op */ {
    const char* depth_fail_op = NULL;
    if (!config_setting_lookup_string(back_face, "depth_fail_op", &depth_fail_op)) {
      lwe_log("  > Unable to find `back_face::depth_fail_op`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_stencil_operation((lwe_const_str_t)depth_fail_op, &dss_blob.stencil.back_face.depth_fail_op)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", depth_fail_op);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* dss_blob.stencil.back_face.pass_op */ {
    const char* pass_op = NULL;
    if (!config_setting_lookup_string(back_face, "pass_op", &pass_op)) {
      lwe_log("  > Unable to find `back_face::pass_op`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_stencil_operation((lwe_const_str_t)pass_op, &dss_blob.stencil.back_face.pass_op)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", pass_op);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* dss_blob.stencil.back_face.func */ {
    const char* func = NULL;
    if (!config_setting_lookup_string(back_face, "func", &func)) {
      lwe_log("  > Unable to find `back_face::func`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_comparison_func((lwe_const_str_t)func, &dss_blob.stencil.back_face.func)) {
      lwe_log("  > `%s` is not a valid stencil operation\n", func);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  config_destroy(&cfg);

  if (fwrite((void*)&dss_blob, sizeof(lwe_depth_stencil_state_blob_t), 1, acd->mrd) != 1) {
    lwe_log("  > Unable to write memory-resident data.\n");
    return FALSE;
  }

  return TRUE;
}

#endif