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

#include <lwe/d3d11_input_layout.h>
#include <lwe/assets/d3d11_vertex_shader.h>
#include <lwe/d3d11_render_device.h>

static const UINT input_element_lens[] = {
  12, 12, 12, 12, 12, 12, 12, 4, 4
};

static const D3D11_INPUT_ELEMENT_DESC input_element_descs[] = {
  { "POSITION",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "COLOR",        0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TEXCOORD",     1, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "NORMAL",       0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "TANGENT",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "BINORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "BLENDINDICES", 0, DXGI_FORMAT_R8G8B8A8_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  { "BLENDWEIGHT",  0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

lwe_input_layout_t* lwe_input_layout_create(
  lwe_vertex_shader_t* vertex_shader_,
  lwe_vertex_declaration_t vertex_decl )
{
  lwe_assert(vertex_shader_ != NULL);
  lwe_assert(vertex_shader_->vertex_decl.components != 0);
  lwe_assert(vertex_decl.components != 0);

  lwe_fail_if(
    (~(vertex_decl.components) & vertex_shader_->vertex_decl.components) != 0,
    "Failed to create semantic mapping between vertex shader and vertex data!"
  );

  UINT offset = 0;
  UINT num_input_elements = 0;
  UINT component_offsets[9] = { 0, };

  // Determine offsets per data component:
  uint32_t components = vertex_decl.components;
  for (; components; num_input_elements++) {
    const UINT input_element = lwe_count_trailing_zeros(components);
    component_offsets[input_element] = offset;
    offset += input_element_lens[input_element];
    components &= (components - 1);
  }

  // Match data components with shader components:
  D3D11_INPUT_ELEMENT_DESC ieds[9];
  components = vertex_shader_->vertex_decl.components;
  for (num_input_elements = 0; components; num_input_elements++) {
    const UINT input_element = lwe_count_trailing_zeros(components);

    ieds[num_input_elements].SemanticName =
      input_element_descs[input_element].SemanticName;

    ieds[num_input_elements].SemanticIndex =
      input_element_descs[input_element].SemanticIndex;

    ieds[num_input_elements].Format =
      input_element_descs[input_element].Format;

    ieds[num_input_elements].InputSlot =
      input_element_descs[input_element].InputSlot;

    ieds[num_input_elements].AlignedByteOffset =
      component_offsets[input_element];

    ieds[num_input_elements].InputSlotClass =
      input_element_descs[input_element].InputSlotClass;

    ieds[num_input_elements].InstanceDataStepRate =
      input_element_descs[input_element].InstanceDataStepRate;

    components &= (components - 1);
  }

  HRESULT hr;
  lwe_d3d11_input_layout_t* input_layout =
    (lwe_d3d11_input_layout_t*)lwe_alloc(sizeof(lwe_d3d11_input_layout_t));

  lwe_d3d11_vertex_shader_t* vertex_shader =
    (lwe_d3d11_vertex_shader_t*)vertex_shader_;

  lwe_fail_if(FAILED(
    hr = _d3d11_device->CreateInputLayout(
      &ieds[0],
      num_input_elements,
      vertex_shader->byte_code,
      vertex_shader->byte_code_len,
      &input_layout->input_layout
    )),

    "ID3D11Device::CreateInputLayout failed, hr=%#08X", hr
  );

  input_layout->stride = lwe_vertex_declaration_to_size(vertex_decl);
  return (lwe_input_layout_t*)input_layout;
}

void lwe_input_layout_destroy(
  lwe_input_layout_t* input_layout_ )
{
  lwe_assert(input_layout_ != NULL);

  lwe_d3d11_input_layout_t* input_layout =
    (lwe_d3d11_input_layout_t*)input_layout_;

  input_layout->input_layout->Release();

  lwe_free((void*)input_layout);
}