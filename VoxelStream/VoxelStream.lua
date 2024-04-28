project "VoxelStream"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp", "Include/**.h" }

   includedirs
   {
      "src",
	  "Include",
	  "vendor/Cereal1.3.2/include"
   }

   targetdir ("../lib/%{cfg.buildcfg}")
   objdir ("..lib-int/%{cfg.buildcfg}")

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"
