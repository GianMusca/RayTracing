-- premake5.lua
workspace "Trenzalore"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Trenzalore"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "Trenzalore"