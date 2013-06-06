# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

if "bpy" in locals():
  from imp import reload
  if 'node' in locals():
    reload(node)

import bpy
from .node import Node

class Empty(Node):
  """ Represents an empty Node. """

  def __init__(self, name, **kwargs):
    super(Empty, self).__init__('EMPTY', name, **kwargs)

  @staticmethod
  def parse(obj, context, properties):
    assert obj.type == 'EMPTY'
    return Empty(obj.name, position=obj.location, rotation=obj.rotation, scale=obj.scale)
