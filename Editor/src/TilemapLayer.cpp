#include "TilemapLayer.h"
#include "CommonUI.h"
#include <imgui/imgui.h>
#include "Core/AssetRegistry.h"
#include "Renderer/Font.h"
#include "Core/Application.h"

using namespace Core;
using namespace Core::Gfx;

void TilemapLayer::OnAttach()
{
	Core::Events::Dispatcher::Subscribe(std::bind(&TilemapLayer::OnEvent, this, std::placeholders::_1));

    m_Camera = Core::Gfx::OrthographicCamera(0.0f, (float)m_Application->GetWindow()->GetWidth(), (float)m_Application->GetWindow()->GetHeight(), 0.0f);
    m_FrameBuffer = FrameBuffer(m_Application->GetWindow()->GetWidth(), m_Application->GetWindow()->GetHeight());
}

void TilemapLayer::OnUpdate()
{
    m_FrameBuffer.Bind();
    Renderer::SetBackgroundColor({0.07f, 0.1f, 0.37f});
    Renderer::OnViewportResize(m_FrameBuffer.GetWidth(), m_FrameBuffer.GetHeight());
    Renderer::Clear();
    Renderer2D::Begin(m_Camera);
    {

        Renderer2D::DrawRectangle({ 100, 100 }, { 100, 200 }, { 1.0f, 0.0f, 0.5f });
        Renderer2D::DrawLine({ -1000.0f, 1000.0f }, { 1000.0f, -1000.0f }, glm::vec3(1.0f));
        Renderer2D::DrawLine({ -1000.0f, -1000.0f }, { 1000.0f, 1000.0f }, glm::vec3(1.0f));

        Renderer2D::DrawLine({ 100, 100 }, { 200, 100 }, { 1, 0, 0 });

        Renderer2D::DrawPolygon(Renderer2D::GeneratePolygonVertices(8), glm::vec2(20.0f, 400.0f), glm::vec2(40.0f, 40.0f), glm::vec3(1.0f, 0.0f, 1.0f)); // Draw a red triangle
    
    }
    Renderer2D::End();
    //TextRenderer::Begin(m_Camera);
    //{
    //    const char* message = "Tilemap Editor";
    //    
        //Font* font = ResourceManager::get().GetFirstByName<Font>("OCR-Font");
        //TextRenderer::DrawTextEx(*font, message, 500, 500, 10, glm::vec3(1.0f));
    //}
    //TextRenderer::End();
    m_FrameBuffer.UnBind();
}

void TilemapLayer::OnImGuiRender()
{

    DrawViewport();

}

void TilemapLayer::DrawViewport()
{
    static ImVec2 availableSpace = ImVec2();
    ImGui::GetStyle().WindowPadding = { 1.5f, 1.5f };
    if (ImGui::Begin("Tilemap", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        if (availableSpace.x != ImGui::GetContentRegionAvail().x || availableSpace.y != ImGui::GetContentRegionAvail().y)
        {
            availableSpace = ImGui::GetContentRegionAvail();
            m_FrameBuffer.Resize((int)availableSpace.x, (int)availableSpace.y);
            m_Camera.OnViewportResize((int)availableSpace.x, (int)availableSpace.y);
        }
        
        m_FrameBuffer.SetPosition(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 6);

        ImVec2 min = ImGui::GetCursorScreenPos();
        ImVec2 max = ImVec2(min.x + availableSpace.x, min.y + availableSpace.y);
        ImGui::GetWindowDrawList()->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_WindowBg)), 6.0f, ImDrawFlags_RoundCornersAll, 6.0f);
        
        ImGui::BeginChild("TilemapViewport", availableSpace, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
        DisplayRoundedImage((ImTextureID)m_FrameBuffer.GetTextureID(), ImVec2(m_FrameBuffer.GetWidth(), m_FrameBuffer.GetHeight()), 6.0f, ImVec2(0, 1), ImVec2(1, 0));
        if (ImGui::IsWindowHovered())
        {
            m_Camera.Move();
        }
        ImGui::EndChild();

    }
    ImGui::End();
}

void TilemapLayer::OnEvent(Core::Events::Event* event)
{
}

void TilemapLayer::OnDettach()
{
}
