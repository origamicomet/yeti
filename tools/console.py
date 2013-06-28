# This file is part of Butane. See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

# def usage():
#   return (
#     "usage: console [options]\n"
#     "  connects to an engine instance and displays a Lua REPL\n"
#     "\n"
#     "options:\n"
#     "    -host -h [address or hostname] overrides the default hostname and port\n"
#     "      -h logs.voxkings.com:5000 # listens at logs.voxkings.com on port 5000 for engines logging to network\n")

# if __name__ != "__main__":
#   raise usage()

# import sys, os, threading
# sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'deps', 'foundation', 'py'))
# from foundation import network
# import wx

# class ConsoleFrame(wx.Frame):
#   def __init__(self, parent=None, id=-1):
#     wx.Frame.__init__(self, parent, id)
#     self.__ui__()

#   def __ui__(self):
#     self.SetTitle('Console')
#     self.SetSize((800, 450))

#     # Menu:
#     menu = wx.MenuBar()
#     file_menu = wx.Menu()
#     self.Bind(wx.EVT_MENU, self.on_exit, file_menu.Append(wx.ID_EXIT, '&Exit'))
#     menu.Append(file_menu, '&File')
#     help_menu = wx.Menu()
#     self.Bind(wx.EVT_MENU, self.on_documentation, help_menu.Append(wx.ID_HELP, '&Documentation'))
#     self.Bind(wx.EVT_MENU, self.on_about, help_menu.Append(wx.ID_ANY, '&About'))
#     menu.Append(help_menu, '&Help')
#     self.SetMenuBar(menu)

#     # Tabs:
#     self.tabs = wx.Notebook(self)
#     self.sizer = wx.BoxSizer(wx.VERTICAL)
#     self.sizer.Add(self.tabs, 1, wx.EXPAND)
#     self.SetSizer(self.sizer)

#     tab = wx.Panel(self.tabs)
#     tab.sizer = wx.BoxSizer(wx.VERTICAL)

#     log = wx.TextCtrl(tab, style = wx.TE_MULTILINE | wx.TE_READONLY | wx.TE_DONTWRAP | wx.TE_PROCESS_ENTER)
#     log.AppendText("Connected to (127.0.0.1, 5000)\n")
#     tab.sizer.Add(log, 1, wx.EXPAND)

#     row = wx.Panel(tab)
#     row.sizer = wx.BoxSizer(wx.HORIZONTAL)
#     execute = wx.Button(row, label='Execute', style=wx.BU_EXACTFIT)
#     execute.Bind(wx.EVT_BUTTON, self.on_execute)
#     row.sizer.Add(execute, 0, wx.EXPAND | wx.RIGHT, 5)
#     command = wx.TextCtrl(row, value='cmd')
#     row.sizer.Add(command, 1, wx.EXPAND)
#     row.SetSizer(row.sizer)

#     tab.sizer.Add(row, 0, wx.EXPAND | wx.TOP, 3)
#     tab.SetSizer(tab.sizer)

#     self.tabs.AddPage(tab, "butane (windows, x86, debug) - 7062")

#   def on_exit(self, e):
#     self.Close()

#   def on_documentation(self, e):
#     pass

#   def on_about(self, e):
#     pass

#   def on_execute(self, e):
#     pass

# class App(wx.App):
#   def OnInit(self):
#     self.frame = ConsoleFrame()
#     self.frame.Show(True)
#     self.SetTopWindow(self.frame)
#     return True

# app = App()
# app.MainLoop()

#####

import sys, os, threading
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'deps', 'foundation', 'py'))
from foundation import network

def on_msg(_, p):
  print("[{}] {}".format(p.read_str("context"), p.read_str("msg")))

conn = (network.Protocol("console", 1)
  .remote_to_local("msg", on_msg)
  .connect('127.0.0.1', 5000))

while True:
  conn.update()

# def connected(conn):
#   print("{} connected".format(conn.address))
#   return None

# def disconnected(conn, engine):
#   print("{} disconnected".format(conn.address))

# def unhandled(engine, type):
#   print("Unhandled packet, type={}".format(type))

# def execute(engine, p, command):
#   p.write_str("cmd", command)

# def on_msg(engine, p):
#   print("[{}] {}".format(p.read_str("context"), p.read_str("msg")))

# (network.Protocol("console", 1)
#   .connected(connected)
#   .disconnected(disconnected)
#   .unhandled(unhandled)
#   .local_to_remote("cmd", execute)
#   .remote_to_local("msg", on_msg)
#   .host('127.0.0.1', 5000))
