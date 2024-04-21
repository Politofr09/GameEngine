#include "Keyboard.h"

bool Core::Input::Keyboard::keys[65536] = { false };
bool Core::Input::Keyboard::keysDown[65536] = { false };


bool Core::Input::Keyboard::IsKeyDown(int keyCode)
{
    return keys[keyCode];
}

bool Core::Input::Keyboard::IsKeyPressed(int keyCode)
{
    bool keyPressed = keysDown[keyCode];
    keysDown[keyCode] = false;
    return keyPressed;
}

void Core::Input::Keyboard::Init(GLFWwindow* window)
{
	glfwSetKeyCallback(window, glfw_event_poll);
}

void Core::Input::Keyboard::glfw_event_poll(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(window, true);

	keys[key] = action != GLFW_RELEASE;
	keysDown[key] = action == GLFW_PRESS;
}
