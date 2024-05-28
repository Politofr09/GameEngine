project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    
    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/intermediates/%{cfg.buildcfg}"

    files {
        "src/Core/**",
        "src/Events/**",
        "src/Renderer/**",
        "../vendor/stb/**.h",
        "../vendor/stb/**.cpp",
        "../vendor/imgui/**.cpp"
    }
    
    includedirs {
        "**",
		"../vendor/GLFW/include/",
        "../vendor/GLEW/include/",
        "../vendor/stb/",
        "../vendor/glm/",
        "../vendor/assimp/include/",
        "../vendor/"
	}
	
	libdirs {
		"../vendor/GLFW/bin/GLFW/",
        "../vendor/GLEW/bin/%{cfg.buildcfg}"
	}

    links {
        "GLEW",
        "GLFW",
        "assimp"
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
        runtime "Debug"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"
        runtime "Release"