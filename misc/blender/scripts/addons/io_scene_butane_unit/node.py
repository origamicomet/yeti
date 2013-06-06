# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

import bpy

class Node:
  """ An object in a SceneGraph, like a Camera or Mesh. """

  def __init__(self, type, name, **kwargs):
    assert Node.__is_acceptable_type(type)
    assert len(kwargs['position']) == 3 if kwargs['position'] else True
    assert len(kwargs['rotation']) == 4 if kwargs['rotation'] else True
    assert len(kwargs['scale']) == 3 if kwargs['scale'] else True

    self.type     = type
    self.name     = name
    self.parent   = (kwargs['parent'] if 'parent' in kwargs else None)
    self.position = (kwargs['position'] if 'position' in kwargs else (0, 0, 0))
    self.rotation = (kwargs['rotation'] if 'rotation' in kwargs else (1, 0, 0, 0))
    self.scale    = (kwargs['scale'] if 'scale' in kwargs else (1, 1, 1))

  @staticmethod
  def __is_acceptable_type(type):
    return True if type in ['EMPTY', 'MESH'] else False

  @property
  def link_depth(self):
    return (self.parent.link_depth + 1) if self.parent else 0

  @property
  def fully_qualified_name(self):
    return "{}.{}".format(self.parent.fully_qualified_name, self.name) if self.parent else self.name

  def export(self, out, path):
    pass
