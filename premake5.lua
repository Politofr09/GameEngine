-- premake5.lua
workspace "GameEngine"
    configurations { "Debug", "Release" }
    startproject "Sandbox"

include "vendor/GLFW"
include "Core"
include "Sandbox"