# This file is part of Butane.
# See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

import bpy, math, mathutils

class Object(bpy.types.Object):
  @property
  def rotation(self):
    """ Returns this object's rotation as a quaternion irrespective of self.rotation_mode. """
    if self.rotation_mode == 'QUATERNION':
      r = self.rotation_quaternion
    elif self.rotation_mode == 'AXIS_ANGLE':
      r = mathutils.Quaternion(self.rotation_axis_angle)
    else:
      r = mathutils.Euler(self.rotation_euler, self.rotation_mode).to_quaternion()
    return (r.w, r.x, r.y, r.z)

  @property
  def link_depth(self):
    return (self.parent.link_depth + 1) if self.parent else 0

  @property
  def fully_qualified_name(self):
    return "{}.{}".format(self.parent.fully_qualified_name, self.name) if self.parent else self.name

bpy.types.Object.rotation = Object.rotation
bpy.types.Object.link_depth = Object.link_depth
bpy.types.Object.fully_qualified_name = Object.fully_qualified_name

class ResizableList(list):
  """ A "resizable" list. """

  def __init__(self, default=None):
    self._default = default

  def __setitem__(self, key, value):
    if key >= len(self):
      self += [self._default] * (key - len(self) + 1)
    super(ResizableList, self).__setitem__(key, value)
