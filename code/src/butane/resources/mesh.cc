// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#include <butane/resources/mesh.h>

namespace butane {
  static Allocator& __allocator_initializer() {
    static ProxyAllocator allocator("mesh resources", Allocators::heap());
    return allocator;
  }

  static const thread_safe::Static< Allocator >
    __ts_allocator(&__allocator_initializer);

  static Allocator& allocator() {
    return __ts_allocator();
  }

  static const Resource::Type& __type_initializer() {
    static const Resource::Type type(
      "mesh", "mesh",
      (Resource::Type::Load)&MeshResource::load,
      (Resource::Type::Unload)&MeshResource::unload,
      (Resource::Type::Compile)&MeshResource::compile);
    return type;
  }

  static const thread_safe::Static< const Resource::Type >
    __ts_type(&__type_initializer);

  const Resource::Type& MeshResource::type() {
    return __ts_type();
  }

  MeshResource::MeshResource(
    const Resource::Id id
  ) : butane::Resource(MeshResource::type(), id)
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

    const MemoryResidentData* mrd =
      ((const MemoryResidentData*)stream.memory_resident_data());

    MeshResource* mesh =
      make_new(MeshResource, allocator())(id);

    mesh->_vertex_declaration = mrd->vertex_declaration;

    const MemoryResidentData::Material* materials = mrd->materials;
    mesh->_materials.resize(mrd->num_of_materials);
    for (size_t m = 0; m < mrd->num_of_materials; ++m) {
      mesh->_materials[m].name = materials[m].name;
      mesh->_materials[m].shader = materials[m].shader;
      mesh->_materials[m].num_of_textures = 0;
      for (size_t t = 0; t < 8; ++t) {
        if (materials[m].textures[t] == Resource::Id())
          break;
        mesh->_materials[m].textures[t] = materials[m].textures[t];
        ++mesh->_materials[m].num_of_textures;
      }
    }

    const void* vertices = mrd->vertices;
    const size_t vertices_len = mrd->num_of_vertices * mrd->vertex_declaration.size();
    mesh->_vertices = VertexBuffer::create(vertices, vertices_len);

    const void* indicies = mrd->indicies;
    const size_t indicies_len = mrd->num_of_indicies * sizeof(uint32_t);
    mesh->_num_of_indicies = mrd->num_of_indicies;
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

    if (fscanf(input.data, "%u", &mrd.num_of_materials) != 1) {
      output.log("Malformed input: expected number of materials!");
      return false; }

    unsigned num_of_vertex_components = 0;

    if (fscanf(input.data, "%u", &num_of_vertex_components) != 1) {
      output.log("Malformed input: expected number of vertex components!");
      return false; }

    if (num_of_vertex_components == 0) {
      output.log("Malformed input: expected one or more vertex components!");
      return false; }

    for (unsigned c = 0; c < num_of_vertex_components; ++c) {
      char component[256] = { 0, };
      if (fscanf(input.data, "%255s", &component[0]) != 1) {
        output.log("Malformed input: expected vertex component (%u/%u)", c, num_of_vertex_components);
        return false; }

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
      else if (strncmp("BITANGENT", &component[0], 8) == 0)
        bit = VertexDeclaration::BITANGENT;
      // else if (strncmp("BONEINDICES", &component[0], 11) == 0)
      //   bit = VertexDeclaration::BONEINDICES;
      // else if (strncmp("BONEWEIGHTS", &component[0], 11) == 0)
      //   bit = VertexDeclaration::BONEWEIGHTS;
      else {
        output.log("Malformed input: unknown vertex component '%s'", &component[0]);
        return false; }

      if (mrd.vertex_declaration.components() > bit) {
        output.log("Malformed input: expected vertex components to be sequential!");
        return false; }

      mrd.vertex_declaration.components() |= bit;
    }

    if (fscanf(input.data, "%u", &mrd.num_of_vertices) != 1) {
      output.log("Malformed input: expected number of vertices!");
      return false; }

    if (fscanf(input.data, "%u", &mrd.num_of_indicies) != 1) {
      output.log("Malformed input: expected number of indicies!");
      return false; }

    const bool use_default_material = (mrd.num_of_materials == 0);
    if (use_default_material)
      mrd.num_of_materials += 1;

    {
      int64_t offset = sizeof(MemoryResidentData);
      mrd.materials = relative_ptr<MemoryResidentData::Material*>(offset - offsetof(MemoryResidentData, materials));
      offset += mrd.num_of_materials * sizeof(MemoryResidentData::Material);
      mrd.vertices = relative_ptr<void*>(offset - offsetof(MemoryResidentData, vertices));
      offset += mrd.num_of_vertices * mrd.vertex_declaration.size();
      mrd.indicies = relative_ptr<void*>(offset - offsetof(MemoryResidentData, indicies));
      // offset += mrd.num_of_indicies * sizeof(uint32_t);
    }

    if (!File::write(output.memory_resident_data, (const void*)&mrd, sizeof(MemoryResidentData))) {
      output.log("Unable to write to memory-resident data!");
      return false; }

    Array<MemoryResidentData::Material> materials(Allocators::heap());
    materials.resize(mrd.num_of_materials);

    if (use_default_material) {
      zero(&materials[0], sizeof(MemoryResidentData::Material));
      materials[0].name = Material::Name("default");
      materials[0].shader = Resource::Id(ShaderResource::type(), "shaders/mesh");
      materials[0].textures[0] = Resource::Id(TextureResource::type(), "textures/not_found");
    } else {
      for (uint32_t m = 0; m < mrd.num_of_materials; ++m) {
        zero(&materials[m], sizeof(MemoryResidentData::Material));

        char material[256] = { 0, };
        if (fscanf(input.data, "%255s", &material[0]) != 1)
          return false;
        materials[m].name = Material::Name(&material[0]);

        /* materials[m].shader = */ {
          char shader[256] = { 0, };
          if (fscanf(input.data, "%255s", &shader[0]) != 1)
            return false;
          materials[m].shader = Resource::Id(ShaderResource::type(), &shader[0]);
        }

        unsigned num_of_textures = 0;
        if (fscanf(input.data, "%u", &num_of_textures) != 1)
          return false;

        for (unsigned t = 0; t < num_of_textures; ++t) {
          char name[256] = { 0, };
          const int offs = snprintf(&name[0], 256, "%s/%s/", output.path, &material[0]);
          if (fscanf(input.data, "%255s", &name[offs]) != 1)
            return false;
          materials[m].textures[t] = Resource::Id(TextureResource::type(), &name[0]);
        }
      }
    }

    if (!File::write(output.memory_resident_data, materials.raw(), max(mrd.num_of_materials, (size_t)1) * sizeof(MemoryResidentData::Material))) {
      output.log("Unable to write to materials!");
      return false; }

    for (uint32_t v = 0; v < mrd.num_of_vertices; ++v) {
      if (mrd.vertex_declaration & VertexDeclaration::POSITION) {
        float xyz[3];
        if (fscanf(input.data, "%f %f %f", &xyz[0], &xyz[1], &xyz[2]) != 3) {
          output.log("Malformed input: expected position!");
          return false; }
        if (!File::write(output.memory_resident_data, (const void*)&xyz[0], 12))
          goto unable_to_write_vertex;
      }

      for (uint32_t channel = 0; channel < 8; ++channel) {
        if (mrd.vertex_declaration & (VertexDeclaration::COLOR0 << channel)) {
          float rgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
          if (fscanf(input.data, "%f %f %f", &rgba[0], &rgba[1], &rgba[2]) != 3) {
            output.log("Malformed input: expected per-vertex color!");
            return false; }
          if (!File::write(output.memory_resident_data, (const void*)&rgba[0], 16))
            goto unable_to_write_vertex;
        }
      }

      for (uint32_t channel = 0; channel < 8; ++channel) {
        if (mrd.vertex_declaration & (VertexDeclaration::TEXCOORD0 << channel)) {
          float uvw[3];
          if (fscanf(input.data, "%f %f %f", &uvw[0], &uvw[1], &uvw[2]) != 3) {
            output.log("Malformed input: expected texture coords!");
            return false; }
          if (!File::write(output.memory_resident_data, (const void*)&uvw[0], 12))
            goto unable_to_write_vertex;
        }
      }

      for (uint32_t basis = 0; basis < 3; ++basis) {
        if (mrd.vertex_declaration & (VertexDeclaration::NORMAL << basis)) {
          float xyz[3];
          if (fscanf(input.data, "%f %f %f", &xyz[0], &xyz[1], &xyz[2]) != 3) {
            output.log("Malformed input: expected normal, or tangent, or binormal!");
            return false; }
          if (!File::write(output.memory_resident_data, (const void*)&xyz[0], 12))
            goto unable_to_write_vertex;
        }
      }
    }

    for (uint32_t i = 0; i < mrd.num_of_indicies; ++i) {
      uint32_t index = 0;
      if (fscanf(input.data, "%u", &index) != 1) {
        output.log("Malformed input: expected index!");
        return false; }
      if (!File::write(output.memory_resident_data, (const void*)&index, 4))
        goto unable_to_write_index;
    }

    return true;

  unable_to_write_vertex:
    output.log("Unable to write vertex!");
    return false;

  unable_to_write_index:
    output.log("Unable to write index!");
    return false;
  }
} // butane
