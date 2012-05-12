import Options
from os import unlink, symlink
from os.path import exists 

srcdir = "."
blddir = "build"
VERSION = "0.1.0"

def set_options(opt):
  opt.tool_options("compiler_cxx compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx compiler_cc")
  conf.check_tool("node_addon")

def build(bld):
  aeternum = bld.new_task_gen("cc")
  aeternum.target = "aeternum"
  aeternum.source = ["deps/aeternum/aeternum.c"]
  aeternum.includes = "deps/aeternum/"
  aeternum.ccflags = "-fPIC"
  prefork = bld.new_task_gen("cxx", "shlib", "node_addon", add_objects="aeternum")
  prefork.target = "prefork"
  prefork.source = ["src/prefork.cc"]
