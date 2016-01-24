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

# HACK(mtwilliams): Use Rake until Ryb is complete.
 # TODO(mtwilliams): Use file-based rules.

# Use like so:
 # rake yeti:build[debug,windows,x86] TOOLCHAIN=vs2015 LINKAGE=static
  #=> yeti_debug_windows_32.lib

require 'visual_studio'
require 'fileutils'

CONFIGURATIONS = %i{debug development release}
PLATFORMS = %i{windows}
ARCHITECTURES = %i{x86 x86_64}

namespace :yeti do
  task :unity => ['fs'] do
    unity = "_build/_unities/yeti.cc"
    sources = Dir["src/**/*.cc"].sort.map{|src| File.realpath(src)}
    FileUtils.rm(unity, force: true)
    puts "Generating #{unity}..."
    File.open(unity, "wb") do |unity|
      sources.each do |source|
        puts "==> #{source}"
        unity.write("#include \"#{source}\"\n")
      end
    end
  end

  task :build, [:configuration, :platform, :architecture] => ['yeti:unity', 'fs'] do |_, args|
    puts "Building Yeti..."

    configuration = args[:configuration].to_sym
    configuration_is_known = CONFIGURATIONS.include?(configuration)
    raise "Unknown configuration '#{configuration}'!" unless configuration_is_known

    platform = args[:platform].to_sym
    platform_is_supported = PLATFORMS.include?(platform)
    raise "Unsupported platform '#{platform}'!" unless platform_is_supported

    architecture = args[:architecture].to_sym
    architecture_is_supported = ARCHITECTURES.include?(architecture)
    raise "Unsupported architecture '#{architecture}'!" unless architecture_is_supported

    triplet = [configuration, platform, Hash[ARCHITECTURES.zip(%w{32 64})][architecture]].join('_')

    # TODO(mtwilliams): Refactor this.
    case platform
      when :windows
        raise "You don't have Visual Studio installed!" unless VisualStudio.available?

        # TODO(mtwilliams): Support other toolchains.
        vs = if ENV['toolchain']
          supported = VisualStudio::NAMES.include?(ENV['toolchain'])
          raise "Unsupported toolchain '#{ENV['toolchain']}'!" unless supported
          vs = VisualStudio.find_by_name(ENV['toolchain'])
          raise "You don't have #{VisualStudio::NAME_TO_PRETTY_NAME[ENV['toolchain']]} installed." unless vs
          vs
        else
          vs = VisualStudio.latest
          puts "==> Using #{vs.name.pretty}."
          vs
        end

        vc = vs.products[:c_and_cpp]

        platform_is_supported = vc.supports[:platforms].include?(platform)
        raise "#{vs.name.pretty} doesn't support '#{platform}'!" unless platform_is_supported
        sdk = vc.sdks[:windows].first
        architecture_is_supported = vc.supports[:architectures].include?(architecture)
        raise "#{vs.name.pretty} doesn't support '#{architecture}'!" unless architecture_is_supported

        env = vc.environment(target: {platform: platform, architecture: architecture})

        compiler_args = []

        # Suppress the annoying startup banner.
        compiler_args << "/nologo"

        # Don't link after compilation.
        compiler_args << "/c"

        # Be vocal about shit code.
        compiler_args << "/W4"

        # Use appropriate system and compiler include paths.
        sdk.includes.each{|path| compiler_args << "/I#{path}" }
        vc.includes.each{|path| compiler_args << "/I#{path}" }

        # Search our includes.
        compiler_args << "/Iinclude"

        # Compile for the appropriate architecture.
        case architecture
          when :x86
            compiler_args << "/arch:IA32"
          end

        # Generate debug symbols, as appropriate.
        case configuration
          when /(debug|development)/
            # Generate debug symbols.
            compiler_args << "/MDd" << "/Zi"
          when :release
            # Don't generate debug symbols.
            compiler_args << "/MD"
          end

        # Fuck off, RTTI.
        compiler_args << "/GR-"

        # Optimize, as appropriate.
        case configuration
          when :debug
            # Don't optimize.
            compiler_args << "/Od" << "/RTCsu" << "/fp:precise" << "/fp:except"
          when /(development|release)/
            # Optimize for speed.
            compiler_args << "/Ox" << "/fp:fast" << "/fp:except-"
          end

        # Don't optimize for one architecture at the loss of another.
        compiler_args << "/favor:blend"


        compiler_args << '/D__YETI_IS_BEING_COMPILED__=1'
        compiler_args << '/DYETI_LINKAGE=YETI_LINKAGE_STATIC'
        compiler_args << "/DYETI_CONFIGURATION=YETI_CONFIGURATION_#{configuration.upcase}"
        compiler_args << '/D_HAS_EXCEPTIONS=0'
        compiler_args << '/D_SCL_SECURE_NO_WARNINGS=1'
        compiler_args << '/D_CRT_SECURE_NO_WARNINGS=1'
        compiler_args << '/D_CRT_SECURE_NO_DEPRECATE=1'
        compiler_args << '/D_SECURE_SCL_THROWS=0'
        compiler_args << '/D_SILENCE_DEPRECATION_OF_SECURE_SCL_THROWS=1'
        compiler_args << '/D_USING_V110_SDK71_=1'

        case configuration
          when :debug
            compiler_args << '/D_DEBUG=1'
            compiler_args << '/D_HAS_ITERATOR_DEBUGGING=1'
            compiler_args << '/D_SECURE_SCL=1'
          when /(development|release)/
            compiler_args << '/DNDEBUG=1'
            compiler_args << '/D_HAS_ITERATOR_DEBUGGING=0'
            compiler_args << '/D_SECURE_SCL=0'
          end

        # Compile!
        puts "==> Compiling _build/obj/yeti_#{triplet}.obj..."
        system(env,
               "cl.exe",
               *compiler_args,
               "/Fd_build/lib/yeti_#{triplet}.pdb",
               "/Fo_build/obj/yeti_#{triplet}.obj",
               "_build/_unities/yeti.cc")

        # Link!
        puts "==> Linking _build/lib/yeti_#{triplet}.lib..."
        system(env,
               "lib.exe", "/nologo",
               "/OUT:_build/lib/yeti_#{triplet}.lib",
               "_build/obj/yeti_#{triplet}.obj")

        puts "Done!"
      else
        raise "Not implemented yet."
      end
  end
end

namespace :runtime do
  task :unity => ['fs'] do
    unity = "_build/_unities/runtime.cc"
    sources = Dir["runtime/src/**/*.cc"].sort.map{|src| File.realpath(src)}
    FileUtils.rm(unity, force: true)
    puts "Generating #{unity}..."
    File.open(unity, "wb") do |unity|
      sources.each do |source|
        puts "==> #{source}"
        unity.write("#include \"#{source}\"\n")
      end
    end
  end

  task :build, [:configuration, :platform, :architecture] => ['runtime:unity', 'fs'] do |_, args|
    puts "Building Runtime..."

    configuration = args[:configuration].to_sym
    configuration_is_known = CONFIGURATIONS.include?(configuration)
    raise "Unknown configuration '#{configuration}'!" unless configuration_is_known

    platform = args[:platform].to_sym
    platform_is_supported = PLATFORMS.include?(platform)
    raise "Unsupported platform '#{platform}'!" unless platform_is_supported

    architecture = args[:architecture].to_sym
    architecture_is_supported = ARCHITECTURES.include?(architecture)
    raise "Unsupported architecture '#{architecture}'!" unless architecture_is_supported

    triplet = [configuration, platform, Hash[ARCHITECTURES.zip(%w{32 64})][architecture]].join('_')

    # TODO(mtwilliams): Refactor this.
    case platform
      when :windows
        raise "You don't have Visual Studio installed!" unless VisualStudio.available?

        # TODO(mtwilliams): Support other toolchains.
        vs = if ENV['toolchain']
          supported = VisualStudio::NAMES.include?(ENV['toolchain'])
          raise "Unsupported toolchain '#{ENV['toolchain']}'!" unless supported
          vs = VisualStudio.find_by_name(ENV['toolchain'])
          raise "You don't have #{VisualStudio::NAME_TO_PRETTY_NAME[ENV['toolchain']]} installed." unless vs
          vs
        else
          vs = VisualStudio.latest
          puts "==> Using #{vs.name.pretty}."
          vs
        end

        vc = vs.products[:c_and_cpp]

        platform_is_supported = vc.supports[:platforms].include?(platform)
        raise "#{vs.name.pretty} doesn't support '#{platform}'!" unless platform_is_supported
        sdk = vc.sdks[:windows].first
        architecture_is_supported = vc.supports[:architectures].include?(architecture)
        raise "#{vs.name.pretty} doesn't support '#{architecture}'!" unless architecture_is_supported

        env = vc.environment(target: {platform: platform, architecture: architecture})

        compiler_args = []
        linker_args = []

        # Suppress the annoying startup banner.
        compiler_args << "/nologo"
        linker_args << "/nologo"

        # Don't link after compilation.
        compiler_args << "/c"

        # Don't create or embed a manifest file.
        linker_args << "/manifest:no"

        # Link for the appropriate architecture.
        case architecture
          when :x86
            linker_args << "/machine:X86"
          when :x86_64
            linker_args << "/machine:X64"
          end

        # Be vocal about shit code.
        compiler_args << "/W4"

        # Use appropriate system and compiler include paths.
        sdk.includes.each{|path| compiler_args << "/I#{path}" }
        vc.includes.each{|path| compiler_args << "/I#{path}" }

        # Use appropriate system and compiler libraries.
        sdk.libraries[architecture].each{|path| linker_args << "/LIBPATH:#{path}" }
        vc.libraries[architecture].each{|path| linker_args << "/LIBPATH:#{path}" }

        # Search our includes.
        compiler_args << "/Iinclude"
        compiler_args << "/Iruntime/include"

        # Search our libraries.
        linker_args << "/LIBPATH:_build/lib"

        # Compile for the appropriate architecture.
        case architecture
          when :x86
            compiler_args << "/arch:IA32"
          end

        # Generate debug symbols, as appropriate.
        case configuration
          when /(debug|development)/
            # Generate debug symbols.
            compiler_args << "/MDd" << "/Zi"
            linker_args << "/DEBUG"
          when :release
            # Don't generate debug symbols.
            compiler_args << "/MD"
          end

        # Fuck off, RTTI.
        compiler_args << "/GR-"

        # Optimize, as appropriate.
        case configuration
          when :debug
            # Don't optimize.
            compiler_args << "/Od" << "/RTCsu" << "/fp:precise" << "/fp:except"
          when /(development|release)/
            # Optimize for speed.
            compiler_args << "/Ox" << "/fp:fast" << "/fp:except-"
          end

        # Don't optimize for one architecture at the loss of another.
        compiler_args << "/favor:blend"

        compiler_args << '/DYETI_LINKAGE=YETI_LINKAGE_STATIC'
        compiler_args << "/DYETI_CONFIGURATION=YETI_CONFIGURATION_#{configuration.upcase}"
        compiler_args << '/D_HAS_EXCEPTIONS=0'
        compiler_args << '/D_SCL_SECURE_NO_WARNINGS=1'
        compiler_args << '/D_CRT_SECURE_NO_WARNINGS=1'
        compiler_args << '/D_CRT_SECURE_NO_DEPRECATE=1'
        compiler_args << '/D_SECURE_SCL_THROWS=0'
        compiler_args << '/D_SILENCE_DEPRECATION_OF_SECURE_SCL_THROWS=1'
        compiler_args << '/D_USING_V110_SDK71_=1'

        case configuration
          when :debug
            compiler_args << '/D_DEBUG=1'
            compiler_args << '/D_HAS_ITERATOR_DEBUGGING=1'
            compiler_args << '/D_SECURE_SCL=1'
          when /(development|release)/
            compiler_args << '/DNDEBUG=1'
            compiler_args << '/D_HAS_ITERATOR_DEBUGGING=0'
            compiler_args << '/D_SECURE_SCL=0'
          end

        # Compile!
        puts "==> Compiling _build/obj/runtime_#{triplet}.obj..."
        system(env,
               "cl.exe",
               *compiler_args,
               "/Fd_build/bin/runtime_#{triplet}.pdb",
               "/Fo_build/obj/runtime_#{triplet}.obj",
               "_build/_unities/runtime.cc")

        # Link!
        puts "==> Linking _build/bin/runtime_#{triplet}.exe..."
        system(env,
               "link.exe",
               *linker_args,
               "/OUT:_build/bin/runtime_#{triplet}.exe",
               "_build/obj/runtime_#{triplet}.obj")

        puts "Done!"
      else
        raise "Not implemented yet."
      end
  end
end

task :fs do
  ['_build/_unities', '_build/obj', '_build/bin', '_build/lib'].each do |path|
    FileUtils.mkdir_p(path)
  end
end

task :build, [:configuration] do |_, args|
  configurations = [*args[:configuration]]
  configurations = CONFIGURATIONS if args[:configuration].nil?
  # TODO(mtwilliams): Detect the current platform.
  platforms = [:windows]
  architectures = ARCHITECTURES

  Rake::Task['fs'].execute
  Rake::Task['yeti:unity'].execute
  Rake::Task['runtime:unity'].execute

  architectures.each do |architecture|
    platforms.each do |platform|
      configurations.each do |configuration|
        Rake::Task['yeti:build'].execute(:configuration => configuration,
                                         :platform => platform,
                                         :architecture => architecture)
        Rake::Task['runtime:build'].execute(:configuration => configuration,
                                            :platform => platform,
                                            :architecture => architecture)
      end
    end
  end
end

task :clean do
  Dir["_build/*"].each{|artifact| FileUtils.rm_rf(artifact)}
end
