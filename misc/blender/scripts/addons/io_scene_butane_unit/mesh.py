# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

if "bpy" in locals():
  from imp import reload
  if 'misc' in locals():
    reload(misc)
  if 'node' in locals():
    reload(node)

import bpy, os, math, mathutils
from .misc import (Object, ResizableList)
from .node import Node

class Vertex:
  def __init__(self, index, position, **kwargs):
    assert index >= 0
    assert len(position) == 3
    assert kwargs['color'] is list if 'color' in kwargs else True
    assert kwargs['texcoord'] is list if 'texcoord' in kwargs else True
    assert len(kwargs['normal']) == 3 if 'normal' in kwargs else True
    assert len(kwargs['tangent']) == 3 if 'tangent' in kwargs else True
    assert len(kwargs['bitangent']) == 3 if 'bitangent' in kwargs else True

    self.index       = index
    self.position    = position
    self.color       = kwargs['color'] if 'color' in kwargs else ResizableList()
    self.texcoord    = kwargs['texcoord'] if 'texcoord' in kwargs else ResizableList()
    self.normal      = kwargs['normal'] if 'normal' in kwargs else None
    self.tangent     = kwargs['tangent'] if 'tangent' in kwargs else None
    self.bitangent   = kwargs['bitangent'] if 'bitangent' in kwargs else None
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
    assert kwargs['textures'] is list if 'textures' in kwargs else True
    assert kwargs['shader'] is str if 'shader' in kwargs else True

    self.name = name
    self.shader = kwargs['shader'] if 'shader' in kwargs else "shaders/mesh"
    self.textures = kwargs['textures'] if 'textures' in kwargs else []

class Mesh(Node):
  """ Represents a Mesh. """

  def __init__(self, name, **kwargs):
    super(Mesh, self).__init__('MESH', name, **kwargs)
    self.materials    = ResizableList()
    self.vertices     = ResizableList()
    self.vertices.has = {'position':True,'color':ResizableList(),'texcoord':ResizableList()}
    self.indicies     = []

  @staticmethod
  def parse(obj, context, properties):
    assert obj.type == 'MESH'

    mesh = Mesh(obj.name, position=obj.location, rotation=obj.rotation, scale=obj.scale)
    data = obj.to_mesh(context.scene, properties.apply_modifiers, 'PREVIEW')
    assert data is not None

    data.calc_normals()
    data.calc_tessface()

    if len(data.tessfaces) <= 0:
      return Empty(obj.name, position=obj.location, rotation=obj.rotation, scale=obj.scale)

    for material in data.materials:
      m = Material(material.name)
      m.shader = material.get("shader", "shaders/mesh")
      for slot in material.texture_slots:
        if slot and slot.use:
          m.textures.append(slot.texture.name)
      mesh.materials.append(m)

    if properties.export_normals:
      mesh.vertices.has["normal"] = True

    for vertex in data.vertices:
      v = Vertex(vertex.index, vertex.co)
      v.normal = vertex.normal if properties.export_normals else None
      mesh.vertices[v.index] = v

    for face in data.tessfaces:
      assert len(face.vertices) >= 3

      v = []
      for index in face.vertices:
        v.append(mesh.vertices[index])

      if properties.export_colors:
        if data.tessface_vertex_colors and data.tessface_vertex_colors[face.index].data:
          for i, colors in enumerate(data.tessface_vertex_colors[face.index].data):
            mesh.vertices.has["color"][i] = True
            v[0].color[i] = colors.color1
            v[1].color[i] = colors.color2
            v[2].color[i] = colors.color3
            if len(face.vertices) == 4:
              v[3].color[i] = colors.color4

      if properties.export_texture_coords:
        if data.tessface_uv_textures and data.tessface_uv_textures[face.index].data:
          for i, coords in enumerate(data.tessface_uv_textures[face.index].data):
            mesh.vertices.has['texcoord'][coords.image.render_slot] = True
            v[0].texcoord[coords.image.render_slot] = (coords.uv1[0], coords.uv1[1], 0.0)
            v[1].texcoord[coords.image.render_slot] = (coords.uv2[0], coords.uv2[1], 0.0)
            v[2].texcoord[coords.image.render_slot] = (coords.uv3[0], coords.uv3[1], 0.0)
            if len(face.vertices) == 4:
              v[3].texcoord[coords.image.render_slot] = (coords.uv4[0], coords.uv4[1], 0.0)

      mesh.indicies.append(face.vertices[2])
      mesh.indicies.append(face.vertices[1])
      mesh.indicies.append(face.vertices[0])

      if len(face.vertices) == 4:
        mesh.indicies.append(face.vertices[0])
        mesh.indicies.append(face.vertices[3])
        mesh.indicies.append(face.vertices[2])

    if properties.export_tangents_and_bitangents:
      mesh.__calculate_tangents_and_bitangents()

    return mesh

  def __calculate_tangents_and_bitangents(self):
    if 'texcoord' not in self.vertices.has:
      return
    if len(self.vertices.has['texcoord']) == 0:
      return

    self.vertices.has['tangent'] = True
    self.vertices.has['bitangent'] = True

    for v in self.vertices:
      v.tangent = mathutils.Vector((0.0, 0.0, 0.0))
      v.bitangent = mathutils.Vector((0.0, 0.0, 0.0))

    for i0, i1, i2 in zip(self.indicies[0::3], self.indicies[1::3], self.indicies[2::3]):
      v0 = self.vertices[i0]
      v1 = self.vertices[i1]
      v2 = self.vertices[i2]
      dco1 = mathutils.Vector(v1.texcoord[0]) - mathutils.Vector(v0.texcoord[0])
      dco2 = mathutils.Vector(v2.texcoord[0]) - mathutils.Vector(v0.texcoord[0])
      duv1 = mathutils.Vector(v1.texcoord[0]) - mathutils.Vector(v0.texcoord[0])
      duv2 = mathutils.Vector(v2.texcoord[0]) - mathutils.Vector(v0.texcoord[0])
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

  def export(self, out, path):
    mesh_path = "{}/{}.mesh".format(os.path.splitext(path)[0], "/".join(self.fully_qualified_name.split('.')))

    try:
      os.makedirs(os.path.dirname(mesh_path))
    except FileExistsError:
      pass
    except Exception:
      raise

    mesh = open(mesh_path, 'w')
    mesh.write("{}\n".format(len(self.materials)))

    vertex_declaration = []
    if 'position' in self.vertices.has:
      vertex_declaration.append('POSITION')
    if 'color' in self.vertices.has:
      for (channel, has) in enumerate(self.vertices.has['color']):
        if has:
          vertex_declaration.append("COLOR{}".format(channel))
    if 'texcoord' in self.vertices.has:
      for (channel, has) in enumerate(self.vertices.has['texcoord']):
        if has:
          vertex_declaration.append("TEXCOORD{}".format(channel))
    if 'normal' in self.vertices.has:
      vertex_declaration.append('NORMAL')
    if 'tangent' in self.vertices.has:
      vertex_declaration.append('TANGENT')
    if 'bitangent' in self.vertices.has:
      vertex_declaration.append('BITANGENT')

    mesh.write("{}\n".format(len(vertex_declaration)))
    for component in vertex_declaration:
      mesh.write("{}\n".format(component))

    mesh.write("{}\n".format(len(self.vertices)))
    mesh.write("{}\n".format(len(self.indicies)))

    for material in self.materials:
      mesh.write("{}\n".format(material.name))
      mesh.write("{}\n".format(material.shader))
      mesh.write("{}\n".format(len(material.textures)))
      for texture in material.textures:
        mesh.write("{}\n".format(texture))

    for v in self.vertices:
      if 'position' in self.vertices.has:
        mesh.write("{0[0]} {0[1]} {0[2]}\n".format(v.position))
      if 'color' in self.vertices.has:
        for (channel, has) in enumerate(self.vertices.has['color']):
          if has:
            mesh.write("{0[0]} {0[1]} {0[2]}\n".format(v.color[channel]))
      if 'texcoord' in self.vertices.has:
        for (channel, has) in enumerate(self.vertices.has['texcoord']):
          if has:
            mesh.write("{0[0]} {0[1]} {0[2]}\n".format(v.texcoord[channel]))
      if 'normal' in self.vertices.has:
        mesh.write("{0[0]} {0[1]} {0[2]}\n".format(v.normal))
      if 'tangent' in self.vertices.has:
        mesh.write("{0[0]} {0[1]} {0[2]}\n".format(v.tangent))
      if 'bitangent' in self.vertices.has:
        mesh.write("{0[0]} {0[1]} {0[2]}\n".format(v.bitangent))

    for i0, i1, i2 in zip(self.indicies[0::3], self.indicies[1::3], self.indicies[2::3]):
      mesh.write("%u %u %u\n" % (i0, i1, i2))

    mesh.close()
