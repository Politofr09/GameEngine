#include <GL/glew.h>
#include "Window.h"
#include "Utils.h"
#include "Events/WindowEvents.h"
#include "Events/InputEvents.h"

namespace Core
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        Logger::LogError(description);
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            Events::KeyPressedEvent e(key);
            Events::Dispatcher::Trigger(e);
            e.Handled = true;
        } 
        else if (action == GLFW_RELEASE)
        {
            Events::KeyReleasedEvent e(key);
            Events::Dispatcher::Trigger(e);
            e.Handled = true;
        }
    }

    void window_resize_callback(GLFWwindow* window, int width, int height)
    {
        // Trigger WindowResizedEvent
        Events::WindowResizedEvent e(width, height);
        Events::Dispatcher::Trigger(e);
    }

    void window_pos_callback(GLFWwindow* window, int xpos, int ypos)
    {
        // Trigger WindowMovedEvent
        Events::WindowMovedEvent e(xpos, ypos);
        Events::Dispatcher::Trigger(e);
    }

    void Window::Init()
    {
        ASSERT(glfwInit());
        glfwSetErrorCallback(GLFWErrorCallback);

        _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
        ASSERT(_window);
        glfwMakeContextCurrent(_window);

        // Setup callbacks
        glfwSetKeyCallback(_window, key_callback);
        glfwSetWindowSizeCallback(_window, window_resize_callback);
        glfwSetWindowPosCallback(_window, window_pos_callback);

        // Init glew
        ASSERT(glewInit() == GLEW_OK);
    }

    Window::Window(uint32_t w, uint32_t h, const std::string& title)
    {
        _width = w;
        _height = h;

        _title = title;
        Init();
    }

    uint32_t Window::GetWidth()
    {
        return _width;
    }

    uint32_t Window::GetHeight()
    {
        return _height;
    }

    void Window::Update()
    {
        glfwPollEvents();
        glfwSwapBuffers(_window);
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(_window);
    }

    void Window::Close()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();

        Events::WindowClosedEvent e;
        Events::Dispatcher::Trigger(e);
    }

}