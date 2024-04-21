#include "Mouse.h"

glm::vec2 Core::Input::Mouse::mousePosition = glm::vec2(0.0f);
glm::vec2 Core::Input::Mouse::mouseDelta = glm::vec2(0.0f);
float Core::Input::Mouse::scrollOffset = 0.0f;
GLFWwindow* Core::Input::Mouse::m_Window = nullptr;

void Core::Input::Mouse::Init(GLFWwindow* window)
{
	m_Window = window;

	glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
		mouseDelta.x = (float)xpos - mousePosition.x;
		mouseDelta.y = (float)ypos - mousePosition.y;

		mousePosition.x = (float)xpos;
		mousePosition.y = (float)ypos;
	});

	glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
		scrollOffset = (float)yoffset;
	});
}

glm::vec2 Core::Input::Mouse::GetMousePosition()
{
	return mousePosition;
}

glm::vec2 Core::Input::Mouse::GetMouseDelta()
{
	glm::vec2 delta = glm::vec2(mouseDelta);
	// Reset delta values
	mouseDelta = glm::vec2(0.0f);
	return delta;
}

float Core::Input::Mouse::GetScrollOffset()
{
	float scroll = scrollOffset;
	scrollOffset = 0;
	return scroll;
}

bool Core::Input::Mouse::IsButtonPressed(int button)
{
	return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
}
