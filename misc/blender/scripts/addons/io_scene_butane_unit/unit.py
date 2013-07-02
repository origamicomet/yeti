# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

if "bpy" in locals():
  from imp import reload
  if 'scene_graph' in locals():
    reload(scene_graph)

import bpy, os
from .scene_graph import SceneGraph

class Unit:
  def __init__(self, scene_graph = SceneGraph()):
    self.scene_graph = scene_graph

  @staticmethod
  def parse(context, properties):
    return Unit(SceneGraph.parse(context, properties))

  def export(self, path):
    try:
      for f in os.listdir(os.path.splitext(path)[0]):
        if f.endswith(".mesh"):
          os.remove(f)
    except:
      pass

    out = open(path, 'w')
    out.write("nodes = [\n")
    for node in self.scene_graph.nodes:
      out.write("  {\n")
      out.write("    type = \"{}\"\n".format(node.type))
      out.write("    name = \"{}\"\n".format(node.name))
      out.write("    fully_qualified_name = \"{}\"\n".format(node.fully_qualified_name))
      out.write("    parent = {}\n".format(node.parent.id if node.parent else -1))
      out.write("    pose = {\n")
      out.write("      position = [{0[0]} {0[1]} {0[2]}]\n".format(node.position))
      out.write("      rotation = [{0[0]} {0[1]} {0[2]} {0[3]}]\n".format(node.rotation))
      out.write("      scale = [{0[0]} {0[1]} {0[2]}]\n".format(node.scale))
      out.write("    }\n")
      node.export(out, path)
      out.write("  }\n")
    out.write("]\n")
    out.close()
