#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma once

namespace Core::Input
{
	class Mouse
	{
	private:
		static glm::vec2 mousePosition;
		static glm::vec2 mouseDelta;
		static float scrollOffset;

		static GLFWwindow* m_Window;

	public:

		static void Init(GLFWwindow* window);
		static glm::vec2 GetMousePosition();
		static glm::vec2 GetMouseDelta();
		static float GetScrollOffset();

		static bool IsButtonPressed(int button);
	};
}