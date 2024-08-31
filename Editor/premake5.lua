project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/intermediates/%{cfg.buildcfg}"

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "../Core/src",
        "../vendor/GLFW/include/",
        "../vendor/GLEW/include/",
        "../vendor/stb/",
        "../vendor/glm/",
        "../vendor/assimp/include/",
        "../vendor/"
    }

    libdirs {
        "../vendor/GLFW/bin/GLFW/",
        "../vendor/GLEW/bin/%{cfg.buildcfg}",
        "../vendor/assimp/bin/assimp/",
        "../Core/bin/%{cfg.buildcfg}"
    }

    links {
        "Core",
        "GLEW",
        "GLFW",
        "assimp",
    }

    filter "system:windows"
        links {
            "opengl32",
        }

    filter "system:linux"
        buildoptions { "-fPIC" }
        linkoptions { "-pie" }
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
