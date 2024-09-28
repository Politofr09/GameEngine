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
        "../Core/src/",
        "../vendor/GLFW/include/",
        "../vendor/GLEW/include/",
        "../vendor/stb/",
        "../vendor/glm/",
        "../vendor/assimp/include/",
        "../vendor/yaml-cpp/include/",
        "../vendor/"
    }

    libdirs {
        "../vendor/GLFW/bin/%{cfg.buildcfg}",
        "../vendor/GLEW/bin/%{cfg.buildcfg}",
        "../vendor/assimp/bin/%{cfg.buildcfg}",
        "../Core/bin/%{cfg.buildcfg}"
    }

    links {
        "Core",
        "glew",
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
        symbols "On"
        optimize "Off"  -- Turn off optimization for debug builds
		staticruntime "on"
        runtime "Debug"

    filter "configurations:Release"
        symbols "Off"
        optimize "On"  -- Full optimization for release builds
		staticruntime "off"
        runtime "Release"
