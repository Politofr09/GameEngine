#include <GLFW/glfw3.h>

#pragma once

namespace Core::Input
{
	class Keyboard
	{
	private:
		static bool keys[65536];
		static bool keysDown[65536];
		static void glfw_event_poll(GLFWwindow* window, int key, int scancode, int action, int mods);

	public:
		static bool IsKeyDown(int keyCode);
		static bool IsKeyPressed(int keyCode);

		// Sets the GLFW callbacks
		static void Init(GLFWwindow* window);
	};
}