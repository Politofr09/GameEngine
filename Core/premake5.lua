project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/intermediates/%{cfg.buildcfg}"

    files {
        "src/**"
    }
    
    includedirs {
        "**",
		"../vendor/GLFW/include/"
	}
	
	libdirs {
		"../vendor/GLFW/bin/GLFW/"
	}

	links {
        "opengl32",
        "GLFW"
    }

	filter "system:linux"
        pic "On"
        systemversion "latest"
        links { "X11", "dl", "pthread" } -- Link against X11 and other necessary libraries

	
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"