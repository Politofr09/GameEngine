#include <GL/glew.h>
#include "Window.h"
#include "Utils.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Events/WindowEvents.h"
#include "Events/InputEvents.h"
#include <imgui/imgui.h>

namespace Core
{
    float Window::_deltaTime = 0.0f;
    std::chrono::steady_clock::time_point Window::_prevTime;

    static void GLFWErrorCallback(int error, const char* description)
    {
        Logger::LogError(description);
    }

    void Window::window_resize_callback(GLFWwindow* window, int width, int height)
    {
        // Trigger WindowResizedEvent
        Events::WindowResizedEvent* e = new Events::WindowResizedEvent(width, height);
        Events::Dispatcher::Trigger(e);
    }

    void Window::window_pos_callback(GLFWwindow* window, int xpos, int ypos)
    {
        // Trigger WindowMovedEvent
        Events::WindowMovedEvent* e = new Events::WindowMovedEvent(xpos, ypos);
        Events::Dispatcher::Trigger(e);
    }

    void Window::Init(bool decorated)
    {
        ASSERT(glfwInit());
        glfwSetErrorCallback(GLFWErrorCallback);

        if (!decorated) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
        ASSERT(_window);
        glfwMakeContextCurrent(_window);

        // Setup callbacks
        //glfwSetKeyCallback(_window, key_callback);
        glfwSetWindowSizeCallback(_window, window_resize_callback);
        glfwSetWindowPosCallback(_window, window_pos_callback);
        glfwMaximizeWindow(_window);
        Input::Keyboard::Init(_window);
        Input::Mouse::Init(_window);

        // Add another callback when the window resizes...
        Events::Dispatcher::Subscribe([&](Events::Event* event){
            if (event->GetType() != "WindowResizedEvent") return;

            auto e = dynamic_cast<Events::WindowResizedEvent*>(event);
            _width = e->width;
            _height = e->height;
        });

        // Init glew
        ASSERT(glewInit() == GLEW_OK);

        _prevTime = std::chrono::steady_clock::now();
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
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupWindowPtr = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupWindowPtr);
        }

        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - _prevTime;
        _deltaTime = elapsedTime.count();
        _prevTime = currentTime;

        glfwPollEvents();
        glfwSwapBuffers(_window);
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(_window);
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(_window, true);
    }

    void Window::Maximize()
    {
        if (glfwGetWindowAttrib(_window, GLFW_MAXIMIZED))
            glfwRestoreWindow(_window);
        else
            glfwMaximizeWindow(_window);
    }

    void Window::Minimize()
    {
        glfwIconifyWindow(_window);
    }
}