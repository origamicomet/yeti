function collect_options()
  local options = {}

  for i,v in ipairs(_ARGS) do
    if v:find("--") == 1 then
      local eq = v:find("=")
      if eq then options[v:sub(3, eq - 1)] = v:sub(eq + 1, -1)
      else options[v:sub(3, -1)] = true end
    end
  end

  return options
end

function determine_commit()
  return os.outputof("git rev-parse HEAD"):gsub("\n", "")
end

function collect_build_info()
  return({
    ["tool"]      = _ACTION,
    ["platform"]  = os.get(),
    ["options"]   = collect_options(),
    ["commit"]    = determine_commit(),
    ["build_dir"] = string.format("%s/%s", os.get(), _ACTION),
  })
end

local dispatch = {}

dispatch.gmake = function()
  _action = "build"
  return dispatch["build"]()
end

dispatch.vs2010 = function()
  _action = "build"
  return dispatch["build"]()
end

dispatch.vs2008 = function()
  _action = "build"
  return dispatch["build"]()
end

dispatch.vs2005 = function()
  _action = "build"
  return dispatch["build"]()
end

dispatch.build = function()
  build_info = collect_build_info()

  solution("lwe")
    location(build_info.build_dir)
    configurations({ "debug", "development", "release" })

    project("engine")
      kind("WindowedApp")
      language("C++")

      objdir(string.format("%s/obj/lwe", build_info.build_dir))
      targetdir(string.format("%s/bin", build_info.build_dir))
      targetname("lwe")

      configuration("debug")
        targetsuffix("-dbg")
        flags({ "Symbols" })
        defines("_DEBUG")

      configuration("development")
        targetsuffix("-dev")
        flags({ "Symbols" })
        defines("_DEVELOPMENT")

      configuration("release")
        flags({ "Optimize", "EnableSSE", "EnableSSE2" })

      configuration("windows")
        links("DbgHelp")
        files("../src/**.rc")

      configuration("not windows")
        excludes("../src/**/win32_*.cc");

      configuration({})
        includedirs("../include")
        files({ "../include/**.h", "../src/**.c", "../src/**.cc" })

        local render_devices = {
          ["d3d11"] = function()
            excludes({ "../src/**/d3d9_*.cc", "../include/**/d3d9_*.h" })
            excludes({ "../src/**/gl31_*.cc", "../include/**/gl31_*.h" })
            excludes({ "../src/**/gles_*.cc", "../include/**/gles_*.h" })
            excludes({ "../src/**/gcm_*.cc", "../include/**/gcm_*.h" })

            if build_info.platform == "windows" then
              links({ "dxgi", "d3d11" })
            else
              print(string.format("Error: the render device `d3d11` is not supported on `%s`.", build_info.platform))
              os.exit()
            end
          end,
        }

        local render_device = build_info.options["render-device"]

        if not render_device then
          print("Error: no render device specified!")
          os.exit()
        elseif not render_devices[render_device] then
          print(string.format("Error: the render device `%s` doesn't exist", render_device))
          os.exit()
        end

        render_devices[render_device]()
end

if _ACTION and dispatch[_ACTION] then
  _action = _ACTION
  dispatch[_ACTION]()
end