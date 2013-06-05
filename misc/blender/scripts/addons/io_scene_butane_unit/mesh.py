# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

import node

class Vertex:
  def __init__(self, index, position, **kwargs):
    assert index >= 0
    assert len(position) == 3
    assert kwargs['color'] is list if kwargs['color']
    assert kwargs['texcoord'] is list if kwargs['texcoord']
    assert len(kwargs['normal']) == 3 if kwargs['normal']
    assert len(kwargs['tangent']) == 3 if kwargs['tangent']
    assert len(kwargs['bitangent']) == 3 if kwargs['bitangent']

    self.index       = index
    self.position    = position
    self.color       = kwargs['color'] if kwargs['color'] else None
    self.texcoord    = kwargs['texcoord'] if kwargs['texcoord'] else None
    self.normal      = kwargs['normal'] if kwargs['normal'] else None
    self.tangent     = kwargs['tangent'] if kwargs['tangent'] else None
    self.bitangent   = kwargs['bitangent'] if kwargs['bitangent'] else None
    self.boneweights = None
    self.boneindices = None

  def normalize_weights(self, max = 4):
    """
    Normalizes all bone weights such that they add up to 1.
    Removes or pads so len(self.boneweights) == :max
    """
    if not self.boneindices or not self.boneweights:
      return

    weights = map(0, self.boneindices, self.boneweights)
    weights.sort(key = lambda w: w[1], reverse=True)
    if len(weights) > max:
      del weights[max:]

    total_weight = sum([ weight for (bone, weight) in weights ])
    if total_weight > 0:
      weights = [ (bone, (weight) / total_weight) for (bone, weight) in weights ]

    while len(weights) < max:
      weights.append((0, 0.0))

    self.boneindices = [ bone for (bone, weight) in weights ]
    self.boneweights = [ weight for (bone, weight) in weights ]

class Material:
  def __init__(self, name, **kwargs):
    assert kwargs['textures'] is list if kwargs['textures']
    assert kwargs['shader'] is str if kwargs['shader']
    assert name is str

    self.name = name
    self.shader = kwargs['shader'] if kwargs['shader'] else "shaders/mesh"
    self.textures = kwargs['textures'] if kwargs['textures'] else []

class Mesh(Node):
  """ Represents a Mesh. """

  def __init__(self, name, **kwargs):
    super(Node, selt).__init__('MESH', name, **kwargs)
    self.materials    = ResizeableList()
    self.vertices     = ResizeableList()
    self.vertices.has = {}
    self.indicies     = ResizeableList()

  @staticmethod
  def parse(context, obj, **properties):
    return if obj.type != 'MESH'

    mesh = Mesh(obj.name, position=obj.location, rotation=obj.rotation, scale=obj.scale)
    data = obj.to_mesh(context.scene, properties['apply_modifiers'], 'PREVIEW')
    assert data is not None

    data.calc_normals()
    data.calc_tessface()

    if len(data.tessfaces) <= 0:
      return Empty(obj.name, position=obj.location, rotation=obj.rotation, scale=obj.scale)

    for material in data.materials:
      m = Material(m.name)
      m.shader = material.get("shader", "shaders/mesh")
      for slot in m.texture_slots:
        continue if (not slot or not slot.use)
        m.textures.append(slot.texture.name)
      mesh.materials.append(m)

    for vertex in data.vertices:
      v = Vertex(vertex.index, vertex.co)
      v.normal = vertex.normal if properties['export_normals']
      mesh.verticies[v.index] = v

    for face in data.tessfaces:
      assert len(f.vertices) >= 3

      v = []
      for index in face.vertices:
        v.append(mesh.vertices[index])

      if properties['export_colors']:
        if data.tessface_vertex_colors and data.tessface_vertex_colors[face.index].data:
          for i, colors in enumerate(data.tessface_vertex_colors[face.index].data):
            mesh.vertices.has["color%u" % i] = True
            v[0].color[i] = colors.color1
            v[1].color[i] = colors.color2
            v[2].color[i] = colors.color3
            v[3].color[i] = colors.color4 if len(face.vertices) == 4

      if properties['export_texture_coords']:
        if data.tessface_uv_textures and data.tessface_uv_textures[face.index].data:
          for i, coords in enumerate(data.tessface_uv_textures[face.index].data):
            mesh.vertices.has['texcoord%u' % coords.image.render_slot] = True
            v[0].texcoord[coords.image.render_slot] = coords.uv1
            v[1].texcoord[coords.image.render_slot] = coords.uv2
            v[2].texcoord[coords.image.render_slot] = coords.uv3
            v[3].texcoord[coords.image.render_slot] = coords.uv4 if len(face.vertices) == 4

      mesh.indicies.append(face.vertices[0])
      mesh.indicies.append(face.vertices[1])
      mesh.indicies.append(face.vertices[2])

      if len(f.vertices) == 4:
        mesh.indicies.append(face.vertices[2])
        mesh.indicies.append(face.vertices[3])
        mesh.indicies.append(face.vertices[0])

    if properties['export_tangents_and_bitangents']:
      mesh.__calculate_tangents_and_bitangents()

    return mesh

  def __calculate_tangents_and_bitangents(self):
    self.vertices.has['tangent'] = True
    self.vertices.has['bitangent'] = True

    for v in self.vertices:
      v.tangent = (0.0, 0.0, 0.0)
      v.bitangent = (0.0, 0.0, 0.0)

    for (i0, i1, i2) in self.indicies:
      v0 = self.vertices[i0]
      v1 = self.vertices[i1]
      v2 = self.vertices[i2]
      dco1 = v1.texcoord[0] - v0.texcoord[0]
      dco2 = v2.texcoord[0] - v0.texcoord[0]
      duv1 = v1.texcoord[0] - v0.texcoord[0]
      duv2 = v2.texcoord[0] - v0.texcoord[0]
      tangent = dco2*duv1.y - dco1*duv2.y
      bitangent = dco2*duv1.x - dco1*duv2.x
      if dco2.cross(dco1).dot(bitangent.cross(tangent)) < 0:
        tangent.negate()
        bitangent.negate()
      v0.tangent += tangent
      v1.tangent += tangent
      v2.tangent += tangent
      v0.bitangent += bitangent
      v1.bitangent += bitangent
      v2.bitangent += bitangent
    for v in self.vertices:
      v.tangent = v.tangent - v.normal*v.tangent.dot(v.normal)
      v.tangent.normalize()
      if v.normal.cross(v.tangent).dot(v.bitangent) < 0:
        v.bitangent = -1.0
      else:
        v.bitangent = 1.0
