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
    
    filter "system:windows"
        links {
            "GLFW",
            "opengl32",
        }

    filter "system:linux"
        links {
            "GLFW",
            "GL"
        }
        
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"