project "GLEW"
    group "Dependencies"
    kind "StaticLib"
    language "C"

    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin-obj/%{cfg.buildcfg}")

    files
    {
        "src/glew.c",
        "include/GL/*.h"
    }

    includedirs
    {
        "include"
    }

    filter "system:linux"
        pic "On"

    filter "system:windows"
        defines "_CRT_SECURE_NO_WARNINGS"

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
