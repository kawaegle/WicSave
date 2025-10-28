workspaceName = "WICGames"

workspace (workspaceName)
  configurations { "Debug", "Release"}
  objdir ".obj/%{prj.name}/%{cfg.buildcfg}"
  targetdir "bin/%{cfg.buildcfg}"

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
    runtime "Debug"  -- /MDd
    flags { "MultiProcessorCompile" }
    warnings "Extra"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "Full"
    runtime "Release"  -- /MD
    flags { "MultiProcessorCompile" }

  filter("system:linux")
    defines({ "LINUX", "__linux__" })

  filter("system:Windows")
    defines({ "_WIN32", "WIN32_LEAN_AND_MEAN", "_WIN32_WINNT=0x0A00"})

filter {}

cdialect "C99"
cppdialect "C++20"

filter({})

include ("./WicSave_premake5.lua")
include("./gtest_premake5.lua")

project ("unit_test")
  kind "ConsoleApp"
  location "./unit_test/"

  files({
    "./unit_test/*.cpp"
  })
  include_wicsave()
  include_gtest()
  link_gtest()
  link_wicsave()