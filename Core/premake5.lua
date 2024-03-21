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
	
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"