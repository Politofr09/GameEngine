#include <iostream>
#include <imgui/imgui.h>
#include <imgui/IconsFontAwesome5.h>

#include "EditorLayer.h"
#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Core/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"

using namespace Core::Gfx;

Model model1;
Model model2;
Font font;
Shader shader;
Texture texture;

void EditorLayer::OnAttach()
{
    // Setup OpenGL renderer
    Renderer::Init();
    Renderer2D::Init();
    TextRenderer::Init();

    shader = Shader("res/VertexShader.vs", "res/FragmentShader.fs");

    model1 = Model::LoadModel("res/Monkey.obj");

    model2 = Model::LoadModel("res/Ball.obj");

    model1.GetMaterial().SetShader(shader);
    model2.GetMaterial().SetShader(shader);

    /**** Setup the camera(s) ****/
    m_Cam.SetAspectRatio((float)m_Window->GetWidth() / m_Window->GetHeight());
    m_Cam2d = Core::Gfx::OrthographicCamera(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f);

    texture = Texture("res/water.jpg");

    font = Font("res/ocr-a-extended.ttf", 100);
    
    m_FrameBuffer = FrameBuffer(m_Window->GetWidth(), m_Window->GetHeight());
}

static int FONT_SCALE = 3;

void EditorLayer::OnUpdate()
{
    // UpdateCameraController();
    shader.SetFloat("uTime", (float)glfwGetTime());

    Renderer::Clear();

    m_FrameBuffer.Bind();
    Renderer::Begin(m_Cam);
    {
        Renderer::Clear();

        glm::mat4 trans = glm::mat4(1);

        Renderer::DrawModel(model1, trans);
        
        trans = glm::translate(trans, glm::vec3(10, 0, 0)); 
        Renderer::DrawModel(model2, trans);
    }
    Renderer::End();

    Renderer2D::Begin(m_Cam2d);
    {
        //Renderer2D::DrawRectangle( Core::Input::Mouse::GetMousePosition() - glm::vec2{ 50.0f, 100.0f } + glm::vec2{  }, {100, 200}, {1.0f, 1.0f, 1.0f});
        Renderer2D::DrawTexture(texture, 
            { 
                Core::Input::Mouse::GetMousePosition().x - m_FrameBuffer.GetOffsetX() * m_FrameBuffer.GetAspectRatio(),
                Core::Input::Mouse::GetMousePosition().y - m_FrameBuffer.GetOffsetY() * m_FrameBuffer.GetAspectRatio()
            });
        Renderer2D::DrawLine({ 100, 100 }, { 200, 100 }, { 1, 0, 0 });
    }
    Renderer2D::End();

    TextRenderer::Begin(m_Cam2d);
    {
        const char* message = "Engine demo";
        
        glm::ivec2 offset = font.MeasureText(message, FONT_SCALE);
        float x = m_FrameBuffer.GetWidth() - offset.x - 10;
        float y = offset.y + 10;
        TextRenderer::DrawTextEx(font, message, x, y, FONT_SCALE, glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 1));
    }
    TextRenderer::End();
    m_FrameBuffer.UnBind();
}

void EditorLayer::ShowCameraControlImgui(bool* p_open)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_Tooltip;
    if (ImGui::Begin("Switch perspective"), p_open, window_flags)
    {
        
        static float in_FOV = 75.0F;
        if (ImGui::SliderFloat("FOV", &in_FOV, 30.0f, 120.0f))
        {
            float aspectRatio = (float)m_Window->GetWidth() / m_Window->GetHeight();
            m_Cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
            shader.SetMatrix("uProjection", m_Cam.GetProjectionMatrix());
        }

        ImGui::SameLine();
        if (ImGui::Button("Reset"))
        {
            in_FOV = 75.0F;
            float aspectRatio = (float)m_Window->GetWidth() / m_Window->GetHeight();
            m_Cam.SetProjectionMatrix(glm::perspective(glm::radians(in_FOV), aspectRatio, 0.01f, 1000.0f));
            shader.SetMatrix("uProjection", m_Cam.GetProjectionMatrix());
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

void EditorLayer::OnImGuiRender()
{
    // Setup dockspace
    ImGui::SetNextWindowSize(ImVec2(m_Window->GetWidth(), m_Window->GetHeight()));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    int windowFlags = ImGuiWindowFlags_MenuBar |    ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar   |           ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove       |           ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_NoNav        |           ImGuiWindowFlags_MenuBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Dockspace", nullptr, windowFlags);
    ImGui::PopStyleVar(3);
    ImVec2 min = ImGui::GetCursorScreenPos();
    ImVec2 max = ImVec2(min.x + ImGui::GetContentRegionAvail().x, min.y + ImGui::GetContentRegionAvail().y);
    ImGui::GetWindowDrawList()->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg)), 6.0f, ImDrawFlags_RoundCornersAll, 6.0f);

    ImGui::DockSpace(ImGui::GetID("Dockspace"));

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                m_Window->Close();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

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

    ImGui::GetStyle().WindowPadding = { 1.5f, 1.5f };
    if (ImGui::Begin("VIEWPORT", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImVec2 availableSpace = ImGui::GetContentRegionAvail();
        m_FrameBuffer.Resize((int)availableSpace.x, (int)availableSpace.y);
        m_FrameBuffer.SetPosition(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 6);

        ImVec2 min = ImGui::GetCursorScreenPos();
        ImVec2 max = ImVec2(min.x + availableSpace.x, min.y + availableSpace.y);
        ImGui::GetWindowDrawList()->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg)), 6.0f, ImDrawFlags_RoundCornersAll, 6.0f);
        
        ImGui::BeginChild("Viewport", availableSpace, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
        Core::ImGuiLayer::DisplayRoundedImage((ImTextureID)m_FrameBuffer.GetTextureID(), ImVec2(m_FrameBuffer.GetWidth(), m_FrameBuffer.GetHeight()), 6.0f, ImVec2(0, 1), ImVec2(1, 0));
        if (ImGui::IsWindowHovered())
        {
            m_Cam.Move();
        }
        ImGui::EndChild();

    }
    ImGui::End();

    ImGui::End(); // Dockspace
}

void EditorLayer::OnEvent(Core::Events::Event* event)
{
    if (event->GetType() == "WindowResizedEvent")
    {
        auto e = dynamic_cast<Core::Events::WindowResizedEvent*>(event);

        Renderer::OnViewportResize(e->width, e->height);
    }
    if (event->GetType() == "ViewportResizedEvent")
    {
        auto e = dynamic_cast<Core::Events::ViewportResizedEvent*>(event);

        Renderer::OnViewportResize(e->width, e->height);

        m_Cam.OnViewportResize(e->width, e->height);
        m_Cam2d.OnViewportResize(e->width, e->height);
    }
}

void EditorLayer::OnDettach()
{
    // Cleanup
}

//void EditorLayer::UpdateCameraController()
//{
//    using namespace Core::Input;
//
//    // Calculate view matrix
//    if (!Core::ImGuiLayer::OnTop())
//    {
//        m_Cam.Move();
//    }
//
//    // Update camera position based on user input
//    shader.SetMatrix("uView", m_Cam.GetViewMatrix());
//}

void EditorLayer::DrawViewportImGui()
{

}