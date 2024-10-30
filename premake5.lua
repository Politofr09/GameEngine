-- premake5.lua
workspace "GameEngine"
    configurations { "Debug", "Release" }
    startproject "Editor"
    staticruntime "on"
    
    defines
    {
        "GLEW_STATIC",
        "YAML_CPP_STATIC_DEFINE"    
    }

    group "Engine"    
        include "Core"
        include "Editor"
        
    group "Dependencies"
        include "vendor/GLEW"
        include "vendor/GLFW"
        include "vendor/yaml-cpp"
        include "vendor/assimp"

	filter "configurations:Debug"
        symbols "On"
        optimize "Off"  -- Turn off optimization for debug builds
		staticruntime "on"
        runtime "Debug"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"  -- Full optimization for release builds
		staticruntime "off"
        runtime "Release"

    filter ""
