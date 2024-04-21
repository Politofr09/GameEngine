#include "Keyboard.h"
#include "Window.h"
#include "Events/InputEvents.h"
#include "Events/WindowEvents.h"

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
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        // Toggle full screen mode
        if (glfwGetWindowMonitor(window) == nullptr)
        {
            // Switch to full screen
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            glfwSetWindowMonitor(window, primaryMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);

            Events::WindowResizedEvent e(mode->width, mode->height);
            Events::Dispatcher::Trigger(e);
        }
        else
        {
            // Switch to windowed mode
            glfwSetWindowMonitor(window, nullptr, 100, 100, 800, 600, GLFW_DONT_CARE);
            glfwMaximizeWindow(window);
        }
    }

	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(window, true);

	keys[key] = action != GLFW_RELEASE;
	keysDown[key] = action == GLFW_PRESS;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        Events::KeyPressedEvent e(key);
        Events::Dispatcher::Trigger(e);
    }
    else if (action == GLFW_RELEASE)
    {
        Events::KeyReleasedEvent e(key);
        Events::Dispatcher::Trigger(e);
    }
}
