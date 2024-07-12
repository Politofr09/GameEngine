project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/intermediates/%{cfg.buildcfg}"

    files {
        "src/**"
    }

    includedirs {
        "../Core/src/",
        "../vendor/GLFW/include/",
        "../vendor/GLEW/include/",
        "../vendor/assimp/include/",
        "../vendor/glm",
        "../vendor/",
    }

	libdirs {
		"../vendor/GLFW/bin/GLFW/",
        "../vendor/GLEW/bin/%{cfg.buildcfg}",
        "../vendor/assimp/bin/assimp/",
        "../Core/bin/%{cfg.buildcfg}"
    }   

    links {
        "assimp",
        "Core",
    }

    filter "system:windows"
        links {
            "Opengl32",
        }

    filter "system:linux"
        links {
            "GL",
        }

    filter "configurations:Debug"
        runtime "Debug"
        defines "DEBUG"
        symbols "on"
        optimize "Off"  -- Turn off optimization for debug builds

    filter "configurations:Release"
        runtime "Release"
        defines "RELEASE"
        optimize "on"