-- premake5.lua
workspace "GameEngine"
    configurations { "Debug", "Release" }
    startproject "Sandbox"

    defines "GLEW_STATIC"

-- Git submodules
group "Dependencies"
    include "vendor/GLEW"
    include "vendor/GLFW"
    include "vendor/assimp"
group ""

include "Core"
include "Sandbox"