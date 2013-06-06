# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

if "bpy" in locals():
  from imp import reload
  if 'node' in locals():
    reload(node)
  if 'empty' in locals():
    reload(empty)
  if 'mesh' in locals():
    reload(mesh)

import bpy
from .node import Node
from .empty import Empty
from .mesh import Mesh

class SceneGraph:
  def __init__(self):
    self._nodes = {}

  def add(self, node):
    assert node.fully_qualified_name not in self._nodes
    self._nodes[node.fully_qualified_name] = node

  def remove(self, node):
    if node.fully_qualified_name in self._nodes:
      assert self._nodes[node.fully_qualified_name] == node
    self._nodes[node.fully_qualified_name] = None

  @property
  def nodes(self):
    nodes = sorted([node for fqn, node in self._nodes.items()], key=lambda node: node.link_depth)
    for (id, node) in enumerate(nodes):
      node.id = id
    return nodes

  @staticmethod
  def parse(context, properties):
    scene_graph = SceneGraph()

    objs = [obj for obj in (context.selected_objects if properties.only_export_selected else context.scene.objects)]
    objs.sort(key=lambda obj: obj.link_depth)

    for obj in objs:
      if obj.type == 'EMPTY':
        node = Empty.parse(obj, context, properties)
      elif obj.type == 'MESH':
        node = Mesh.parse(obj, context, properties)
      else:
        continue

      if obj.parent:
        node.parent = scene_graph._nodes[obj.parent.fully_qualified_name]
      scene_graph.add(node)

    return scene_graph
