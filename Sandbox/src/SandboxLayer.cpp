#include <iostream>
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>
#include "SandboxLayer.h"
#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Core/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Model.h"

Core::Gfx::Renderer* renderer = nullptr;
Core::Gfx::VertexArray* va = nullptr;
Core::Gfx::Model model;

void SandboxLayer::OnAttach()
{
    renderer = new Core::Gfx::Renderer();

    //// Setup buffers and layouts
    va = new Core::Gfx::VertexArray();
    model = Core::Gfx::LoadFromOBJ("res/TestModel.obj", *va);
    shader = Shader("res/VertexShader.vs", "res/FragmentShader.fs");
    shader.Use();

    /**** Load texture ****/
    texture.Load("res/TestModel_tex.png");

    /**** Transformation matrix ****/
    trans = glm::mat4(1.0f);
    shader.SetMatrix("uTransform", trans);

    /**** Setup the camera ****/
    cam = Core::Camera((float)m_Window->GetWidth() / m_Window->GetHeight());
    shader.SetMatrix("uView", cam.GetViewMatrix());
    shader.SetMatrix("uProjection", cam.GetProjectionMatrix());
}

void SandboxLayer::OnUpdate()
{
    UpdateCameraController();

    // Render here
    renderer->Clear();
    renderer->Draw(*va, model.GetIndexBuffer());
}

void SandboxLayer::ShowCameraControlImgui(bool* p_open)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_Tooltip;
    if (ImGui::Begin("Switch perspective"), p_open, window_flags)
    {
        
        static float in_FOV = 75.0F;
        if (ImGui::SliderFloat("FOV", &in_FOV, 30.0f, 120.0f))
        {
            float aspectRatio = (float)m_Window->GetWidth() / m_Window->GetHeight();
            cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.0f, 1000.0f));
            shader.SetMatrix("uProjection", cam.GetProjectionMatrix());
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            in_FOV = 75.0F;
            float aspectRatio = (float)m_Window->GetWidth() / m_Window->GetHeight();
            cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.0f, 1000.0f));
            shader.SetMatrix("uProjection", cam.GetProjectionMatrix());
        }

        // Display a 2D representation of the perspective frustum
        ImVec2 startPos = ImGui::GetCursorScreenPos();
        startPos.x += ImGui::GetWindowSize().x / 2;
        startPos.y += 100;
        ImVec2 size = ImVec2(100.0f, 100.0f); // Adjust size as needed
        ImVec2 endPos = ImVec2(startPos.x + size.x, startPos.y + size.y);

        // Draw the outline of the frustum
        float halfFOV = glm::radians(in_FOV) / 2.0f;
        float nearDistance = 0.0f;
        float farDistance = 1000.0f;
        float aspectRatio = (float)m_Window->GetWidth() / m_Window->GetHeight();

        float nearHeight = tan(halfFOV) * nearDistance / 20.0f;
        float farHeight = tan(halfFOV) * farDistance / 20.0f;

        ImColor col1(ImGui::GetStyle().Colors[ImGuiCol_Text]);
        ImColor col2(ImGui::GetStyle().Colors[ImGuiCol_Button]);

        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x + size.x / 2, endPos.y - nearDistance * 25.0f), ImVec2(startPos.x - size.x / 2, endPos.y - nearDistance * 25.0f), col1, 2.0f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x + size.x / 2, endPos.y - farDistance / 10.0f), ImVec2(startPos.x - size.x / 2, endPos.y - farDistance / 10.0f), col1, 2.0f);
        
        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x, endPos.y), ImVec2(startPos.x + farHeight / 2, startPos.y - 25), col2, 2.0f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(startPos.x, endPos.y), ImVec2(startPos.x - farHeight / 2, startPos.y - 25), col2, 2.0f);
    }
    ImGui::End();
}

void SandboxLayer::OnImGuiRender()
{
    // ImGui content
    ImGui::ShowDemoWindow();

    // Theme switcher
    if (ImGui::Begin("Switch theme"))
    {
        if (ImGui::Button("Switch to dark/light mode"))
        {
            Core::ImGuiLayer::SwitchTheme(!Core::ImGuiLayer::IsDarkThemed());
        }
        ImGui::Text(ICON_FA_PAINT_BRUSH "  Paint"           );
        ImGui::Text(ICON_FA_ARROW_DOWN  "  Down"            );
        ImGui::Text(ICON_FA_GAMEPAD     "  Preview"         );
        ImGui::Text(ICON_FA_RUNNING     "  Animation"       );
        ImGui::Text(ICON_FA_CAMERA      "  Camera"          );
        ImGui::Text(ICON_FA_BUG         "  Debug"           );
        ImGui::Text(ICON_FA_TOILET_PAPER"  Toilet paper"    );
        ImGui::Text(ICON_FA_CLOCK       "  Clock"           );
        ImGui::Text(ICON_FA_CODE        "  Coding"          );
        ImGui::Text(ICON_FA_COG         "  Settings"        );
        ImGui::Text(ICON_FA_MUSIC       "  Music"           );

    }
    ImGui::End();

    static bool show_tree_open = true;
    ShowCameraControlImgui(&show_tree_open);
}

void SandboxLayer::OnEvent(Core::Events::Event& event)
{
    if (event.GetType() == "WindowResizedEvent")
    {
        auto& e = static_cast<Core::Events::WindowResizedEvent&>(event);
        glViewport(0, 0, e.width, e.height);
        float aspectRatio = (float)e.width / e.height;
        glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspectRatio, 0.0f, 1000.0f);
        cam.SetProjectionMatrix(projection);
        shader.SetMatrix("uProjection", projection);
    }
}

void SandboxLayer::OnDettach()
{
    model.Free();
    delete va, renderer;
}

void SandboxLayer::UpdateCameraController()
{
    using namespace Core::Input;

    // Calculate view matrix
    if (!Core::ImGuiLayer::OnTop())
    {
        cam.Move();
    }

    // Update camera position based on user input
    shader.SetMatrix("uView", cam.GetViewMatrix());
}
