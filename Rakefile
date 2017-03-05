# ===-- Rakefile -----------------------------------------*- mode: Ruby -*-=== #
#
#                              __ __     _   _
#                             |  |  |___| |_|_|
#                             |_   _| -_|  _| |
#                               |_| |___|_| |_|
#
#        This file is distributed under the terms described in LICENSE.
#
# ===----------------------------------------------------------------------=== #

# TODO(mtwilliams): --analyze

require 'active_support'
require 'active_support/core_ext'

PLATFORMS = %w{windows mac linux}
ARCHITECTURES = %w{x86 x86_64}
CONFIGURATIONS = %w{debug development release}

module Defaults
  def self.toolchain
    case self.platform
      when "windows"
        "msvc"
      when "mac"
        "clang"
      when "linux"
        "gcc"
    end
  end

  def self.platform
    case RbConfig::CONFIG["host_os"]
      when /mswin|windows|mingw|cygwin/i
        "windows"
      when /darwin/i
        "mac"
      when /linux/i
        "linux"
    end
  end

  def self.architecture
    if self.platform == "mac"
      return {"i386" => "x86", "x86_64" => "x86_64"}[ENV['ARCHS'].downcase] if ENV['ARCHS']
    end

    "x86_64"
  end

  def self.configuration
    if self.platform == "mac"
      return ENV['CONFIGURATION'].downcase if ENV['CONFIGURATION']
    end

    "development"
  end
end

namespace :engine do
  task :build, [:platform, :architecture, :configuration] do |t, args|
    args.with_defaults(
      :platform => Defaults.platform,
      :architecture => Defaults.architecture,
      :configuration => Defaults.configuration)

    toolchain = "clang".inquiry
    platform = args[:platform].inquiry
    architecture = args[:architecture].inquiry
    configuration = args[:configuration].inquiry

    raise "Unknown or unsupported platform!" unless PLATFORMS.include?(platform)
    raise "Unknown architecture!" unless ARCHITECTURES.include?(architecture)
    raise "Unknown configuration!" unless CONFIGURATIONS.include?(configuration)

    FileUtils.mkdir_p ['_build/unity', '_build/bin', '_build/lib']

    cxx = case toolchain
            when "gcc" then "g++"
            when "clang" then "clang++"
            else toolchain end

    unity = "_build/unity/yeti.cc"
    sources = Dir.glob("src/**/*.cc")
    File.write unity, sources.map {|source| "#include \"#{File.expand_path(source)}\""}.join("\n")

    image = "lib" + mangle("yeti", platform, architecture, configuration) + ".dylib"
    cflags = flags_for_engine(toolchain, platform, architecture, configuration).compact.join(' ')

    success = system(ENV.to_h, "#{cxx} #{cflags} -dynamiclib -install_name @executable_path/#{image} -o _build/bin/#{image} #{unity}")
    raise "Failed!" unless success
  end
end

namespace :runtime do
  task :build, [:platform, :architecture, :configuration] do |t, args|
    args.with_defaults(
      :platform => Defaults.platform,
      :architecture => Defaults.architecture,
      :configuration => Defaults.configuration)

    toolchain = "clang".inquiry
    platform = args[:platform].inquiry
    architecture = args[:architecture].inquiry
    configuration = args[:configuration].inquiry

    raise "Unknown or unsupported platform!" unless PLATFORMS.include?(platform)
    raise "Unknown architecture!" unless ARCHITECTURES.include?(architecture)
    raise "Unknown configuration!" unless CONFIGURATIONS.include?(configuration)

    FileUtils.mkdir_p ['_build/unity', '_build/bin', '_build/lib']

    cxx = case toolchain
            when "gcc" then "g++"
            when "clang" then "clang++"
            else toolchain end

    unity = "_build/unity/runtime.cc"
    sources = Dir.glob("runtime/src/**/*.cc")
    File.write unity, sources.map {|source| "#include \"#{File.expand_path(source)}\""}.join("\n")

    image = "_build/bin/" + mangle("runtime", platform, architecture, configuration)
    cflags = flags_for_runtime(toolchain, platform, architecture, configuration).compact.join(' ')

    success = system(ENV.to_h, "#{cxx} #{cflags} -o #{image} #{unity}")
    raise "Failed!" unless success
  end
end

namespace :resource_compiler do
  task :build, [:platform, :architecture, :configuration] do |t, args|
    args.with_defaults(
      :platform => Defaults.platform,
      :architecture => Defaults.architecture,
      :configuration => Defaults.configuration)

    toolchain = "clang".inquiry
    platform = args[:platform].inquiry
    architecture = args[:architecture].inquiry
    configuration = args[:configuration].inquiry

    raise "Unknown or unsupported platform!" unless PLATFORMS.include?(platform)
    raise "Unknown architecture!" unless ARCHITECTURES.include?(architecture)
    raise "Unknown configuration!" unless CONFIGURATIONS.include?(configuration)

    FileUtils.mkdir_p ['_build/unity', '_build/bin', '_build/lib']

    cxx = case toolchain
            when "gcc" then "g++"
            when "clang" then "clang++"
            else toolchain end

    unity = "_build/unity/resource_compiler.cc"
    sources = Dir.glob("tools/resource_compiler/src/**/*.cc")
    File.write unity, sources.map {|source| "#include \"#{File.expand_path(source)}\""}.join("\n")

    image = "_build/bin/" + mangle("resource_compiler", platform, architecture, configuration)
    cflags = flags_for_tool('resource_compiler', toolchain, platform, architecture, configuration).compact.join(' ')

    success = system(ENV.to_h, "#{cxx} #{cflags} -o #{image} #{unity}")
    raise "Failed!" unless success
  end
end

# See https://clang.llvm.org/docs/CommandGuide/clang.html

def flags(toolchain, platform, architecture, configuration, options = {})
  [
    # We target C++11
    "--std=c++11",

    # ..without all the muck.
    "-fno-rtti",
    "-fno-exceptions",
    "-fno-cxx-exceptions",

    # Link to `libc++` instead of `libstdc++`.
    "-stdlib=libc++",

    # Keep us honest.
    "--pedantic",
    "-Wall",
    #"-Werror",
    "-Wdocumentation",
    "-Wextra-tokens",
    "-Wambiguous-member-template",
    "-Wbind-to-temporary-copy",

    # Make concessions too useful extensions.
    "-Wno-variadic-macros",
    "-Wno-gnu-zero-variadic-macro-arguments",
    "-Wno-gnu-anonymous-struct",
    "-Wno-nested-anon-types",
    "-Wno-zero-length-array",

    # Generate debug information for debug and development builds.
    if configuration.debug? or configuration.development?
      "-g"
    else
      "-g0"
    end,

    if !configuration.debug?
      "-DNDEBUG"
    end,

    # Don't optimize debug builds so we can (easily) poke around disassembly.
    if configuration.debug?
      "-O0"
    else
      "-O3"
    end,

    # We target Mavericks and later.
    "-mmacosx-version-min=10.9",

    if toolchain.clang?
      aliased = {'x86' => 'i386', 'x86_64' => 'x86_64'}[architecture]
      if platform.linux?
        "--target=#{aliased}-unknown-linux-elf"
      elsif platform.mac?
        "--target=#{aliased}-apple-darwin-macho"
      end
    end,

    if toolchain.clang? or toolchain.gcc?
      if architecture.x86?
        "-m32"
      elsif architecture.x86_64?
        "-m64"
      end
    end,

    # Generate position independent code.
    "-fPIC",

    # TODO(mtwilliams): Figure out where this breaks ABI.
    # We manually vectorize.
    # "-mno-sse",

    # Search paths.
    "-Iinclude",
    "-L_build/bin",
    "-L_build/lib",
    "-I_deps/luajit/include",
    "-L_deps/luajit/_build/lib",
    "-L_deps/luajit/_build/bin",
    "-I_deps/sqlite3/include",
    "-L_deps/sqlite3/_build/lib",
    "-L_deps/sqlite3/_build/bin",

    # Defines.
    "-DYETI_LINKAGE=YETI_LINKAGE_DYNAMIC",
    "-DGALA_LINKAGE=GALA_LINKAGE_STATIC",
    "-DYETI_CONFIGURATION=YETI_CONFIGURATION_#{configuration.upcase}",
    "-DGALA_CONFIGURATION=GALA_CONFIGURATION_#{configuration.upcase}"
  ]
end

def flags_for_engine(toolchain, platform, architecture, configuration, options = {})
  common = flags(toolchain, platform, architecture, configuration, options)

  specific = [
    "-D__YETI_IS_BEING_COMPILED__=1",
    "-D__YETI_VERSION__=\"\\\"#{`git rev-parse HEAD`.rstrip}\\\"\"",
    "-D__YETI_REVISION__=#{`git rev-list --count HEAD`.strip.to_i}",
    "-D__YETI_COPYRIGHT__=\"\\\"Copyright 2013-#{Time.new.year} Origami Comet Games, Inc. All rights reserved.\\\"\"",

    "-l" + mangle("luajit", platform, architecture, configuration),
    "-l" + mangle("sqlite3", platform, architecture, configuration)
  ]

  common + specific
end

def flags_for_tool(tool, toolchain, platform, architecture, configuration, options = {})
  common = flags(toolchain, platform, architecture, configuration, options)

  specific = [
    "-Itools/#{tool}/include",

    "-l" + mangle("luajit", platform, architecture, configuration),
    "-l" + mangle("sqlite3", platform, architecture, configuration),
    "-l" + mangle("yeti", platform, architecture, configuration),

    # LuaJIT needs to allocate in the lower 2GiB of address space.
    if platform.mac? and architecture.x86_64?
      ["-pagezero_size 10000", "-image_base 100000000"]
    end
  ]

  common + specific
end

def flags_for_runtime(toolchain, platform, architecture, configuration, options = {})
  common = flags(toolchain, platform, architecture, configuration, options)

  specific = [
    "-Iruntime/include",

    "-l" + mangle("luajit", platform, architecture, configuration),
    "-l" + mangle("sqlite3", platform, architecture, configuration),
    "-l" + mangle("yeti", platform, architecture, configuration),

    # LuaJIT needs to allocate in the lower 2GiB of address space.
    if platform.mac? and architecture.x86_64?
      ["-pagezero_size 10000", "-image_base 100000000"]
    end
  ]

  common + specific
end

def mangle(name, platform, architecture, configuration)
  bits = {"x86" => "32", "x86_64" => "64"}[architecture]
  "#{name}_#{configuration}_#{platform}_#{bits}"
end
