# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

bl_info = {
  "name": "Butane Engine Unit (.unit)",
  "author": "Michael 'devbug' Williams",
  "version": (2013, 6, 2),
  "blender": (2, 6, 3),
  "location": "File > Import-Export",
  "description": "Import and export units from and to the butane engine unit format",
  "warning": "",
  "wiki_url": "",
  "tracker_url": "",
  "category": "Import-Export" }

if "bpy" in locals():
  from imp import reload
  if 'misc' in locals():
    reload(misc)
  if 'unit' in locals():
    reload(unit)

import bpy, os
from bpy.props import (BoolProperty, FloatProperty, StringProperty, EnumProperty)
from bpy_extras.io_utils import (ImportHelper, ExportHelper)

from .misc import (Object, ResizableList)
from .unit import Unit

class Properties(dict):
  def __init__(self, values = {}):
    for k, v in values.items():
      dict.__setitem__(self, k, v)

  def __getitem__(self, key):
    try:
      return super(Properties, self).__getitem__(key)
    except:
      return None

  def __getattr__(self, name):
    return self.__getitem__(name)

class ImportUnit(bpy.types.Operator, ImportHelper):
  """Load a Butane engine unit"""
  bl_idname = "import_scene.unit"
  bl_label = "Import a Unit"
  filename_ext = ".unit"

class ExportUnit(bpy.types.Operator, ExportHelper):
  """Save a Butane engine unit"""
  bl_idname = "export_scene.unit"
  bl_label = "Export a Unit"
  filename_ext = ".unit"

  apply_modifiers = BoolProperty(
    name="Apply Modifiers",
    description="Applies modifiers prior to export",
    default=True)

  only_export_selected = BoolProperty(
    name="Export Selected",
    description="Exports only selected objects",
    default=False)

  export_empties = BoolProperty(
    name="Export Empties",
    description="Export empties",
    default=True)

  export_meshes = BoolProperty(
    name="Export Meshes",
    description="Export meshes",
    default=True)

  export_normals = BoolProperty(
    name="Normals",
    description="Export per-vertex normals",
    default=True)

  export_colors = BoolProperty(
    name="Colors",
    description="Export per-vertex colors",
    default=True)

  export_texture_coords = BoolProperty(
    name="Texture Coordinates",
    description="Export per-vertex texture coordinates",
    default=True)

  export_tangents_and_bitangents = BoolProperty(
    name="Tangents & Bitangents",
    description="Generate and export per-vertex tangent and bitanges",
    default=True)

  def execute(self, context):
    properties = Properties({
      'apply_modifiers' : self.properties.apply_modifiers,
      'only_export_selected' : self.properties.only_export_selected,
      'export_empties' : self.properties.export_empties,
      'export_meshes' : self.properties.export_meshes,
      'export_normals' : self.properties.export_normals,
      'export_colors' : self.properties.export_colors,
      'export_texture_coords' : self.properties.export_texture_coords,
      'export_tangents_and_bitangents' : self.properties.export_tangents_and_bitangents })

    unit = Unit.parse(context, properties)
    unit.export(self.filepath)

    return {'FINISHED'}

  def draw(self, context):
    layout = self.layout

    options = layout.box()
    options.label(text="Export Options", icon='OUTLINER_DATA_MESH')

    export_options = options.column()
    export_options.prop(self, "apply_modifiers")
    export_options.prop(self, "only_export_selected")

    options.prop(self, "export_empties", icon='OUTLINER_OB_EMPTY')

    options.prop(self, "export_meshes", icon='OUTLINER_OB_MESH')
    mesh_options_include = options.box().column()
    mesh_options_include.prop(self, "export_normals")
    mesh_options_include.prop(self, "export_colors")
    mesh_options_include.prop(self, "export_texture_coords")
    mesh_options_include.prop(self, "export_tangents_and_bitangents")

def menu_func_import(self, context):
  self.layout.operator(ImportUnit.bl_idname, text="Butane Unit (.unit)")

def menu_func_export(self, context):
  self.layout.operator(ExportUnit.bl_idname, text="Butane Unit (.unit)")

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
