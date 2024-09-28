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
        "src/Ecs/**",
        "../vendor/stb/**.h",
        "../vendor/stb/**.cpp",
        "../vendor/imgui/**.cpp",
        "../vendor/tinyfiledialogs/tinyfiledialogs.c"
    }

    includedirs {
        "**",
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
        "../vendor/assimp/bin/%{cfg.buildcfg}/",
        "../vendor/yaml-cpp/bin/%{cfg.buildcfg}/"
    }

    links {
        "glew",
        "GLFW",
        "assimp",
        "yaml-cpp"
    }

    filter "system:windows"
        links {
            "opengl32",
        }

    filter "system:linux"
        buildoptions { "-fPIC" }
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
