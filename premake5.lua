-- premake5.lua
workspace "GameEngine"
    configurations { "Debug", "Release" }
    startproject "Sandbox"

    defines "GLEW_STATIC"

include "vendor/GLEW"
include "vendor/GLFW"
include "Core"
include "Sandbox"