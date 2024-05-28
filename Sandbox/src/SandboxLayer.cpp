#include <iostream>
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>
#include "SandboxLayer.h"
#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Core/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"

using namespace Core::Gfx;

Model model1;
Model model2;
Texture texture;
Font font;

void SandboxLayer::OnAttach()
{
    // Setup OpenGL renderer
    Renderer::Init();
    Renderer2D::Init();
    TextRenderer::Init();

    model1 = Model::LoadModel("res/Monkey.obj");
    model2 = Model::LoadModel("res/Ball.obj");
    shader = Shader("res/VertexShader.vs", "res/FragmentShader.fs");
    shader.Use();

    /**** Setup the camera(s) ****/
    cam = Core::Gfx::Camera((float)m_Window->GetWidth() / m_Window->GetHeight());
    cam2d = Core::Gfx::OrthographicCamera(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f);

    texture.Load("res/water.jpg");
    font.Load("res/ocr-a-extended.ttf", 100);
}

static int FONT_SCALE = 3;

void SandboxLayer::OnUpdate()
{
    UpdateCameraController();

    shader.SetFloat("uTime", (float)glfwGetTime());

    Renderer::Clear();

    // Render here
    Renderer::Begin(cam);
    {
        glm::mat4 trans = glm::mat4(1);

        Renderer::DrawModel(model1, shader, trans);

        trans = glm::translate(trans, glm::vec3(10, 0, 0));
        Renderer::DrawModel(model2, shader, trans);
    }
    Renderer::End();

    Renderer2D::Begin(cam2d);
    {
        //Renderer2D::DrawRectangle( Core::Input::Mouse::GetMousePosition() - glm::vec2{ 50.0f, 100.0f }, {100, 200}, {1.0f, 1.0f, 1.0f});
        //Renderer2D::DrawTexture(font.GetTexture(), glm::vec2(100, 150), glm::vec2(1, 1));
        //Renderer2D::DrawLine({ 100, 100 }, { 200, 100 }, { 1, 0, 0 });
    }
    Renderer2D::End();

    TextRenderer::Begin(cam2d);
    {
        const char* message = "Engine demo";
        
        glm::ivec2 offset = font.MeasureText(message, FONT_SCALE);
        float x = m_Window->GetWidth() - offset.x - 10;
        float y = offset.y + 10;
        TextRenderer::DrawTextEx(font, message, x, y, FONT_SCALE, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 1));
    }
    TextRenderer::End();
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
            cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
            shader.SetMatrix("uProjection", cam.GetProjectionMatrix());
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            in_FOV = 75.0F;
            float aspectRatio = (float)m_Window->GetWidth() / m_Window->GetHeight();
            cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
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
        glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspectRatio, 0.1f, 1000.0f);
        cam.SetProjectionMatrix(projection);
        shader.SetMatrix("uProjection", projection);
    }
}

void SandboxLayer::OnDettach()
{
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
