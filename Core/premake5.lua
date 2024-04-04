project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/intermediates/%{cfg.buildcfg}"

    files {
        "src/Core/**",
        "src/Events/**",
        "../vendor/stb/**.h",
        "../vendor/stb/**.cpp",
        "../vendor/glm/glm/**.hpp",
    }
    
    includedirs {
        "**",
		"../vendor/GLFW/include/",
        "../vendor/GLEW/include/",
        "../vendor/stb/",
        "../vendor/glm/"
	}
	
	libdirs {
		"../vendor/GLFW/bin/GLFW/",
        "../vendor/GLEW/bin/%{cfg.buildcfg}"
	}

    filter "system:windows"
        links {
            "GLEW",
            "GLFW",
            "opengl32",
        }

    filter "system:linux"
        links {
            "GLEW",
            "GLFW",
            "GL",
        }
        
    filter "configurations:Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"