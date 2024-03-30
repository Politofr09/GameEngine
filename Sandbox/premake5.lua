project "Sandbox"
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
        "../vendor/"
    }

	libdirs {
		"../vendor/GLFW/bin/GLFW/",
        "../vendor/GLEW/bin/%{cfg.buildcfg}"
	}   

    links {
        "GLEW",
        "GLFW",
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
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"