workspace "ini-parser-cpp"
  configurations { "Debug", "Release" }
  platforms { "x64" }
  location "build"

project "ini-parser-cpp"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  warnings "Extra"
  targetname "ini-parser-cpp"
  targetdir "build"

  buildoptions { "-pedantic" }
  includedirs { "src" }

  files { "src/**.cc" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
    optimize "Off"

  filter "configurations:Release"
    defines { "NDEBUG" }
    symbols "Off"
    optimize "Full"
