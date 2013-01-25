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

#include <lwe/assets/rasterizer_state.h>

#include <libconfig.h>

static bool _string_to_fill_mode(
  lwe_const_str_t str,
  lwe_fill_mode_t* fill_mode )
{
  if (strcmp("WIREFRAME", str) == 0)
    *fill_mode = LWE_FILL_WIREFRAME;
  else if (strcmp("SOLID", str) == 0)
    *fill_mode = LWE_FILL_SOLID;
  else
    return false;

  return true;
}

static bool _string_to_cull_mode(
  lwe_const_str_t str,
  lwe_cull_mode_t* cull_mode )
{
  if (strcmp("NONE", str) == 0)
    *cull_mode = LWE_CULL_NONE;
  else if (strcmp("FRONT", str) == 0)
    *cull_mode = LWE_CULL_FRONT;
  else if (strcmp("BACK", str) == 0)
    *cull_mode = LWE_CULL_BACK;
  else
    return false;

  return true;
}

static bool _string_to_front_face(
  lwe_const_str_t str,
  lwe_front_face_t* front_face )
{
  if (strcmp("CW", str) == 0)
    *front_face = LWE_FRONT_FACE_CW;
  else if (strcmp("CCW", str) == 0)
    *front_face = LWE_FRONT_FACE_CCW;
  else
    return false;

  return true;
}

bool lwe_rasterizer_state_compile(
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

  lwe_rasterizer_state_blob_t rs_blob;

  /* rs_blob.fill_mode */ {
    const char* fill_mode = NULL;
    if (!config_lookup_string(&cfg, "fill_mode", &fill_mode)) {
      lwe_log("  > Unable to find `color::fill_mode`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_fill_mode((lwe_const_str_t)fill_mode, &rs_blob.fill_mode)) {
      lwe_log("  > `%s` is not a valid blend factor\n", fill_mode);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* rs_blob.cull_mode */ {
    const char* cull_mode = NULL;
    if (!config_lookup_string(&cfg, "cull_mode", &cull_mode)) {
      lwe_log("  > Unable to find `color::cull_mode`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_cull_mode((lwe_const_str_t)cull_mode, &rs_blob.cull_mode)) {
      lwe_log("  > `%s` is not a valid blend factor\n", cull_mode);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  /* rs_blob.front_face */ {
    const char* front_face = NULL;
    if (!config_lookup_string(&cfg, "front_face", &front_face)) {
      lwe_log("  > Unable to find `color::front_face`\n");
      config_destroy(&cfg);
      return FALSE;
    }

    if (!_string_to_front_face((lwe_const_str_t)front_face, &rs_blob.front_face)) {
      lwe_log("  > `%s` is not a valid blend factor\n", front_face);
      config_destroy(&cfg);
      return FALSE;
    }
  }

  int scissor_enabled = FALSE;
  config_lookup_bool(&cfg, "scissor_enabled", &scissor_enabled);

  rs_blob.scissor_enabled = (scissor_enabled != FALSE);

  config_destroy(&cfg);

  if (fwrite((void*)&rs_blob, sizeof(lwe_rasterizer_state_blob_t), 1, acd->mrd) != 1) {
    lwe_log("  > Unable to write memory-resident data.\n");
    return FALSE;
  }

  return TRUE;
}