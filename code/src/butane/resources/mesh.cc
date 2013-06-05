// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/mesh.h>

namespace butane {
  static Allocator& allocator() {
    static ProxyAllocator allocator("mesh resources", Allocators::heap());
    return allocator;
  }

  const Resource::Type MeshResource::type(
    "mesh", "mesh",
    (Resource::Type::Load)&MeshResource::load,
    (Resource::Type::Unload)&MeshResource::unload,
    (Resource::Type::Compile)&MeshResource::compile);

  MeshResource::MeshResource(
    const Resource::Id id
  ) : butane::Resource(MeshResource::type, id)
    , _materials(allocator())
    , _vertices(nullptr)
    , _indicies(nullptr)
    , _num_of_indicies(0)
  {
  }

  MeshResource::~MeshResource()
  {
    if (_vertices)
      _vertices->destroy();
    if (_indicies)
      _indicies->destroy();
  }

  MeshResource* MeshResource::load(
    const Resource::Id id,
    const Resource::Stream& stream )
  {
    const LogScope _("MeshResource::load");

    const MemoryResidentData& mrd =
      *((const MemoryResidentData*)stream.memory_resident_data());

    MeshResource* mesh =
      make_new(MeshResource, allocator())(id);

    mesh->_vertex_declaration = mrd.vertex_declaration;

    mesh->_materials.resize(mrd.num_of_materials);

    const MemoryResidentData::Material* materials =
      ((const MemoryResidentData::Material*)((uintptr_t)&mrd) + sizeof(MemoryResidentData));

    for (size_t m = 0; m < mrd.num_of_materials; ++m) {
      mesh->_materials[m].name = materials[m].name;

      for (size_t t = 0; t < 8; ++t) {
        if (materials[m].textures[t] == Resource::Id())
          break;
        mesh->_materials[m].textures[t] = materials[m].textures[t]; }
    }

    const void* vertices =
      (const void*)(((uintptr_t)materials) + mrd.num_of_materials * sizeof(MemoryResidentData::Material));
    const size_t vertices_len = mrd.num_of_vertices * mrd.vertex_declaration.size();
    mesh->_vertices = VertexBuffer::create(vertices, vertices_len);

    const void* indicies =
      (const void*)(((uintptr_t)vertices) + vertices_len);
    const size_t indicies_len = mrd.num_of_indicies * sizeof(uint32_t);
    mesh->_num_of_indicies = mrd.num_of_indicies;
    mesh->_indicies = IndexBuffer::create(indicies, indicies_len);

    return mesh;
  }

  void MeshResource::unload(
    MeshResource* mesh )
  {
    const LogScope _("MeshResource::unload");

    assert(mesh != nullptr);
    make_delete(MeshResource, allocator(), mesh);
  }

  bool MeshResource::compile(
    const Resource::Compiler::Input& input,
    const Resource::Compiler::Output& output )
  {
    const LogScope _("MeshResource::compile");

    MemoryResidentData mrd;

    if (fscanf(input.data, "%u", &mrd.num_of_materials) != 1)
      return false;

    unsigned num_of_vertex_components = 0;

    if (fscanf(input.data, "%u", &num_of_vertex_components) != 1)
      return false;

    if (num_of_vertex_components == 0)
      return false;

    for (unsigned c = 0; c < num_of_vertex_components; ++c) {
      char component[256] = { 0, };
      if (fscanf(input.data, "%255s", &component[0]) != 1)
          return false;

      uint32_t bit = 0;
      if (strncmp("POSITION", &component[0], 8) == 0)
        bit = VertexDeclaration::POSITION;
      else if (strncmp("COLOR", &component[0], 5) == 0)
        bit = VertexDeclaration::COLOR0 << (component[5] - '0');
      else if (strncmp("TEXCOORD", &component[0], 8) == 0)
        bit = VertexDeclaration::TEXCOORD0 << (component[8] - '0');
      else if (strncmp("NORMAL", &component[0], 6) == 0)
        bit = VertexDeclaration::NORMAL;
      else if (strncmp("TANGENT", &component[0], 7) == 0)
        bit = VertexDeclaration::TANGENT;
      else if (strncmp("BINORMAL", &component[0], 8) == 0)
        bit = VertexDeclaration::BITANGENT;
      // else if (strncmp("BONEINDICES", &component[0], 11) == 0)
      //   bit = VertexDeclaration::BONEINDICES;
      // else if (strncmp("BONEWEIGHTS", &component[0], 11) == 0)
      //   bit = VertexDeclaration::BONEWEIGHTS;
      else
        return false;

      if (mrd.vertex_declaration.components() > bit)
        return false;

      mrd.vertex_declaration.components() |= bit;
    }

    if (fscanf(input.data, "%u", &mrd.num_of_vertices) != 1)
      return false;

    if (fscanf(input.data, "%u", &mrd.num_of_indicies) != 1)
      return false;

    if (!File::write_out(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData)))
      return false;

    Array<MemoryResidentData::Material> materials(Allocators::heap());
    materials.resize(max(mrd.num_of_materials, (size_t)1));

    if (mrd.num_of_materials == 0) {
      zero(&materials[0], sizeof(MemoryResidentData::Material));
      materials[0].name = Material::Name("default");
      materials[0].textures[1] = Resource::Id(TextureResource::type, "textures/not_found");
    } else {
      for (uint32_t m = 0; m < mrd.num_of_materials; ++m) {
        zero(&materials[m], sizeof(MemoryResidentData::Material));

        char material[256] = { 0, };
        if (fscanf(input.data, "%255s", &material[0]) != 1)
          return false;
        materials[m].name = Material::Name(&material[0]);

        unsigned num_of_textures = 0;
        if (fscanf(input.data, "%u", &num_of_textures) != 1)
          return false;

        for (unsigned t = 0; t < num_of_textures; ++t) {
          char name[256] = { 0, };
          const int offs = snprintf(&name[0], 256, "%s/%s/", output.path, &material[0]);
          if (fscanf(input.data, "%255s", &name[offs]) != 1)
            return false;
          materials[m].textures[t] = Resource::Id(TextureResource::type, &name[0]);
        }
      }
    }

    if (!File::write_out(output.memory_resident_data, materials.raw(), mrd.num_of_materials * sizeof(Material)))
      return false;

    for (uint32_t v = 0; v < mrd.num_of_vertices; ++v) {
      if (mrd.vertex_declaration & VertexDeclaration::POSITION) {
        float xyz[3];
        if (fscanf(input.data, "%f %f %f", &xyz[0], &xyz[1], &xyz[2]) != 3)
          return false;
        if (!File::write_out(output.memory_resident_data, (const void*)&xyz[0], 12))
          return false;
      }

      for (uint32_t channel = 0; channel < 8; ++channel) {
        if (mrd.vertex_declaration & (VertexDeclaration::COLOR0 << channel)) {
          float rgb[3];
          if (fscanf(input.data, "%f %f %f", &rgb[0], &rgb[1], &rgb[2]) != 3)
            return false;
          if (!File::write_out(output.memory_resident_data, (const void*)&rgb[0], 12))
            return false; }
      }

      for (uint32_t channel = 0; channel < 8; ++channel) {
        if (mrd.vertex_declaration & (VertexDeclaration::TEXCOORD0 << channel)) {
          float uvw[3];
          if (fscanf(input.data, "%f %f %f", &uvw[0], &uvw[1], &uvw[2]) != 3)
            return false;
          if (!File::write_out(output.memory_resident_data, (const void*)&uvw[0], 12))
            return false; }
      }

      for (uint32_t basis = 0; basis < 3; ++basis) {
        if (mrd.vertex_declaration & (VertexDeclaration::NORMAL << basis)) {
          float xyz[3];
          if (fscanf(input.data, "%f %f %f", &xyz[0], &xyz[1], &xyz[2]) != 3)
            return false;
          if (!File::write_out(output.memory_resident_data, (const void*)&xyz[0], 12))
            return false; }
      }
    }

    for (uint32_t i = 0; i < mrd.num_of_indicies; ++i) {
      uint32_t index = 0;
      if (fscanf(input.data, "%u", &index) != 1)
        return false;
      if (!File::write_out(output.memory_resident_data, (const void*)&index, 4))
        return false;
    }

    return true;
  }
} // butane
