// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include "shaders/globals.vs.include"

struct VS_INPUT {
  float3 position : POSITION;
};

struct VS_OUTPUT {
  float4 position : POSITION;
};

VS_OUTPUT vs_main( VS_INPUT IN )
{
  VS_OUTPUT OUT;
  OUT.position = mul(float4(IN.position, 1.0), model_view_proj);
  return OUT;
}
