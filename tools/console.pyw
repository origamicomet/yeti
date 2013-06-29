# This file is part of Butane. See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

import sys, os, threading, socket, unicodedata
from threading import Thread
from Queue import Queue
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'deps', 'foundation', 'py'))
from foundation import network
import wx
from wx.lib.newevent import NewEvent

OnConnectedEvent, EVT_ON_CONNECTED = NewEvent()
OnConnectionFailedEvent, EVT_ON_CONNECTION_FAILED = NewEvent()
OnDisconnectedEvent, EVT_ON_DISCONNCTED = NewEvent()
OnMessageEvent, EVT_ON_MESSAGE = NewEvent()

class NetworkingThread(Thread):
  def __init__(self, listener):
    Thread.__init__(self)
    self._protocol = (
      network.Protocol("console", 1)
        .remote_to_local("msg", self._on_message))
    self._listener = listener
    self._stop = threading.Event()
    self._connections = []
    self._connection_requests = Queue()
    self._disconnection_requests = Queue()
    self.start()

  def run(self):
    while not self._stop.is_set():
      while not self._connection_requests.empty():
        remote, port = self._connection_requests.get()
        try:
          conn = self._protocol.connect(remote, port)
        except:
          wx.PostEvent(self._listener, OnConnectionFailedEvent(remote=remote, port=port))
          continue
        self._connections.append(conn)
        wx.PostEvent(self._listener, OnConnectedEvent(conn=conn))

      while not self._disconnection_requests.empty():
        conn = self._disconnection_requests.get()
        wx.PostEvent(self._listener, OnDisconnectedEvent(conn=conn))
        if conn not in self._connections:
          continue
        try:
          conn.disconnect()
        except:
          raise
          pass
        self._connections.remove(conn)

      for conn in self._connections:
        if not conn.update(conn):
          wx.PostEvent(self._listener, OnDisconnectedEvent(conn=conn))
          try:
            conn.disconnect()
          except:
            pass
          self._connections.remove(conn)

  def connect_to(self, remote, port):
    self._connection_requests.put((remote, port))

  def disconnect_from(self, conn):
    self._disconnection_requests.put(conn)

  def stop(self):
    self._stop.set()

  def _on_message(self, conn, p):
    wx.PostEvent(self._listener, OnMessageEvent(conn=conn, context=p.read_str("context"), msg=p.read_str("msg")))

class ConnectToDialog(wx.Dialog):
  def __init__(self, parent=None, id=wx.ID_ANY):
    wx.Dialog.__init__(self, parent, id)
    self.__ui__()

  def __ui__(self):
    self.SetTitle('Connect to...')
    self.sizer = wx.BoxSizer(wx.VERTICAL)
    self.SetSizer(self.sizer)

    container = wx.Panel(self)
    container.sizer = wx.BoxSizer(wx.VERTICAL)
    container.SetSizer(container.sizer)
    self.sizer.Add(container, 1, wx.EXPAND | wx.ALL, 15)

    self.remote = wx.TextCtrl(container, value="127.0.0.1:5000")
    container.sizer.Add(self.remote, 0, wx.EXPAND)

    row = wx.Panel(container)
    row.sizer = wx.BoxSizer(wx.HORIZONTAL)
    row.SetSizer(row.sizer)

    connect = wx.Button(row, label='Connect', id=wx.ID_OK)
    connect.Bind(wx.EVT_BUTTON, self.on_close)
    row.sizer.Add(connect, 0, wx.EXPAND | wx.RIGHT, 5)
    cancel = wx.Button(row, label='Cancel', id=wx.ID_CANCEL)
    cancel.Bind(wx.EVT_BUTTON, self.on_close)
    row.sizer.Add(cancel, 0, wx.EXPAND)

    container.sizer.Add(row, 0, wx.EXPAND | wx.TOP, 10)

    self.sizer.Fit(self)

  def on_close(self, e):
    self.SetReturnCode(e.GetId())
    self.Destroy()

class ConsoleTab(wx.Panel):
  def __init__(self, idx, conn, parent=None, id=wx.ID_ANY):
    wx.Panel.__init__(self, parent, id)
    self._idx = idx
    self._conn = conn
    self.__ui__()

  def __ui__(self):
    self.sizer = wx.BoxSizer(wx.VERTICAL)
    self.SetSizer(self.sizer)

    self.log = wx.TextCtrl(self, style=wx.TE_MULTILINE | wx.TE_READONLY | wx.TE_DONTWRAP | wx.TE_PROCESS_ENTER)
    self.sizer.Add(self.log, 1, wx.EXPAND)

    row = wx.Panel(self)
    row.sizer = wx.BoxSizer(wx.HORIZONTAL)
    row.SetSizer(row.sizer)

    self.command = wx.TextCtrl(row)
    self.command.Bind(wx.EVT_TEXT_ENTER, self.on_execute)
    row.sizer.Add(self.command, 1, wx.EXPAND)

    self.sizer.Add(row, 0, wx.EXPAND | wx.TOP, 3)

  def on_execute(self, e):
    pass

class ConsoleFrame(wx.Frame):
  def __init__(self, parent=None, id=wx.ID_ANY):
    wx.Frame.__init__(self, parent, id)
    self._networking_thread = NetworkingThread(self)
    self._connections = {}
    self.Bind(wx.EVT_CLOSE, self.on_close)
    self.Bind(EVT_ON_CONNECTED, self.on_connected)
    self.Bind(EVT_ON_CONNECTION_FAILED, self.on_connection_failed)
    self.Bind(EVT_ON_DISCONNCTED, self.on_disconnected)
    self.Bind(EVT_ON_MESSAGE, self.on_message)
    self.__ui__()

  def __ui__(self):
    self.SetTitle('Console')
    self.SetSize((800, 450))
    self.sizer = wx.BoxSizer(wx.VERTICAL)
    self.SetSizer(self.sizer)

    # Menu:
    menu = wx.MenuBar()
    file_menu = wx.Menu()
    self.Bind(wx.EVT_MENU, self.on_connect, file_menu.Append(wx.ID_ANY, '&Connect'))
    self.Bind(wx.EVT_MENU, self.on_disconnect, file_menu.Append(wx.ID_ANY, '&Disconnect'))
    self.Bind(wx.EVT_MENU, self.on_exit, file_menu.Append(wx.ID_EXIT, '&Exit'))
    menu.Append(file_menu, '&File')
    help_menu = wx.Menu()
    self.Bind(wx.EVT_MENU, self.on_documentation, help_menu.Append(wx.ID_HELP, '&Documentation'))
    self.Bind(wx.EVT_MENU, self.on_about, help_menu.Append(wx.ID_ANY, '&About'))
    menu.Append(help_menu, '&Help')
    self.SetMenuBar(menu)

    # Tabs:
    self.tabs = wx.Notebook(self)
    self.sizer.Add(self.tabs, 1, wx.EXPAND)

  def on_close(self, e):
    self._networking_thread.stop()
    e.Skip()

  def on_connected(self, e):
    if e.conn.address in self._connections:
      tab = self._connections[e.conn.address]
      tab.connected = True
      tab.log.AppendText("Connected to {0[0]}:{0[1]}\n".format(e.conn.address))
      self._connections[e.conn.address] = tab
    else:
      tab = ConsoleTab(self.tabs.GetPageCount(), e.conn, self.tabs)
      tab.connected = True
      self.tabs.AddPage(tab, "{0[0]}:{0[1]}".format(e.conn.address), True)
      tab.log.AppendText("Connected to {0[0]}:{0[1]}\n".format(e.conn.address))
      self._connections[e.conn.address] = tab

  def on_connection_failed(self, e):
    wx.MessageBox("Unable to connect to {}:{}!".format(e.remote, e.port), "Connection failed!", wx.OK | wx.ICON_ERROR)

  def on_disconnected(self, e):
    if e.conn.address in self._connections:
      tab = self._connections[e.conn.address]
      tab.connected = False
      tab.log.AppendText("Disconnected from {0[0]}:{0[1]}\n".format(e.conn.address))
      tab.command.SetEditable(False)

  def on_message(self, e):
    try:
      if e.conn.address in self._connections:
        tab = self._connections[e.conn.address]
        tab.log.AppendText("[{}] {}\n".format(e.context, e.msg))
    except:
      pass

  def on_connect(self, e):
    dlg = ConnectToDialog(self)
    if dlg.ShowModal() == wx.ID_OK:
      try:
        normalized = unicodedata.normalize('NFKD', dlg.remote.GetValue().strip(' \r\n\t')).encode('ascii', 'ignore')
        parts = normalized.split(':')
        address = parts[0]
        try:
          port = int(parts[1])
        except:
          port = 5000
        socket.inet_aton(address)
        remote = (address, port)
      except:
        wx.MessageBox("The supplied address '{}' is invalid.".format(normalized), "Connection refused!", wx.OK | wx.ICON_ERROR)
        return

      if remote in self._connections and self._connections[remote].connected:
        wx.MessageBox("Already connected to {0[0]}:{0[1]}!".format(remote), "Connection refused!", wx.OK | wx.ICON_ERROR)
      else:
        self._networking_thread.connect_to(remote[0], remote[1])

  def on_disconnect(self, e):
    tab = self.tabs.GetCurrentPage()
    if tab:
      self._connections.pop(tab._conn.address, None)
      self._networking_thread.disconnect_from(tab._conn)
      self.tabs.DeletePage(tab._idx)

  def on_exit(self, e):
    self.Close()

  def on_documentation(self, e):
    pass

  def on_about(self, e):
    pass

class App(wx.App):
  def OnInit(self):
    self.frame = ConsoleFrame()
    self.frame.Show(True)
    self.SetTopWindow(self.frame)
    return True

app = App(0)
app.MainLoop()
