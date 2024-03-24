project "GLEW"
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

    defines "GLEW_STATIC"

    filter "system:linux"
        pic "On"

    filter "system:windows"
        defines "_CRT_SECURE_NO_WARNINGS"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
