-- premake5.lua
workspace "GameEngine"
    configurations { "Debug", "Release" }
    startproject "Editor"

    defines "GLEW_STATIC"

	filter "configurations:Debug"
        symbols "On"
        optimize "Off"  -- Turn off optimization for debug builds
		runtime "Debug"
		defines "_ITERATOR_DEBUG_LEVEL=2"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"  -- Full optimization for release builds
		runtime "Release"
		defines "_ITERATOR_DEBUG_LEVEL=0"

    filter ""

    -- Git submodules
    group "Dependencies"
        include "vendor/assimp"
        include "vendor/GLEW"
        include "vendor/GLFW"
    group ""

    -- Engine Code
    group "Engine"
        include "Core"
        include "Editor"
    group ""