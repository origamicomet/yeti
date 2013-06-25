# This file is part of Butane. See README.md and LICENSE.md for details.
# Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

if __name__ == "__main__":
  import sys, os
  sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'deps', 'foundation', 'py'))
  from foundation import network

  def connect(conn):
    print("{} connected".format(conn.address))
    return None

  def disconnect(conn, engine):
    print("{} disconnected".format(conn.address))

  def unhandled(engine, type):
    print("Unhandled packet, type={}".format(type))

  def execute(engine, p, command):
    p.write_str("cmd", command)

  def on_msg(engine, p):
    print("[{}] {}".format(p.read_str("context"), p.read_str("msg")))

  (network.Protocol("console", 1)
    .connect(connect)
    .disconnect(disconnect)
    .unhandled(unhandled)
    .local_to_remote("cmd", execute)
    .remote_to_local("msg", on_msg)
    .host('127.0.0.1', 5000))
