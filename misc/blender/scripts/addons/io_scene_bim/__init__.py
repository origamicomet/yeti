# The author(s) disclaim copyright to this source-code.
# Can be licensed under MIT (in case the public domain is not recognized).
# More information can be found in the LICENSE file.

bl_info = {
	"name": "Butane Intermediate Model (.bim)",
	"author": "Michael 'devbug' Williams",
	"version": (2013, 6, 1),
	"blender": (2, 6, 3),
	"location": "File > Export > Butane Intermediate Model",
	"description": "Export to the Butane Intermediate Model format (.bim)",
	"warning": "",
	"wiki_url": "",
	"tracker_url": "",
	"category": "Import-Export" }

import os, math
import mathutils

import bpy
from bpy.props import *
from bpy_extras.io_utils import ImportHelper, ExportHelper

# See http://stackoverflow.com/a/8849909/1982393
class ResizableList(list):
  def __init__(self, default=None):
    self._default = default
  def __setitem__(self, key, value):
    if key >= len(self):
      self += [self._default] * (key - len(self) + 1)
    super(ResizableList, self).__setitem__(key, value)

class Vertex:
	def __init__(self, index, position, color = ResizableList(), texcoord = ResizableList(), normal = None, tangent = None, bitangent = None, boneindices = None, boneweights = None):
		self.index = index
		self.position = position
		self.color = color
		self.texcoord = texcoord
		self.normal = normal
		self.tangent = tangent
		self.bitangent = bitangent
		self.boneindices = boneindices
		self.boneweights = boneweights

	# Normalizes all bone weights such that they add up to 1.0.
	# Chops or pads so len(boneweights) == max.
	def normalize_weights(self, max = 4):
		if not self.boneindices or not self.boneweights:
			return

		# Drop the least important bones.
		weights = map(0, self.boneindices, self.boneweights)
		weights.sort(key = lambda w: w[1], reverse=True)
		if len(weights) > max:
			del weights[max:]

		# Reweight the bones.
		total_weight = sum([ weight for (bone, weight) in weights ])
		if total_weight > 0:
			weights = [ (bone, (weight) / total_weight) for (bone, weight) in weights ]

		while len(weights) < max:
			weights.append((0, 0.0))

		self.boneindices = [ bone for (bone, weight) in weights ]
		self.boneweights = [ weight for (bone, weight) in weights ]

class Material:
	def __init__(self, name, textures):
		self.name = name
		self.textures = textures

class VertexDeclaration:
	def __init__(self):
		self.position = True
		self.color = [False, False, False, False, False, False, False, False]
		self.texcoord = [False, False, False, False, False, False, False, False]
		self.normal = False
		self.tangent = False
		self.bitangent = False
		self.boneindices = False
		self.boneweights = False

class Mesh:
	def __init__(self, name, vertex_declaration, materials, vertices, indicies):
		self.name			          = name
		self.vertex_declaration = vertex_declaration
		self.materials          = materials
		self.vertices	          = vertices
		self.indicies	          = indicies

	# Calculates tangent and bitangent based on surface normal and texture coordinates.
	def calculate_tangents(self):
		self.vertex_declaration.tangent = True
		self.vertex_declaration.bitangent = True

		# See "Tangent Space Calculation" at http://www.terathon.com/code/tangent.html
		for v in self.vertices:
			v.tangent = mathutils.Vector((0.0, 0.0, 0.0))
			v.bitangent = mathutils.Vector((0.0, 0.0, 0.0))
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

def export_mesh(bim, mesh):
	bim.write('  {\n')

	bim.write('    name = "%s"\n' % mesh.name)

	bim.write('    materials = [\n')
	for material in mesh.materials:
		bim.write('      {\n')
		bim.write('        name="%s"\n' % material.name)
		bim.write('        textures=[\n')
		for texture in material.textures:
			bim.write('          "%s"\n' % texture)
		bim.write('        ]\n')
		bim.write('      }\n')
	bim.write('    ]\n')

	bim.write('    vertices = {\n')
	bim.write('      declaration = [ ')
	bim.write('position ');
	for i in range(0, len(mesh.vertex_declaration.color) - 1):
		if mesh.vertex_declaration.color[i]:
			bim.write('color%d ' % i)
	for i in range(0, len(mesh.vertex_declaration.texcoord) - 1):
		if mesh.vertex_declaration.texcoord[i]:
			bim.write('texcoord%d ' % i)
	if mesh.vertex_declaration.normal:
		bim.write('normal ')
	if mesh.vertex_declaration.tangent:
		bim.write('tangent ')
	if mesh.vertex_declaration.bitangent:
		bim.write('bitangent ')
	bim.write(']\n')

	bim.write('      position = [ ')
	for v in mesh.vertices:
		bim.write("{0.x} {0.y} {0.z} ".format(v.position))
	bim.write(']\n')

	for i in range(0, len(mesh.vertex_declaration.color) - 1):
		if not mesh.vertex_declaration.color[i]:
			continue
		bim.write('      color%d = [ ' % i)
		for v in mesh.vertices:
			bim.write("{0.x} {0.y} {0.z} ".format(v.color[i]))
		bim.write(']\n')

	for i in range(0, len(mesh.vertex_declaration.texcoord) - 1):
		if not mesh.vertex_declaration.texcoord[i]:
			continue
		bim.write('      texcoord%d = [ ' % i)
		for v in mesh.vertices:
			bim.write("{0.x} {0.y} ".format(v.texcoord[i]))
		bim.write(']\n')

	if mesh.vertex_declaration.normal:
		bim.write('      normal = [ ')
		for v in mesh.vertices:
			bim.write("{0.x} {0.y} {0.z} ".format(v.normal))
		bim.write(']\n')

	if mesh.vertex_declaration.tangent:
		bim.write('      tangent = [ ')
		for v in mesh.vertices:
			bim.write("{0.x} {0.y} {0.z} ".format(v.tangent))
		bim.write(']\n')

	if mesh.vertex_declaration.bitangent:
		bim.write('      bitangent = [ ')
		for v in mesh.vertices:
			bim.write("{0.x} {0.y} {0.z} ".format(v.bitangent))
		bim.write(']\n')
	bim.write('    }\n')

	bim.write('    indicies = [ ')
	for index in mesh.indicies:
		bim.write("{0} ".format(index))
	bim.write(']\n')

	bim.write('  }\n')

class ExportBIM(bpy.types.Operator, ExportHelper):
	'''Export a Butane Intermediate Model BIM'''
	bl_idname = "export.bim"
	bl_label = 'Export BIM'
	filepath = ""
	filename_ext = ".bim"

	export_only_selected = BoolProperty(name="Only Export Selected", description="Exports only selected objects", default=False)
	export_meshes = BoolProperty(name="Export Meshes", description="Exports meshes", default=True)
	apply_modifiers = BoolProperty(name="Apply Modifiers", description="Apply modifiers prior to exporting", default=True)
	calculate_tangents = BoolProperty(name="Calculate Tangents", description="Calculates tangent and bitangents from normals and texture coordinates", default=False)

	def mesh(self, context, obj):
		data = obj.to_mesh(context.scene, self.properties.apply_modifiers, 'PREVIEW')
		if not data:
			return

		vertex_declaration = VertexDeclaration()
		vertex_declaration.normal = True

		materials = ResizableList()
		vertices = ResizableList()
		indicies = ResizableList()

		data.calc_normals()
		data.calc_tessface()
		if len(data.tessfaces) <= 0:
			return

		for m in data.materials:
			textures = []
			for slot in m.texture_slots:
				if not slot or not slot.use:
					continue
				textures.append(slot.texture.name)

			materials.append(Material(m.name, textures))

		for v in data.vertices:
			vertex = Vertex(v.index, mathutils.Vector(v.co))
			# vertex.normal = mathutils.Vector(v.normal)
			vertices[v.index] = vertex

		for f in data.tessfaces:
			if len(f.vertices) < 3:
				continue

			v = []
			for index in f.vertices:
				v.append(vertices[index])
				vertices[index].normal = mathutils.Vector(f.normal)

			if data.tessface_vertex_colors and data.tessface_vertex_colors[f.index].data:
				for i, colors in enumerate(data.tessface_vertex_colors[f.index].data):
					vertex_declaration.color[i - 1] = True

					v[0].color[i - 1] = colors.color1
					v[1].color[i - 1] = colors.color2
					v[2].color[i - 1] = colors.color3

					if len(f.vertices) == 4:
						v[3].color[i - 1] = colors.color4

			if data.tessface_uv_textures and data.tessface_uv_textures[f.index].data:
				for i, coords in enumerate(data.tessface_uv_textures[f.index].data):
					vertex_declaration.texcoord[coords.image.render_slot] = True

					v[0].texcoord[coords.image.render_slot] = coords.uv1
					v[1].texcoord[coords.image.render_slot] = coords.uv2
					v[2].texcoord[coords.image.render_slot] = coords.uv3

					if len(f.vertices) == 4:
						v[3].texcoord[coords.image.render_slot] = coords.uv4

			if len(f.vertices) == 4:
				indicies.append(v[0].index)
				indicies.append(v[1].index)
				indicies.append(v[2].index)
				indicies.append(v[2].index)
				indicies.append(v[3].index)
				indicies.append(v[0].index)
			else:
				indicies.append(v[0].index)
				indicies.append(v[1].index)
				indicies.append(v[2].index)

		mesh = Mesh(obj.name, vertex_declaration, materials, vertices, indicies)
		if self.properties.calculate_tangents:
			mesh.calculate_tangents()
		return mesh

	def execute(self, context):
		objs = context.selected_objects if self.properties.export_only_selected else context.scene.objects

		meshes = []
		for obj in objs:
			if obj.type == 'MESH' and self.properties.export_meshes:
				mesh = self.mesh(context, obj)
				if mesh:
					meshes.append(mesh)

		try:
			bim = open(self.filepath, 'w')
		except:
			print ('Failed to open \'%s\'' % (filename))
			return

		bim.write('meshes = [\n')
		for mesh in meshes:
			export_mesh(bim, mesh)
		bim.write(']\n')

		bim.close()
		return {'FINISHED'}

	def check(self, context):
		filepath = bpy.path.ensure_ext(self.filepath, '.bim')
		if filepath != self.filepath:
			self.filepath = filepath
			return True
		return False

# def menu_func_import(self, context):
#	 self.layout.operator(ImportBIM.bl_idname, text="Butane Intermediate Model (.bim)")

def menu_func_export(self, context):
	# default_path = os.path.splitext(bpy.data.filepath)[0] + ".bim"
	self.layout.operator(ExportBIM.bl_idname, text="Butane Intermediate Model (.bim)") #.filepath = default_path

def register():
	bpy.utils.register_module(__name__)
	# bpy.types.INFO_MT_file_import.append(menu_func_import)
	bpy.types.INFO_MT_file_export.append(menu_func_export)

def unregister():
	bpy.utils.unregister_module(__name__)
	# bpy.types.INFO_MT_file_import.remove(menu_func_import)
	bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
	register()
