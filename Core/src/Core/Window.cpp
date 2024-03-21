#include "Window.h"
#include "Utils.h"

namespace Core
{

    static void GLFWErrorCallback(int error, const char* description)
    {
        Logger::LogError(description);
    }

    void Window::Init()
    {
        ASSERT(glfwInit());
        glfwSetErrorCallback(GLFWErrorCallback);

        _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
        ASSERT(_window);
        glfwMakeContextCurrent(_window);
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

        //Events::Dispatcher<Events::WindowClosedEvent>().Trigger(Events::WindowClosedEvent());
    }

}