workspace "VoxelStream"
   architecture "x64"
   configurations { "Debug", "Release"}
   startproject "Test"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Test"
	include "Test/Test.lua"
group ""

include "VoxelStream/VoxelStream.lua"