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
        "../vendor/GLFW/include/"
    }

    filter "system:windows"
        links {
            "Core",
            "GLFW",
            "Opengl32",
        }

    filter "system:linux"
        links {
            "Core",
            "GLFW",
            "GL",
        }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "RELEASE"
        optimize "on"