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
    float Window::m_DeltaTime = 0.0f;
    std::chrono::steady_clock::time_point Window::m_PrevTime;

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
        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        ASSERT(m_Window);
        glfwMakeContextCurrent(m_Window);

        // Setup callbacks
        //glfwSetKeyCallback(_window, key_callback);
        glfwSetWindowSizeCallback(m_Window, window_resize_callback);
        glfwSetWindowPosCallback(m_Window, window_pos_callback);
        glfwMaximizeWindow(m_Window);
        Input::Keyboard::Init(m_Window);
        Input::Mouse::Init(m_Window);

        // Add another callback when the window resizes...
        Events::Dispatcher::Subscribe([&](Events::Event* event){
            if (event->GetType() != "WindowResizedEvent") return;

            auto e = dynamic_cast<Events::WindowResizedEvent*>(event);
            m_Width = e->width;
            m_Height = e->height;
        });

        // Init glew
        ASSERT(glewInit() == GLEW_OK);

        m_PrevTime = std::chrono::steady_clock::now();
    }

    Window::Window(uint32_t w, uint32_t h, const std::string& title)
    {
        m_Width = w;
        m_Height = h;

        m_Title = title;
        Init();
    }

    uint32_t Window::GetWidth()
    {
        return m_Width;
    }

    uint32_t Window::GetHeight()
    {
        return m_Height;
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
        std::chrono::duration<float> elapsedTime = currentTime - m_PrevTime;
        m_DeltaTime = elapsedTime.count();
        m_PrevTime = currentTime;

        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::Close()
    {
        glfwSetWindowShouldClose(m_Window, true);
    }

    void Window::Maximize()
    {
        if (glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED))
            glfwRestoreWindow(m_Window);
        else
            glfwMaximizeWindow(m_Window);
    }

    void Window::Minimize()
    {
        glfwIconifyWindow(m_Window);
    }

    void Window::SetTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_Window, title.c_str());
    }
}