// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include "shaders/globals.vs.include"

struct VS_INPUT {
  float3 position  : POSITION;
  float4 color     : COLOR0;
  float3 tex_coord : TEXCOORD0;
};

struct VS_OUTPUT {
  float4 position  : POSITION;
  float3 color     : COLOR0;
  float2 tex_coord : TEXCOORD0;
};

VS_OUTPUT vs_main( VS_INPUT IN )
{
  VS_OUTPUT OUT;
  OUT.position = float4(IN.position, 1.0); // mul(float4(IN.position, 1.0), model_view_proj);
  OUT.color = IN.color.xyz;
  OUT.tex_coord = IN.tex_coord.xy;
  return OUT;
}
