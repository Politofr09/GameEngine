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
		"../vendor/GLFW/include/",
        "../vendor/GLEW/include/"
	}
	
	libdirs {
		"../vendor/GLFW/bin/GLFW/",
        "../vendor/GLEW/bin/%{cfg.buildcfg}"
	}
    
    links {
        "GLEW",
        "GLFW",
    }

    filter "system:windows"
        links {
            "opengl32",
        }

    filter "system:linux"
        links {
            "GL",
        }
        
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"